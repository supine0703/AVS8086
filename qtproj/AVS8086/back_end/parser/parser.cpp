#include "parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

void Parser::clear()
{
    m_error = false;
    m_infos.clear();
}

QSharedPointer<Program> Parser::newAST()
{
    clear();

    if (m_lexer == nullptr)
        return QSharedPointer<Program>(nullptr);

    m_error = m_lexer->isError();
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
    bool match = false;
    for (auto type : types)
    {
        if (peekToken().is(type))
        {
            match = true;
            break;
        }
    }
    if (is == match)
    {
        nextToken();
        return true;
    }
    return false;
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
        return;
    }

    if (m_idIts.contains(*currToken()))
    {
        addInfo(
            Info::ERROR, currToken().pos(),
            "redefined identifier: " + *currToken()
        );
        return;
    }

    m_idIts.insert(*currToken(), m_offsets.size());
    m_ids.insert(m_offsets.size(), stmt);
    m_offsets.append(m_currOffset);
    m_currOffset = 0;
}

void Parser::callId(const ast::StmtPointer& stmt)
{
    if (!currToken().is(Token::IDENTIFIER))
    {
        addExpectTokenErrorInfo(currToken(), {Token::IDENTIFIER});
        return;
    }

    m_calls.insert(m_offsets.size(), stmt);
    m_offsets.append(m_currOffset);
    m_currOffset = 0;
}

/* ========================================================================== */

void Parser::addInfo(Info::Type type, const Position& pos, const QString& info)
{
    if (!m_error && type == Info::ERROR)
    {
        m_error = true;
    }
    m_infos.insert({type, pos, info});
}

bool Parser::haveInfo(Info::Type type, const Position& p, const Position& ep)
{
    return
        m_infos.lowerBound({type, p, ""}) != m_infos.lowerBound({type, ep, ""});
}

void Parser::addExpectTokenErrorInfo(
    const Token& token, const QList<Token::Type>& types)
{
    if (types.isEmpty())
    {
        addInfo(
            Info::ERROR, token.pos(),
            QString("expected this token to be not '%1', but '%2'")
                .arg(token.typeName(), *token)
        );
        return;
    }
    QString expect = Token::typeName(types.at(0));
    for (int i = 1, end = types.size(); i < end; i++)
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
        Info::ERROR, token.pos(),
        QString("expected this token to be '%1', but %2").arg(expect, but)
    );
}

void Parser::addExpectExprErrorInfo(
    const ExprPointer& expr, const QList<Node::Type>& types)
{
    if (types.isEmpty())
    {
        addInfo(
            Info::ERROR, expr->pos(),
            QString("expected this expression to be not '%1'")
                .arg(expr->typeName())
        );
        return;
    }
    QString expect = Node::typeName(types.at(0));
    for (int i = 1, end = types.size(); i < end; i++)
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
        Info::ERROR, expr->pos(),
        QString("expected this expression to be '%1', but %2").arg(expect, but)
    );
}

void Parser::addExprDivideZeroErrorInfo(const ast::ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->pos(),
        "the expression has a division zero error"
    );
}

void Parser::addExprCannotBeUsedAsIntegerErrorInfo(const ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->pos(),
        "can only be used as integer if size of value less than 8 bytes"
    );
}

void Parser::addValueOverflowErrorInfo(const ExprPointer& expr, size_t max)
{
    addInfo(
        Info::ERROR, expr->pos(),
        QString("expect value of expression between 0x0 and 0x%1, but: 0x%2")
            .arg(QString::number(max, 16), show_integer_hex(expr->bytes()))
    );
}

void Parser::addValueOnlyBeErrorInfo(const ast::ExprPointer& expr, size_t v)
{
    addInfo(
        Info::ERROR, expr->pos(), QString("this value only be %1").arg(v)
    );
}

void Parser::addExprUnableToEvaluateErrorInfo(const ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->pos(),
        "this expression unable to evaluate"
    );
}

void Parser::addExpectCommaCountErrorInfo(
    const ExprPointer& expr, int expect, int have)
{
    Q_ASSERT(expr->is(Node::COMMA));
    addInfo(
        Info::ERROR, expr->pos(),
        QString("Too many arguments, expected %1, have %2")
            .arg(expect).arg(have)
    );
}

bool Parser::expectExprAbleToEvaluate(const ExprPointer& expr)
{
    bool unable = expr->unitDataSize() == 0;
    if (unable)
    {
        int row = expr->pos().row();
        int col = expr->pos().column();
        int end = expr->pos().endColumn();
        if (!haveInfo(Info::ERROR, {row, col, 0}, {row, end, 0}))
        { // 通过 haveInfo 屏蔽掉此错误的传递性
            addExprUnableToEvaluateErrorInfo(expr);
        }
    }
    return !unable;
}

void Parser::addStmtCannotBeExprErrorInfo()
{
    const auto& token = currToken();
    addInfo(
        Info::ERROR, token.pos(),
        QString("this is a statement: '%1', and cannot be expression")
            .arg(token.content())
    );
}

void Parser::addNoPrefixParseFnErrorInfo()
{
    const auto& token = currToken();
    if (sm_infix_parseFns.contains(token.type()))
    {
        addInfo(
            Info::ERROR, token.pos(),
            QString("this token cannot be profix '%1'").arg(token.content())
        );
    }
    else
    {
        addNotYetSupportErrorInfo();
    }
}

void Parser::addReservedWordWarningInfo()
{
    const auto& token = currToken();
    addInfo(
        Info::WARNING, token.pos(),
        QString("this is reserved word, unable to process '%1'")
            .arg(token.content())
    );
}

void Parser::addNotYetSupportErrorInfo()
{
    const auto& token = currToken();
    addInfo(
        Info::ERROR, token.pos(),
        QString("Not yet support parsing the token '%1'")
            .arg(token.content())
    );
}

void Parser::addCSCannotBeModifiedErrorInfo(const ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->pos(),
        "CS cannot be modified directly (use far JMP or CALL)"
    );
}

void Parser::addIPCannotBeModifiedErrorInfo(const ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->pos(),
        "IP cannot be modified directly (use JMP or CALL)"
    );
}

void Parser::addSRegCannotBeModifiedErrorInfo(const ast::ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->pos(),
        QString("%1 cannot be modified by immediate value")
            .arg((*(expr->token())).toUpper())
    );
}

void Parser::addCannotGetValueFromIPErrorInfo(const ast::ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->pos(),
        "Cannot get value of IP register (use POP after CALL)"
    );
}

void Parser::addSRegTogeterErrorInfo(const ast::ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->pos(), "segment registers cannot go together"
    );
}

void Parser::addRegDoNotMatchErrorInfo(const ast::ExprPointer& expr)
{
    addInfo(
        Info::ERROR, expr->pos(), "the two registers do not match in size"
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
