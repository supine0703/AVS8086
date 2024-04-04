#include "parser/parser.h"

using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;


Parser::Parser()
    : Parser(nullptr)
{ }

Parser::Parser(Lexer* lexer)
{
    setLexer(lexer);
}

Parser::~Parser()
{ }

/* ========================================================================== */

void Parser::setLexer(Lexer* lexer)
{
    m_lexer = lexer;
    if (m_lexer != nullptr)
        fristToken();
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
    setLexer(m_lexer);
    return parse_program();
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

void Parser::addExpectPeekTokenErrorInfo(Token::Type type)
{
    addExpectPeekTokenErrorInfo(QList({type}));
}

void Parser::addExpectPeekTokenErrorInfo(const QList<Token::Type>& types)
{
    if (types.empty())
        return;
    QString expectType = Token::tokenTypeName(types.at(0));
    for (int i = 1, end = types.length(); i < end; i++)
        expectType += QString(",%1").arg(Token::tokenTypeName(types.at(i)));

    const auto& token = peekToken();
    addErrorInfo(
        token.row(), token.column(), token.literal().length(),
        QString("expected next token to be '%1', got '%2' instead")
            .arg(expectType, token.typeName())
    );
}

void Parser::addNoPrefixParseFnErrorInfo()
{
    const auto& token = currToken();
    addErrorInfo(
        token.row(), token.column(), token.literal().length(),
        "no prefix parse function for " + token.typeName()
        );
}

/* ========================================================================== */

void Parser::fristToken()
{
    m_errorInfos.clear();
    if (m_lexer->isError())
        m_errorInfos.append(m_lexer->errorInfos());

    auto first = m_lexer->first();
    m_tokens.clear();
    m_tokens.append(first);
    m_tokens.append(first);
    m_tokens.append(m_lexer->next());
    nextToken();
}

void Parser::nextToken()
{
    while (m_tokens.at(2).type() == Token::TOKEN_ANNOTATION)
    {
        m_tokens.pop_back();
        m_tokens.append(m_lexer->next());
    }

    m_tokens.pop_front();
    int currRow = currToken().row();
    int nextRow = peekToken().row();

    if (currRow < nextRow)
    { // 行结束
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

bool Parser::checkCurrToken(Token::Type type) const
{
    return currToken().type() == type;
}

bool Parser::checkPeekToken(Token::Type type) const
{
    return peekToken().type() == type;
}

bool Parser::expectPeekToken(Token::Type type)
{
    if (!checkPeekToken(type))
        return false;
    nextToken();
    return true;
}

bool Parser::expectPeekToken(const QList<Token::Type>& types)
{
    for (auto type : types)
        if (expectPeekToken(type))
            return true;
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

