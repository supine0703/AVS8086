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
    enum TabType {
        TAB_USE_TABLE = 0,
        TAB_USE_SPACE,
    };

    explicit CodeEdit(QWidget* parent = nullptr);

    ~CodeEdit();

    void setEditTabSize(int charWidth, int tabSize = 4);

    void setEditTab(TabType type, int tabSize);

    void setEditTab(TabType type, int tabSize, int charWidth);

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;

private:
    TabType m_tab = TAB_USE_TABLE;
    int m_tabSize = 4;
};

#endif // CODE_EDIT_H
