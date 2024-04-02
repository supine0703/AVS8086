#include "token.h"
#include <QRegularExpression>

Token::Token()
    : Token(TOKEN_ILLEGAL, "", 0, 0)
{ }

Token::Token(Type type, const QString& literal, int row, int column)
    : m_type(type), m_literal(literal), m_row(row), m_column(column)
{ }

Token::Token(const Token& other)
    : m_type(other.m_type)
    , m_literal(other.m_literal)
    , m_row(other.m_row)
    , m_column(other.m_column)
{ }

Token::~Token()
{ }

Token& Token::operator=(const Token& other)
{
    if (this != &other)
    {
        this->m_type = other.m_type;
        this->m_literal = other.m_literal;
        this->m_row = other.m_row;
        this->m_column = other.m_column;
    }
    return *this;
}

/* ========================================================================== */

Token::Type Token::type() const
{
    return m_type;
}

QString Token::literal() const
{
    return m_literal;
}

int Token::row() const
{
    return m_row;
}

int Token::column() const
{
    return m_column;
}

void Token::resetType(Type type)
{
    m_type = type;
}

QString Token::typeName() const
{
    return tokenTypeName(m_type);
}

/* ========================================================================== */

QString Token::tokenTypeName(Type type)
{
    return sm_typeNames.contains(type) ? sm_typeNames.value(type) : "";
}

Token::Type Token::tokenType(char literal)
{
    switch (literal)
    {
    case 0:
        return TOKEN_EOF;
    case ';':
        return TOKEN_ANNOTATION;
    case '#':
        return TOKEN_WELL;
    case ':':
        return TOKEN_COLON;
    case ',':
        return TOKEN_COMMA;
    case '+':
        return TOKEN_PLUS;
    case '-':
        return TOKEN_MINUS;
    case '*':
        return TOKEN_ASTERISK;
    case '/':
        return TOKEN_SLASH;
    case '=':
        return TOKEN_EQUAL;
    case '(':
        return TOKEN_LPAREN;
    case ')':
        return TOKEN_RPAREN;
    case '[':
        return TOKEN_LSQUARE;
    case ']':
        return TOKEN_RSQUARE;
    default:
        return TOKEN_ILLEGAL;
    }
}

Token::Type Token::tokenType(const QString& literal)
{
    static const QRegularExpression regex(
        "^(0X)?([0-9A-F]+?)([HDOB])?$",
        QRegularExpression::CaseInsensitiveOption
    );
    auto up(literal.toUpper());
    auto head = up.at(0);
    auto tail = up.at(up.length() - 1);
    if (up.isEmpty()) {
        return TOKEN_EOF;
    } else if (head == ';') {
        return TOKEN_ANNOTATION;
    } else if ((head == tail) && (head == '\'' || head == '"'))  {
        return TOKEN_STRING;
    } else if (regex.match(up).hasMatch()) {
        return TOKEN_INTEGER;
    } else if (sm_makeNames.contains(up)) {
        return TOKEN_MAKE_;
    } else if (sm_loadNames.contains(up)) {
        return TOKEN_LOAD_;
    } else if (sm_reg8Names.contains(up)) {
        return TOKEN_REG8;
    } else if (sm_reg16Names.contains(up)) {
        return TOKEN_REG16;
    } else if (sm_mnemonicNames.contains(up)) {
        return TOKEN_MNEMONIC;
    } else if (sm_indicateNames.contains(up)) {
        return TOKEN_INDICATE;
    } else {
        return tokenType(up.at(0).toLatin1());
    }
}

/* ========================================================================== */

const QMap<Token::Type, QString> Token::sm_typeNames = {
    { TOKEN_ILLEGAL,        "Illegal" },
    { TOKEN_EOF,            "EOF" },
    { TOKEN_PROGRAM,        "Program" },
    { TOKEN_REG8,           "Register8Bit" },
    { TOKEN_REG16,          "Register16Bit" },
    { TOKEN_LABEL,          "Label"},
    { TOKEN_STRING,         "String"},
    { TOKEN_INTEGER,        "Integer" },
    { TOKEN_MNEMONIC,       "Mnemonic" },
    { TOKEN_INDICATE,       "Indicate" },
    { TOKEN_ANNOTATION,     "Annotation" },
    { TOKEN_LPAREN,         "(" },
    { TOKEN_RPAREN,         ")" },
    { TOKEN_LSQUARE,        "[" },
    { TOKEN_RSQUARE,        "]" },
    { TOKEN_WELL,           "#" },
    { TOKEN_COLON,          ":" },
    { TOKEN_COMMA,          "," },
    { TOKEN_PLUS,           "+" },
    { TOKEN_MINUS,          "-" },
    { TOKEN_ASTERISK,       "*" },
    { TOKEN_SLASH,          "/" },
    { TOKEN_EQUAL,          "=" },
    { TOKEN_MAKE_,          "MAKE_" },
    { TOKEN_LOAD_,          "LOAD_" },
};

const QStringList Token::sm_makeNames = {
    "MAKE_BIN", "MAKE_COM", "MAKE_EXE",
};

const QStringList Token::sm_loadNames = {
    "LOAD_SEGMENT", "LOAD_OFFSET",
};

const QStringList Token::sm_reg8Names = {
    "AL", "AH", "BL", "BH", "CL", "CH", "DL", "DH",
};

const QStringList Token::sm_reg16Names = {
    "AX", "BX", "CX", "DX",
    "SP", "BP", "SI", "DI",
    "CS", "DS", "SS", "ES", "IP",
};

const QStringList Token::sm_mnemonicNames = {
    "AAA" , "AAD" , "AAM" , "AAS" , "ADC" , "ADD" , "AND" ,
    "CALL", "CBW" , "CLC" , "CLD" , "CLI" , "CMC" , "CMP" , "CMPS", "CWD" ,
    "DAA" , "DAS" , "DEC" , "DIV" ,
    "ESC" ,
    "HLT" ,
    "IDIV", "IMUL", "IN"  , "INC" , "INT" , "INTO", "IRET",
    "JA"  , "JNBE", "JAE" , "JNC" , "JNB" , "JB"  , "JC"  , "JNAE", "JBE" ,
    "JNA" , "JCXZ", "JE"  , "JZ"  , "JG"  , "JNLE", "JGE" , "JNL" , "JL"  ,
    "JNGE", "JLE" , "JNG" , "JMP" , "JNE" , "JNZ" , "JNO" , "JNP" , "JPO" ,
    "JNS" , "JO"  , "JP"  , "JPE" , "JS"  ,
    "LAHF", "LDS" , "LEA" , "LES" , "LOCK", "LODS", "LOOP","LOOPE","LOOPZ",
    "LOOPNE", "LOOPNZ",
    "MOV" , "MOVS","MOVSB", "MUL" , "NEG" , "NOP" , "NOT" , "OR"  , "OUT" ,
    "POP" , "POPF", "PUSH",
    "RCL" , "RCR" , "RET" , "REP" , "ROL" , "ROR" , "SAHF", "SAL" , "SBB" ,
    "SHL" , "SAR" , "SCAS", "STC" , "STD" , "STI" , "STOS", "SUB" ,
    "TEST",
    "XCHG", "XLAT", "XOR" , "WAIT",
    // TODO: ...
};

const QStringList Token::sm_indicateNames = {
    "DD", "DW", "DD", "DQ", "DT", "EQU", "=", "LABEL", "SEGMENT", "ENDS",
    "ASSUME", "ORG", "NOTHING", "PROC", "NEAR", "FAR", "RET", "ENDP",
    "MACRO", "ENDM",
    // TODO: ...
};

