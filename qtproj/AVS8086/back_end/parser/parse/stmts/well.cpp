#include "parser/parser.h"
#include "ast/stmts/well.h"
#include "ast/exprs/value.h"
#include "ast/exprs/make_x.h"
#include "ast/exprs/load_x.h"
#include "ast/exprs/assign.h"
#include "ast/exprs/register.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_well()
{
    int row = currToken().row();
    int col = currToken().column();

    nextToken();
    if (currToken().is(Token::WELL))
    {
        addInfo(
            Info::WARNING, {row, col, currToken().endColumn() - col},
            "the two '#' are empty"
        );
        return StmtPointer(new Well);
    }

    auto e = parse_expression();

    if (peekToken().is(Token::TOKEN_EOL))
    {
        addInfo(
            Info::ERROR, {row, col, currToken().endColumn() - col},
            "'#' is not close on this line"
        );
    }
    else if (!expectPeekToken(true, Token::WELL))
    {
        addExpectTokenErrorInfo(peekToken(), {Token::WELL});
    }

    int len = currToken().endColumn() - col;
    QString key;
    QString value;

    if (e->is(Node::MAKE_X))
    {
        key = "MAKE_";
        value = assert_dynamic_cast<Make_X>(e)->value();
    }
    else if (e->is(Node::ASSIGN))
    {
        auto a = assert_dynamic_cast<Assign>(e);
        auto al = a->left();
        if (al->is(Node::REGISTER))
        {
            auto r = assert_dynamic_cast<Register>(al);
            if (r->regType() == Token::REG8)
            {
                static const QList<Token::Type> s_stopType = {
                    Token::LOAD_X, Token::SREG, Token::REG16,
                };
                addExpectTokenErrorInfo(r->token(), s_stopType);
            }
            else
            {
                key = r->str();
            }
        }
        else if (al->is(Node::LOAD_X))
        {
            key = assert_dynamic_cast<Load_X>(al)->value();
        }

        if (!key.isEmpty())
        {
            auto ar = a->right();
            if (expectExprAbleToEvaluate(ar))
            {
                auto v = ar.dynamicCast<Value>();
                if (!v.isNull())
                {
                    bool ok;
                    auto i = v->integer(&ok);
                    if (!ok || i > 0xffff)
                    {
                        addExprVOverflowErrorInfo(ar, 0xffff);
                    }
                    else
                    {
                        value = QString("%1").arg(i, 4, 16, QChar('0'));
                    }
                }
            }
        }
    }
    else
    {
        addExpectExprErrorInfo(e, {Node::MAKE_X, Node::ASSIGN});
    }

    if (!(key.isEmpty() || value.isEmpty()))
    {
        // 检测是否有重复(警告)
        auto it = m_wellInstructions.find(key);
        if (it == m_wellInstructions.end())
        {
            m_wellInstructions.insert(key, {row, col, len});
        }
        else
        {
            if (!it.value().isEmpty())
            {
                addInfo(
                    Info::WARNING,
                    {it.value().at(0), it.value().at(1), it.value().at(2)},
                    "repeated '#'instructions: " + it.key()
                );
                it.value().clear();
            }
            addInfo(
                Info::WARNING, {row, col, len},
                "repeated '#'instructions: " + key
            );
        }
    }

    return StmtPointer(new Well(e, key, value));
}

/* ========================================================================== */

#if 0
StmtPointer Parser::parse_well()
{
    int row = currToken().row();
    int col = currToken().column();

    TokenList tokens;

    static const QList<Token::Type> s_stopType = {
        Token::WELL, Token::TOKEN_EOL, Token::TOKEN_EOF,
    };

    while (expectPeekTokenNot(s_stopType))
    {
        tokens.append(currToken());
    }

    if (expectPeekToken(Token::WELL, false))
    {
        if (tokens.isEmpty())
            addInfo(Info::WARNING, {row, col, 2}, "the two '#' are empty");
    }
    else
    {
        addInfo(Info::ERROR, {row, col, peekToken().endColumn() - col},
                "'#' is not close on this line");
    }
    if (tokens.isEmpty())
        return StmtPointer(new Well);

    int len = peekToken().endColumn() - col;
    QString instruction = Lexer::restore(tokens);
    QString key, value;

    if (tokens.size() == 1 && tokens.at(0).is(Token::MAKE_X))
    {
        key = tokens.at(0).literal().left(5).toUpper();
        value = tokens.at(0).literal().right(3);
    }
    else
    {
        switch (tokens.at(0).type())
        {
        case Token::LOAD_X:
        case Token::SREG:
        case Token::REG8:
        case Token::REG16:
            if (tokens.size() == 3)
            {
                if (!tokens.at(1).is(Token::ASSIGN))
                {
                    addExpectTokenErrorInfo(tokens.at(1), {Token::ASSIGN});
                }
                else if (!tokens.at(2).is(Token::INTEGER))
                {
                    addExpectTokenErrorInfo(tokens.at(2), {Token::INTEGER});
                }
                else
                {
                    if (tokens.at(0).is(Token::REG8))
                    {
                        col = tokens.at(0).column();
                        len = tokens.at(0).endColumn() - col;
                        addInfo(
                            Info::ERROR, {row, col, len},
                            "register need to use 16 bit instead of 8 bit"
                        );
                        return StmtPointer(new Well(instruction, key, value));
                    }
                    key = tokens.at(0).literal().toUpper() + "=";
                    value = tokens.at(2).literal();
                    // 检测取值是否合理(警告)
                    int max = tokens.at(0).is(Token::REG8) ? 0xff : 0xffff;
                    int v = Token::textToInt(value);
                    if (!(0 <= v && v <= max))
                    {
                        addInfo(
                            Info::WARNING,
                            {row, tokens.at(2).column(), tokens.at(2).length()},
                            QString(
                                "integer should between 0H and %2H, but %1H")
                                .arg(v, 0, 16).arg(max, 0, 16)
                        );
                    }
                    break;
                }
            }
        default:
            addInfo(
                Info::ERROR, {row, col, len},
                "illegal '#' instruction: " + instruction
            );
            return StmtPointer(new Well(instruction, key, value));
        }
    }

    // 检测是否有重复(警告)
    auto it = m_wellInstructions.find(key);
    if (it == m_wellInstructions.end())
    {
        m_wellInstructions.insert(key, {row, col, len});
    }
    else
    {
        if (!it.value().isEmpty())
        {
            addInfo(
                Info::WARNING,
                {it.value().at(0), it.value().at(1), it.value().at(2)},
                "repeated instructions: " + it.key()
            );
            it.value().clear();
        }
        addInfo(
            Info::WARNING, {row, col, len},
            "repeated instructions: " + key
        );
    }
    return StmtPointer(new Well(instruction, key, value));
}
#endif

/* ========================================================================== */
