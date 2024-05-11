#ifndef PARSER_H
#define PARSER_H

#include "lexer/lexer.h"
#include "ast/stmts/program.h"
#include "service/identifier.h"

namespace avs8086::parser {

class Parser
{
public:
    enum Precedence
    {
        LOWEST = 0,
        COMMA,          // ,
        ASSIGN,         // DUP =
        LOGIC,          // &, ^, |
        COMPARE,        // >, >=, <, <=, ==, !=
        SHIFT,          // << >>
        SUM,            // +, -
        PRODUCT,        // *, /, %
        PREFIX,         // -x +x ~x
        ADDRESS,        // []
    };

public:
    Parser() : Parser(nullptr) { }

    Parser(lexer::Lexer* lexer) : m_lexer(lexer) { m_tokens.reserve(4); }

    ~Parser() = default;

    void setLexer(lexer::Lexer* lexer) { m_lexer = lexer; }

    lexer::Lexer* lexer() const { return m_lexer; }

    bool isError() const { return m_err; }

    InfoList infos() const { return m_infos; }

    void clear();

    QSharedPointer<ast::Program> newAST();


private:
    /**
     * 关于 token 的操作, 辅助之后的语法分析
    */
    void fristToken();
    void nextToken();
    const token::Token& currToken() const { return m_tokens.at(0); }
    const token::Token& peekToken() const { return m_tokens.at(1); }

    bool expectPeekToken(bool is, token::Token::Type type);
    bool expectPeekToken(bool is, const QList<token::Token::Type>& types);
#if 0
    bool expectExpression(ast::Node::Type type, const ast::ExprPointer& expr);
    bool expectExpression(
        const ast::QList<Node::Type>& types, const ast::ExprPointer& expr
    );
#endif

    Precedence currPrecedence() const { return precedence(currToken().type()); }
    Precedence peekPrecedence() const { return precedence(peekToken().type()); }
    static Precedence precedence(token::Token::Type type);

private:
    lexer::Lexer* m_lexer;
    token::TokenList m_tokens; // 3 空间分别为: curr, peek, buff;
    static const QHash<token::Token::Type, Precedence> sm_precedences;


private:
    /**
     * 关于解析标识符部分
    */
    void defineId(const ast::StmtPointer& stmt);
    void callId(const ast::StmtPointer& stmt);
    void parse_idTable();
    bool parse_idTable(IdTable& t, int begin, int end, int offset, int max);

private:
    int m_currUnitSize = 0; // 和 allocate 相关

    QList<int> m_offsers;
    QHash<QString, int> m_idIts;
    QHash<int, ast::StmtPointer> m_ids;
    QHash<int, ast::StmtPointer> m_calls;
    QList<QPair<int, ast::StmtPointer>> m_lastCalls;
    IdTable m_idTable;
    int m_currOffset = 0;
    QString m_currSeg = "";
    static const QHash<token::Token::Type, Identifier::Type> sm_idTypes;

private:
    /**
     * 关于信息部分的操作
    */
    void addInfo(Info::Type type, const Position& pos, const QString& info);

    void addExpectTokenErrorInfo(
        const token::Token& token, const QList<token::Token::Type>& types = {}
    ); // 如果为空则表示不应是本身的类型

    void addExpectExprErrorInfo(
        const ast::ExprPointer& expr, const QList<ast::Node::Type>& types = {}
    );

    void addExprDivideZeroErrorInfo(const ast::ExprPointer& expr);
    void addExprCanNotBeUsedAsIntegerErrorInfo(const ast::ExprPointer& expr);
    void addExprVOverflowErrorInfo(const ast::ExprPointer& expr, size_t max);
    void addExprUnableToEvaluateErrorInfo(const ast::ExprPointer& expr);
    bool expectExprAbleToEvaluate(const ast::ExprPointer& expr);

    void addStmtCanNotBeExprErrorInfo();
    void addNoPrefixParseFnErrorInfo();
    void addReservedWordErrorInfo();
    void addNotYetSupportErrorInfo();

    void addJmpOverflowErrorInfo(const Position& pos, int min, int max);

private:
    bool m_err = false;
    InfoList m_infos;
    QHash<QString, QList<int>> m_wellInstructions;


private:
    /**
     * 定义关于语法分析的操作和关系
    */
    QSharedPointer<ast::Program> parse_program();
    ast::StmtPointer parse_statement();
    ast::StmtPointer parse_expression_statement();
    ast::ExprPointer parse_expression(Precedence precedence = LOWEST);
    // other
    static ast::ExprPointer parse_illegal(const token::Token& token);


    // 定义函数原型 - (表达式)语句 分为: 语句, 后定义语句, 前缀表达式, 中(后)缀表达式
    typedef ast::StmtPointer (Parser::*stmt_parseFn)(void);
    typedef ast::StmtPointer (Parser::*post_parseFn)(void);
    typedef ast::ExprPointer (Parser::*prefix_parseFn)(void);
    typedef ast::ExprPointer (Parser::*infix_parseFn)(const ast::ExprPointer&);

    // 存储 parse 函数映射表
    static const QHash<token::Token::Type, stmt_parseFn> sm_stmt_parseFns;
    static const QHash<token::Token::Type, post_parseFn> sm_post_parseFns;
    static const QHash<token::Token::Type, prefix_parseFn> sm_prefix_parseFns;
    static const QHash<token::Token::Type, infix_parseFn> sm_infix_parseFns;

    // statement
    ast::StmtPointer parse_well();
    ast::StmtPointer parse_allocate();
    ast::StmtPointer parse_mov();
    ast::StmtPointer parse_jmp();
    ast::StmtPointer parse_jx();
    ast::StmtPointer parse_single();

    // post statement
    ast::StmtPointer parse_define();

    // prefix
    template <class T>
    ast::ExprPointer parse_T();
    ast::ExprPointer parse_value();
    ast::ExprPointer parse_operator();
    ast::ExprPointer parse_group();
    ast::ExprPointer parse_address();

    // infix
    template <class T>
    ast::ExprPointer parse_T(const ast::ExprPointer& left);
    ast::ExprPointer parse_operator(const ast::ExprPointer& left);
    ast::ExprPointer parse_dup(const ast::ExprPointer& left);

    // other
    ast::ExprPointer parse_commaArray(const ast::ExprPointer& comma);

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
    // ast::ExprPointer parse_infix(const ast::ExprPointer& left);
    // ast::ExprPointer parse_comma(const ast::ExprPointer& left);
    // ast::ExprPointer parse_colon(const ast::ExprPointer& left);

    // // postfix
    // ast::ExprPointer parse_postfix(const ast::ExprPointer& left);
};

template<class T>
inline ast::ExprPointer Parser::parse_T()
{
    return ast::ExprPointer(new T(currToken()));
}

template<class T>
inline ast::ExprPointer Parser::parse_T(const ast::ExprPointer& left)
{
    auto p = currPrecedence();
    auto t = currToken();
    if (expectPeekToken(false, token::Token::TOKEN_EOL))
        return ast::ExprPointer(new T(t, left, parse_expression(p)));
    auto eol = parse_illegal(peekToken());
    addExpectExprErrorInfo(eol);
    return ast::ExprPointer(new T(t, left, eol));
}

} // namespace avs8086::parser

#endif // PARSER_H
