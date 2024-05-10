#include "infos.h"

using namespace avs8086;

/* ========================================================================== */

QString Info::typeName(Type type)
{
    auto it = sm_typeNames.find(type);
    if (it != sm_typeNames.end())
        return it.value();
    return sm_informationName;
}

/* ========================================================================== */

const QString Info::sm_informationName = "Information";

const QHash<Info::Type, QString> Info::sm_typeNames = {
    { SUGGEST,          "Suggest" },
    { WARNING,          "Warning" },
    { ERROR,            "Error" },
};
