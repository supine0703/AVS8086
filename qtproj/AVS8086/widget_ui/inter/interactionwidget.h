#ifndef INTERACTIONWIDGET_H
#define INTERACTIONWIDGET_H

#include <QWidget>
#include "FramelessWidget.h"
#include "codeeditor.h"
#include "highlighter.h"
class emulatorwidget;

namespace Ui {
class InteractionWidget;
}

class InteractionWidget : public FramelessWidget
{
    Q_OBJECT

public:
    explicit InteractionWidget(emulatorwidget* emu, QWidget* parent = nullptr);
    ~InteractionWidget();

public:
    Ui::InteractionWidget* ui;

    emulatorwidget* m_emu;

    CodeEditor* inputEdit;
    CodeEditor* breaksEdit;

    Highlighter* inputEditLight;
    Highlighter* breaksEditLight;

    QList<int> m_bks;

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // INTERACTIONWIDGET_H
