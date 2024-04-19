#include "parser/parser.h"
#include "ast/nodes/well.hpp"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Statement> Parser::parse_well()
{
    int row = currToken().row();
    int col = currToken().column();

    if (peekToken().is(Token::TOKEN_EOF))
    {
        addErrorInfo(row, col, 1, "'#' is not close on this line");
        return nullptr;
    }

    nextToken();

    QList<Token> tokens;
    
    while (!currToken().is(Token::TOKEN_WELL)
           && !peekToken().is(Token::TOKEN_EOF))
    {
        tokens.append(currToken());
        nextToken();
    }
    
    if (!currToken().is(Token::TOKEN_WELL))
    {
        addErrorInfo(row, col, 1, "'#' is not close on this line");
        return nullptr;
    }

    if (tokens.isEmpty())
    {
        addWarningInfo(row, col, 2, "empty in the '#'");
        return nullptr;
    }

    QSharedPointer<Well> s(new Well(tokens));

    const auto& t(tokens.last());
    int len = t.column() + t.literal().length() - col + 1;

    if (s->isError())
    {
        addErrorInfo(row, col - 1, len, "illegal '#' instruction: " + s->key());
    }
    else
    {
        // 检测取值是否合理(警告)
        if (s->valueType() == Token::TOKEN_REGISTER)
        {
            auto index = Token::findRegisters(s->key());

            auto value = s->value().toUInt();
            int max = index < 8 ? 0xff : 0xffff;
            if (!(0 <= value && value <= max))
            {
                addWarningInfo(
                    row, tokens.at(2).column(), tokens.at(2).literal().length(),
                    QString("integer should between 0H and %2H: %1H")
                        .arg(value, 0, 16).arg(max, 0, 16)
                );
            }
        }
        // 检测是否有重复(警告)
        auto key = s->key();
        auto count = m_wellInstructions.count(key);
        if (count)
        {
            if (count == 1)
            {
                int i = m_wellInstructions.indexOf(key);
                auto infos(m_wellInfos.at(i).split(' '));
                addWarningInfo(
                    infos.at(0).toInt(),
                    infos.at(1).toInt(),
                    infos.at(2).toInt(),
                    "repeated instructions: " + key + s->op()
                );
            }
            addWarningInfo(
                row, col, len,
                "repeated instructions: " + key +  s->op()
            );
        }
        m_wellInstructions.append(s->key());
        m_wellInfos.append(QString("%1 %2 %3").arg(row).arg(col).arg(len));
    }
    return s;
}
