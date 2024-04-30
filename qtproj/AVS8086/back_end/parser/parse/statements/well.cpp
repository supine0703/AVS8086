#include "parser/parser.h"
#include "ast/nodes/well.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

StmtPointer Parser::parse_well()
{
    QSharedPointer<Well> s(new Well);

    int row = currToken().row();
    int col = currToken().column();
    int len;

    QList<Token> tokens;

    while (expectPeekTokenNot({Token::WELL, Token::TOKEN_EOF}))
    {
        tokens.append(currToken());
    }

    if (!expectPeekToken(Token::WELL, false))
    {
        addErrorInfo(
            row, col, peekToken().endColumn() - col,
            "'#' is not close on this line"
        );
    }
    if (tokens.isEmpty())
    {
        if (currToken().is(Token::WELL))
            addWarningInfo(row, col, 2, "the two '#' are empty");
        return s;
    }

    len = peekToken().endColumn() - col;
    s->m_instruction = Lexer::restore(tokens);

    if (tokens.length() == 1 && tokens.at(0).is(Token::MAKE_X))
    {
        s->m_key = tokens.at(0).literal().left(5).toUpper();
        s->m_value = tokens.at(0).literal().right(3);
    }
    else
    {
        switch (tokens.at(0).type())
        {
        case Token::LOAD_X:
        case Token::SREG:
        case Token::REG8:
        case Token::REG16:
            if (tokens.length() == 3)
            {
                if (!tokens.at(1).is(Token::ASSIGN))
                {
                    addExpectPeekTokenErrorInfo({Token::ASSIGN}, tokens.at(1));
                }
                else if (!tokens.at(2).is(Token::INTEGER))
                {
                    addExpectPeekTokenErrorInfo({Token::INTEGER}, tokens.at(2));
                }
                else
                {
                    if (tokens.at(0).is(Token::REG8))
                    {
                        col = tokens.at(0).column();
                        len = tokens.at(0).endColumn() - col;
                        addErrorInfo(
                            row, col, len,
                            "register need to use 16 bit instead of 8 bit"
                        );
                        return s;
                    }
                    s->m_key = tokens.at(0).literal().toUpper() + "=";
                    s->m_value = tokens.at(2).literal();
                    // 检测取值是否合理(警告)
                    int max = tokens.at(0).is(Token::REG8) ? 0xff : 0xffff;
                    int value = Token::textToInt(s->m_value);
                    if (!(0 <= value && value <= max))
                    {
                        addWarningInfo(
                            row, tokens.at(2).column(), tokens.at(2).length(),
                            QString(
                                "integer should between 0H and %2H, but %1H")
                                .arg(value, 0, 16).arg(max, 0, 16)
                        );
                    }
                    break;
                }
            }
        default:
            addErrorInfo(
                row, col, len,"illegal '#' instruction: " + s->m_instruction
            );
            return s;
        }
    }

    // 检测是否有重复(警告)
    auto it = m_wellInstructions.find(s->m_key);
    if (it == m_wellInstructions.end())
    {
        m_wellInstructions.insert(s->m_key, {row, col, len});
    }
    else
    {
        if (!it.value().isEmpty())
        {
            addWarningInfo(
                it.value().at(0),
                it.value().at(1),
                it.value().at(2),
                "repeated instructions: " + it.key()
            );
            it.value().clear();
        }
        addWarningInfo(
            row, col, len,
            "repeated instructions: " + s->m_key
        );
    }
    return s;
}
