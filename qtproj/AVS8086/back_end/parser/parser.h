#ifndef PARSER_H
#define PARSER_H

#include "lexer/lexer.h"
#include "ast/header.h"
HEADER_H

namespace avs8086::parser {

class Parser
{
public:
    enum Precedence
    {
        LOWEST = 0,
        COMMA,          // ,
        ASSIGN,         // =
        LOGIC,          // &, ^, |
        COMPARE,        // >, >=, <, <=, ==, !=
        SHIFT,          // << >>
        SUM,            // +, -
        PRODUCT,        // *, /, %
        BITNOT,         // ~
        NEGATIVE,       // -x
    };


public:
    Parser();
    Parser(lexer::Lexer* lexer);
    ~Parser();

    void setLexer(lexer::Lexer* lexer);
    lexer::Lexer* lexer() const;

    bool isError() const;
    QStringList errorInfos() const;
    QStringList warningInfos() const;

    QSharedPointer<ast::Program> newAST();

private:
    QSharedPointer<ast::Program> parse_program();
    QSharedPointer<ast::Statement> parse_statement();
    QSharedPointer<ast::ExpressionStatement> parse_expression_statement();
    QSharedPointer<ast::Expression>  parse_expression(Precedence precedence);

    // statement
    QSharedPointer<ast::Statement> parse_mov();

    // prefix
    QSharedPointer<ast::Expression> parse_not_end();
    QSharedPointer<ast::Expression> parse_nagative();
    QSharedPointer<ast::Expression> parse_integer();
    QSharedPointer<ast::Expression> parse_string();
    QSharedPointer<ast::Expression> parse_group();

    // infix
    QSharedPointer<ast::Expression>
    parse_infix(const QSharedPointer<ast::Expression>& left);
    QSharedPointer<ast::Expression>
    parse_comma(const QSharedPointer<ast::Expression>& left);

    // postfix
    QSharedPointer<ast::Expression>
    parse_postfix(const QSharedPointer<ast::Expression>& left);


private:
    void addWarningInfo(int row, int column, int len, const QString& info);
    void addErrorInfo(int row, int column, int len, const QString& info);

    void addExpectPeekTokenErrorInfo(token::Token::Type type);
    void addExpectPeekTokenErrorInfo(const QList<token::Token::Type>& types);
    void addNoPrefixParseFnErrorInfo();

    // 封装了对需要 token 的操作, 辅助之后的语法分析
    void fristToken();
    void nextToken();
    const token::Token& currToken() const;
    const token::Token& peekToken() const;

    bool checkCurrToken(token::Token::Type type) const;
    bool checkPeekToken(token::Token::Type type) const;
    bool expectPeekToken(token::Token::Type type);
    bool expectPeekToken(const QList<token::Token::Type>& types);

    Precedence currTokenPrecedence();
    Precedence peekTokenPrecedence();
    static Precedence tokenPrecedence(token::Token::Type type);



private:
    lexer::Lexer* m_lexer;
    // 三空间分别位: curr, peek, buff; 每个token需要先进入buff再往前走
    QList<token::Token> m_tokens;

    QStringList m_warningInfos;
    QStringList m_errorInfos;

    static const QMap<token::Token::Type, Precedence> sm_precedences;

    // 定义函数原型 - 将所有的 (表达式)语句 分为 前缀 中缀 后缀
    typedef QSharedPointer<ast::Statement> (Parser::*stmt_parseFn)(void);
    typedef QSharedPointer<ast::Expression> (Parser::*prefix_parseFn)(void);
    typedef
        QSharedPointer<ast::Expression>
        (Parser::*infix_parseFn)(const QSharedPointer<ast::Expression>&);
    typedef
        QSharedPointer<ast::Expression>
        (Parser::*postfix_parseFn)(const QSharedPointer<ast::Expression>&);

    static const QMap<token::Token::Type, stmt_parseFn> sm_statement_parseFns;
    static const QMap<token::Token::Type, prefix_parseFn> sm_prefix_parseFns;
    static const QMap<token::Token::Type, infix_parseFn> sm_infix_parseFns;
    static const QMap<token::Token::Type, postfix_parseFn> sm_postfix_parseFns;
};

} // namespace avs8086::parser

#endif // PARSER_H
