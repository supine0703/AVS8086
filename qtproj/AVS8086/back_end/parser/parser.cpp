#include "parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

/* ========================================================================== */

void Parser::clear()
{
    m_err = false;
    m_infos.clear();
}

QSharedPointer<Program> Parser::newAST()
{
    clear();

    if (m_lexer == nullptr)
        return nullptr;

    m_err = m_lexer->isError();
    if (m_err)
        m_infos.insert(m_lexer->infos());

    fristToken();
    auto p = parse_program();

    m_tokens.clear();
    m_wellInstructions.clear();
    return p;
}

/* ========================================================================== */

void Parser::fristToken()
{
#if 0
    m_tokens.append(m_lexer->first());
    while (currToken().is(Token::ANNOTATION))
    {
        m_tokens.pop_back();
        m_tokens.append(m_lexer->next());
    }
    m_tokens.append(m_tokens.at(0));
    m_tokens.append(m_lexer->next());
    nextToken();
#endif
    m_tokens.resize(2, Token());
    m_tokens.append(m_lexer->first());
    nextToken();
    // 尚未开始时, 第 0 个是无效的, 作为缓冲
}

void Parser::nextToken()
{
    // 缓冲区去除注释
    while (m_tokens.back().is(Token::ANNOTATION))
    {
        m_tokens.removeLast();
        m_tokens.append(m_lexer->next());
    }

    m_tokens.removeFirst();
#if 1
    m_tokens.append(m_lexer->next());
#else
    int currRow = currToken().row();
    int nextRow = peekToken().row();
    if (currRow < nextRow)
    { // 行结束
        // 这里EOR使用 下一行行号, 这一列列号, 一个是为了作区分, 另一个是为了方便使用
        m_tokens.insert(
            1,
            Token(
                Token::TOKEN_EOF,
                currToken().literal(),
                nextRow,
                currToken().column()
                )
            );
    }
    else
        m_tokens.append(m_lexer->next());
#endif
}

bool Parser::expectPeekToken(bool is, Token::Type type)
{
    if (is != peekToken().is(type))
        return false;
    nextToken();
    return true;
}

bool Parser::expectPeekToken(bool is, const QList<Token::Type>& types)
{
    for (auto type : types)
        if (peekToken().is(type))
        {
            is &= true;
            break;
        }
    if (is)
        nextToken();
    return is;
}

#if 0
bool Parser::expectExpression(Node::Type type, const ExprPointer& expr)
{
    if (expr->is(type))
        return true;
    addExpectExpressionErrorInfo({type}, expr);
    return false;
}

bool Parser::expectExpression(
    const QList<Node::Type>& types, const ExprPointer& expr)
{
    for (auto type : types)
        if (expr->is(type))
            return true;
    addExpectExpressionErrorInfo(types, expr);
    return false;
}
#endif

Parser::Precedence Parser::precedence(Token::Type type)
{
    auto it = sm_precedences.find(type);
    if (it != sm_precedences.end())
        return it.value();
    return LOWEST;
}

/* ========================================================================== */

void Parser::defineId(const StmtPointer& stmt)
{
    if (!currToken().is(Token::IDENTIFIER))
    {
        addExpectTokenErrorInfo(currToken(), {Token::IDENTIFIER});
        return ;
    }

    if (m_idIts.contains(*currToken()))
    {
        addInfo(
            Info::ERROR, currToken().position(),
            "redefined identifier: " + *currToken()
        );
        return ;
    }

    m_idIts.insert(*currToken(), m_offsers.length());
    m_ids.insert(m_offsers.length(), stmt);
    m_offsers.append(m_currOffset);
    m_currOffset = 0;
}

void Parser::callId(const ast::StmtPointer& stmt)
{
    if (!currToken().is(Token::IDENTIFIER))
    {
        addExpectTokenErrorInfo(currToken(), {Token::IDENTIFIER});
        return ;
    }

    m_calls.insert(m_offsers.length(), stmt);
    m_offsers.append(m_currOffset);
    m_currOffset = 0;
}

/* ========================================================================== */

void Parser::addInfo(Info::Type type, const Position& pos, const QString& info)
{
    m_infos.insert({type, pos, info});
}

