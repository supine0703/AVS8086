#ifndef PARSER_H
#define PARSER_H

#include "lexer/lexer.h"
#include "ast/nodes/program.h"

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
        PREFIX,         // -x +x ~x
        ADDRESS,        // []
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
    void addWarningInfo(int row, int column, int len, const QString& info);
    void addErrorInfo(int row, int column, int len, const QString& info);

    void addExpectPeekTokenErrorInfo(const QList<token::Token::Type>& types);
    void addExpectPeekTokenErrorInfo(
        const QList<token::Token::Type>& types, const token::Token& token
    );
    void addExpectExpressionErrorInfo(
        ast::Node::Type type, const ast::ExprPointer& expr
    );
    void addNoPrefixParseFnErrorInfo();
    void addReservedWordErrorInfo();

    // 封装了对需要 token 的操作, 辅助之后的语法分析
    void fristToken();
    void nextToken();
    const token::Token& currToken() const;
    const token::Token& peekToken() const;

    bool expectPeekTokenNot(token::Token::Type type);
    bool expectPeekTokenNot(const QList<token::Token::Type>& types);
    bool expectPeekToken(token::Token::Type type, bool addErr = true);
    bool expectPeekToken(
        const QList<token::Token::Type>& types, bool addErr = true
    );

    Precedence currTokenPrecedence();
    Precedence peekTokenPrecedence();
    static Precedence tokenPrecedence(token::Token::Type type);


private:
    lexer::Lexer* m_lexer;
    QList<token::Token> m_tokens; // 三空间分别为: curr, peek, buff;

    QStringList m_warningInfos;
    QStringList m_errorInfos;

    QMap<QString, QList<int>> m_wellInstructions;

    QString m_segment = "";
    int m_offset = 0;


    static const QHash<token::Token::Type, Precedence> sm_precedences;


private:
    QSharedPointer<ast::Program> parse_program();
    ast::StmtPointer parse_statement();
    ast::StmtPointer parse_expression_statement();
    ast::ExprPointer parse_expression(Precedence precedence = LOWEST);

    // 定义函数原型 - 将所有的 (表达式)语句 分为: 语句, 前缀表达式, 中(后)缀表达式
    typedef ast::StmtPointer (Parser::*stmt_parseFn)(void);
    typedef ast::ExprPointer (Parser::*prefix_parseFn)(void);
    typedef ast::ExprPointer (Parser::*infix_parseFn)(const ast::ExprPointer&);

    // 存储 parse 函数映射表
    static const QHash<token::Token::Type, stmt_parseFn> sm_stmt_parseFns;
    static const QHash<token::Token::Type, prefix_parseFn> sm_prefix_parseFns;
    static const QHash<token::Token::Type, infix_parseFn> sm_infix_parseFns;

    // statement
    ast::StmtPointer parse_well();
    ast::StmtPointer parse_identifier();
    ast::StmtPointer parse_jmp();

    // prefix
    ast::ExprPointer parse_illegal();
    ast::ExprPointer parse_group();

    // // statement
    // ast::StmtPointer parse_single();
    // ast::StmtPointer parse_well();
    // ast::StmtPointer parse_mov();

    // // prefix
    // ast::ExprPointer parse_reserved_word();
    // ast::ExprPointer parse_illegal();
    // ast::ExprPointer parse_not_end();
    // ast::ExprPointer parse_prefix();
    // ast::ExprPointer parse_float();
    // ast::ExprPointer parse_integer();
    // ast::ExprPointer parse_string();
    // ast::ExprPointer parse_group();
    // ast::ExprPointer parse_register();
    // ast::ExprPointer parse_address();

    // // infix
    // ast::ExprPointer parse_infix(const ExprPointer& left);
    // ast::ExprPointer parse_comma(const ExprPointer& left);
    // ast::ExprPointer parse_colon(const ExprPointer& left);

    // // postfix
    // ast::ExprPointer parse_postfix(const ExprPointer& left);
};

} // namespace avs8086::parser

#endif // PARSER_H
