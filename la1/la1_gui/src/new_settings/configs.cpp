/**
 *  AVS8086 - See <https://github.com/supine0703/AVS8086> to know more.
 *
 *  Copyright (C) <2024>  <李宗霖>  github <https://github.com/supine0703>.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "configs.h"

using namespace la1::gui;


void Configs::setValue(QAnyStringView key, const QVariant& value)
{
    instance().m_settings.setValue(key, value);
    emit instance().configChanged(key, value);
}

QVariant Configs::value(QAnyStringView key)
{
    return instance().m_settings.value(key);
}

Configs& Configs::instance()
{
    static Configs conf;
    return conf;
}