void Parser::addExpectTokenErrorInfo(
    const Token& token, const QList<Token::Type>& types)
{
    if (types.isEmpty())
    {
        addInfo(
            Info::ERROR, token.position(),
            QString("expected this token to be not '%1'").arg(token.content())
        );
        return ;
    }
    QString expect = Token::typeName(types.at(0));
    for (int i = 1, end = types.count(); i < end; i++)
        expect += "," + Token::typeName(types.at(i));
        // expect += QString(",%1").arg(Token::typeName(types.at(i)));

    QString but;
    if (token.is(Token::TOKEN_EOL) || token.is(Token::TOKEN_EOF))
        but = "line break";
    else
    {
        but = QString("got '%1' instead").arg(token.content());
    }
    addInfo(
        Info::ERROR, token.position(),
        QString("expected this token to be '%1', but %2").arg(expect, but)
    );
}

void Parser::addExpectExprErrorInfo(
    const ExprPointer& expr, const QList<Node::Type>& types)
{
    if (types.isEmpty())
    {
        addInfo(
            Info::ERROR, expr->position(),
            QString("expected this expression to be not '%1'")
                .arg(expr->typeName())
        );
        return ;
    }
    QString expect = Node::typeName(types.at(0));
    for (int i = 1, end = types.count(); i < end; i++)
        expect += "," + Node::typeName(types.at(i));

    QString but;
    if (expr->is(Node::ILLEGAL) && expr->token().is(Token::TOKEN_EOL))
    {
        but = "line break";
    }
    else
    {
        but = QString("got '%1' instead").arg(Node::typeName(expr->type()));
    }

    addInfo(
        Info::ERROR, expr->position(),
        QString("expected this expression to be '%1', but %2").arg(expect, but)
    );
}

void Parser::addExprDivideZeroErrorInfo(const ast::ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->position(),
        "the expression has a division zero error"
    );
}

void Parser::addExprCanNotBeUsedAsIntegerErrorInfo(const ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->position(),
        "can only be used as integer if size of value less than 8 bytes"
    );
}

void Parser::addExprVOverflowErrorInfo(const ExprPointer& expr, size_t max)
{
    addInfo(
        Info::ERROR, expr->position(),
        QString("expect value of expression between 0x0 and 0x%1, but: 0x%2")
            .arg(QString::number(max, 16), show_Integer_hex(expr->bytes()))
    );
}

void Parser::addExprUnableToEvaluateErrorInfo(const ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->position(),
        "this expression unable to evaluate"
    );
}

bool Parser::expectExprAbleToEvaluate(const ExprPointer& expr)
{
    bool unable = expr->unitDataSize() == 0;
    if (unable)
        addExprUnableToEvaluateErrorInfo(expr);
    return !unable;
}

void Parser::addStmtCanNotBeExprErrorInfo()
{
    const auto& token = currToken();
    addInfo(
        Info::ERROR, token.position(),
        QString("this is a statement: '%1', and can not be expression")
            .arg(token.content())
    );
}

void Parser::addNoPrefixParseFnErrorInfo()
{
    const auto& token = currToken();
    if (sm_infix_parseFns.contains(token.type()))
    {
        addInfo(
            Info::ERROR, token.position(),
            QString("this token can not be profix '%1'").arg(token.content())
        );
    }
    else
    {
        addNotYetSupportErrorInfo();
    }
}

void Parser::addReservedWordErrorInfo()
{
    const auto& token = currToken();
    addInfo(
        Info::ERROR, token.position(),
        QString("this is reserved word, unable to process '%1'")
            .arg(token.content())
    );
}

void Parser::addNotYetSupportErrorInfo()
{
    const auto& token = currToken();
    addInfo(
        Info::ERROR, token.position(),
        QString("Not yet support parsing the token '%1'")
            .arg(token.content())
    );
}

void Parser::addJmpOverflowErrorInfo(const Position& pos, int min, int max)
{
    addInfo(
        Info::ERROR, pos,
        QString("jmp range should be: %1 and %2").arg(min).arg(max)
    );
}

/* ========================================================================== */

