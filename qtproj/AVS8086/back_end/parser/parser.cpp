#include "parser/parser.h"

using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;


Parser::Parser()
    : Parser(nullptr)
{ }

Parser::Parser(Lexer* lexer)
    : m_lexer(lexer)
{ }

Parser::~Parser()
{ }

/* ========================================================================== */

void Parser::setLexer(Lexer* lexer)
{
    m_lexer = lexer;
}

Lexer* Parser::lexer() const
{
    return m_lexer;
}

bool Parser::isError() const
{
    return !m_errorInfos.isEmpty();
}

QStringList Parser::errorInfos() const
{
    return m_errorInfos;
}

QStringList Parser::warningInfos() const
{
    return m_warningInfos;
}

QSharedPointer<avs8086::ast::Program> Parser::newAST()
{
    m_warningInfos.clear();
    m_errorInfos.clear();

    if (m_lexer == nullptr)
        return nullptr;

    fristToken();
    auto p = parse_program();

    m_tokens.clear();
    m_wellInstructions.clear();
    return p;
}

/* ========================================================================== */

void Parser::addWarningInfo(int row, int column, int len, const QString& info)
{
    m_warningInfos.append(
        QString("[%1:(%3,%4,%5)]>%2")
            .arg(m_lexer->fileName(), info).arg(row).arg(column).arg(len)
    );
}

void Parser::addErrorInfo(int row, int column, int len, const QString& info)
{
    m_errorInfos.append(
        QString("[%1:(%3,%4,%5)]>%2")
            .arg(m_lexer->fileName(), info).arg(row).arg(column).arg(len)
    );
}

void Parser::addExpectPeekTokenErrorInfo(const QList<token::Token::Type>& types)
{
    addExpectPeekTokenErrorInfo(types, peekToken());
}

void Parser::addExpectPeekTokenErrorInfo(
    const QList<Token::Type>& types,  const token::Token& token)
{
    if (types.empty())
        return;
    QString expect = Token::typeName(types.at(0));
    for (int i = 1, end = types.length(); i < end; i++)
        expect += QString(",%1").arg(Token::typeName(types.at(i)));

    QString but;
    if (token.is(Token::TOKEN_EOF))
        but = "but line break";
    else
    {
        but = QString("got '%2: %3' instead")
                  .arg(token.typeName(), token.literal());
    }
    addErrorInfo(
        token.row(), token.column(), token.length(),
        QString("expected this token to be '%1', %2").arg(expect, but)
    );
}

void Parser::addExpectExpressionErrorInfo(
    ast::Node::Type type, const ast::ExprPointer& expr)
{

}

void Parser::addNoPrefixParseFnErrorInfo()
{
    const auto& token = currToken();
    addErrorInfo(
        token.row(), token.column(), token.length(),
        QString("no prefix parse function for '%1': '%2'")
            .arg(token.typeName(), token.literal())
    );
}

void Parser::addReservedWordErrorInfo()
{
    const auto& token = currToken();
    addErrorInfo(
        token.row(), token.column(), token.length(),
        QString("this is reserved word, unable to process '%1: %2'")
            .arg(token.typeName(), token.literal())
    );
}

/* ========================================================================== */

void Parser::fristToken()
{
    if (m_lexer->isError())
        m_errorInfos.append(m_lexer->errorInfos());

    m_tokens.append(m_lexer->first());
    while (currToken().is(Token::ANNOTATION))
    {
        m_tokens.pop_back();
        m_tokens.append(m_lexer->next());
    }
    m_tokens.append(m_tokens.at(0));
    m_tokens.append(m_lexer->next());
    nextToken();
}

void Parser::nextToken()
{
    // 缓冲区去除注释
    while (m_tokens.at(2).is(Token::ANNOTATION))
    {
        m_tokens.pop_back();
        m_tokens.append(m_lexer->next());
    }

    m_tokens.pop_front();
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
}

const Token& Parser::currToken() const
{
    return m_tokens.at(0);
}

const Token& Parser::peekToken() const
{
    return m_tokens.at(1);
}

bool Parser::expectPeekTokenNot(token::Token::Type type)
{
    if (peekToken().is(type))
        return false;
    nextToken();
    return true;
}

bool Parser::expectPeekTokenNot(const QList<token::Token::Type>& types)
{
    for (auto type : types)
        if (peekToken().is(type))
            return false;
    nextToken();
    return true;
}

bool Parser::expectPeekToken(Token::Type type, bool addErr)
{
    if (!peekToken().is(type))
    {
        if (addErr)
            addExpectPeekTokenErrorInfo({type});
        return false;
    }
    nextToken();
    return true;
}

bool Parser::expectPeekToken(const QList<Token::Type>& types, bool addErr)
{
    for (auto type : types)
        if (expectPeekToken(type, false))
            return true;
    if (addErr)
        addExpectPeekTokenErrorInfo(types);
    return false;
}

/* ========================================================================== */

Parser::Precedence Parser::currTokenPrecedence()
{
    return tokenPrecedence(currToken().type());
}

Parser::Precedence Parser::peekTokenPrecedence()
{
    return tokenPrecedence(peekToken().type());
}

Parser::Precedence Parser::tokenPrecedence(Token::Type type)
{
    auto it = sm_precedences.find(type);
    return it != sm_precedences.end() ? it.value() : LOWEST;
}

