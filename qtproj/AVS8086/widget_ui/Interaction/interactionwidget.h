#ifndef INTERACTIONWIDGET_H
#define INTERACTIONWIDGET_H

#include <QWidget>
#include "FramelessWidget.h"
#include "emulatorwidget.h"

namespace Ui {
class InteractionWidget;
}

class InteractionWidget : public FramelessWidget
{
    Q_OBJECT

public:
    explicit InteractionWidget(emulatorwidget* emu, QWidget* parent = nullptr);
    ~InteractionWidget();

private:
    Ui::InteractionWidget* ui;

    emulatorwidget* m_emu;

    QList<int> m_bks;
};

#endif // INTERACTIONWIDGET_H
