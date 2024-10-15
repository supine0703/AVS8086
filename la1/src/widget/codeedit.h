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


#ifndef CODE_EDIT_H
#define CODE_EDIT_H

#include <QPlainTextEdit>

class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEdit(QWidget* parent = nullptr);
    ~CodeEdit();


    // QTC_TEMP
    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
};

#endif // CODE_EDIT_H
