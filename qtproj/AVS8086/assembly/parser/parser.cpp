#include "parser/parser.h"


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

void Parser::clear()
{
    m_lexer = nullptr;
    m_tokens.clear();
    m_errorInfos.clear();
}

void Parser::setLexer(Lexer* lexer)
{
    clear();
    m_lexer = lexer;
    if (m_lexer != nullptr)
    {
        if (m_lexer->isError())
            m_errorInfos.append(m_lexer->errorInfos());
        fristToken();
    }
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

/* ========================================================================== */

void Parser::addErrorInfo(int row, int column, const QString& info)
{
    m_errorInfos.append(
        QString("[%1:(%3,%4)]>%2")
            .arg(m_lexer->fileName(), info).arg(row).arg(column)
    );
}

void Parser::fristToken()
{
    m_tokens.append(m_lexer->first());
    m_tokens.append(m_lexer->next());
}

void Parser::nextToken()
{
    m_tokens.pop_front();
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

bool Parser::ckeckPeekToken(Token::Type type) const
{
    return peekToken().type() == type;
}

bool Parser::expectPeekToken(Token::Type type)
{
    if (!ckeckPeekToken(type))
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
        token.row(), token.column(),
        QString("expected next token to be '%1', got '%2' instead")
            .arg(expectType, token.typeName())
    );
}

Parser::Precedence Parser::currTokenPrecedence()
{
    return tokenPrecedence(currToken().type());
}

Parser::Precedence Parser::peekTokenPrecedence()
{
    return tokenPrecedence(peekToken().type());
}

/* ========================================================================== */

Parser::Precedence Parser::tokenPrecedence(Token::Type type)
{
    return sm_precedences.contains(type) ? sm_precedences.value(type) : LOWEST;
}

void Parser::addNoPrefixParseFnErrorInfo()
{
    const auto& token = currToken();
    addErrorInfo(
        token.row(), token.column(),
        "no prefix parse function for " + token.typeName()
    );
}

/* ========================================================================== */

const QMap<Token::Type, Parser::Precedence> Parser::sm_precedences = {
    { Token::TOKEN_PLUS,        SUM },          // +
    { Token::TOKEN_MINUS,       SUM },          // -
    { Token::TOKEN_ASTERISK,    PRODUCT },      // *
    { Token::TOKEN_SLASH,       PRODUCT },      // /
};

const QMap<Token::Type, Parser::prefix_parseFn> Parser::sm_prefix_parseFns = {
    { Token::TOKEN_INTEGER,     &Parser::parse_integer },   // integer
    { Token::TOKEN_LPAREN,     &Parser::parse_group },      // (
};

const QMap<Token::Type, Parser::infix_parseFn> Parser::sm_infix_parseFns = {
    { Token::TOKEN_PLUS,        &Parser::parse_infix },     // +
    { Token::TOKEN_MINUS,       &Parser::parse_infix },     // -
    { Token::TOKEN_ASTERISK,    &Parser::parse_infix },     // *
    { Token::TOKEN_SLASH,       &Parser::parse_infix },     // /
};

const QMap<Token::Type, Parser::postfix_parseFn> Parser::sm_postfix_parseFns = {

};

