#ifndef PARSER_H
#define PARSER_H

#include "lexer/lexer.h"
#include "ast/nodeheader.h"
#include <QSharedPointer>

NODEHEADER_H

class Program;
class ExpressionStatement;

class Parser
{
public:
    enum Precedence
    {
        LOWEST = 0,
        SUM,            // +, -
        PRODUCT,        // *, /
    };


public:
    Parser();
    Parser(Lexer* lexer);
    ~Parser();

    void clear();

    void setLexer(Lexer* lexer);
    Lexer* lexer() const;

    bool isError() const;
    QStringList errorInfos() const;

    QSharedPointer<Program> parse_program();


private:
    void addErrorInfo(int row, int column, const QString& info);

    // 封装了对需要 token 的操作, 辅助之后的语法分析
    void fristToken();
    void nextToken();
    const Token& currToken() const;
    const Token& peekToken() const;

    bool checkCurrToken(Token::Type type) const;
    bool ckeckPeekToken(Token::Type type) const;
    bool expectPeekToken(Token::Type type);
    bool expectPeekToken(const QList<Token::Type>& types);
    void addExpectPeekTokenErrorInfo(Token::Type type);
    void addExpectPeekTokenErrorInfo(const QList<Token::Type>& types);

    Precedence currTokenPrecedence();
    Precedence peekTokenPrecedence();
    static Precedence tokenPrecedence(Token::Type type);

    // 定义函数原型 - 将所有的 (表达式)语句 分为 前缀 中缀 后缀
    typedef QSharedPointer<Expression> (Parser::*prefix_parseFn)(void);

    typedef QSharedPointer<Expression>
    (Parser::*infix_parseFn)(const QSharedPointer<Expression>&);

    typedef QSharedPointer<Expression>
    (Parser::*postfix_parseFn)(const QSharedPointer<Expression>&);

    void addNoPrefixParseFnErrorInfo();


    QSharedPointer<Statement> parse_statement();
    QSharedPointer<ExpressionStatement> parse_expression_statement();

    QSharedPointer<Expression>  parse_expression(Precedence precedence);

    // prefix
    QSharedPointer<Expression> parse_integer();
    QSharedPointer<Expression> parse_group();

    // infix
    QSharedPointer<Expression>
    parse_infix(const QSharedPointer<Expression>& left);

    // postfix
    QSharedPointer<Expression>
    parse_postfix(const QSharedPointer<Expression>& left);


private:
    Lexer* m_lexer;
    QList<Token> m_tokens;

    QStringList m_errorInfos;

    static const QMap<Token::Type, Precedence> sm_precedences;
    static const QMap<Token::Type, prefix_parseFn> sm_prefix_parseFns;
    static const QMap<Token::Type, infix_parseFn> sm_infix_parseFns;
    static const QMap<Token::Type, postfix_parseFn> sm_postfix_parseFns;
};

#endif // PARSER_H
