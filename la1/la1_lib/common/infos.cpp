#include "infos.h"

using namespace lzl::a1;

/* ========================================================================== */

QString Info::typeName(Type type)
{
    auto it = s_typeNames.find(type);
    if (it != s_typeNames.end())
    {
        return it.value();
    }
    return s_informationName;
}

/* ========================================================================== */

const QString Info::s_informationName = "Information";

const QHash<Info::Type, QString> Info::s_typeNames = {
    {SUGGESTION, "Suggestion"},
    {WARNING,    "Warning"   },
    {ERROR,      "Error"     },
};

/* ========================================================================== */
