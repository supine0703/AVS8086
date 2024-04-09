#include "interactionwidget.h"
#include "ui_interactionwidget.h"

InteractionWidget::InteractionWidget(emulatorwidget* emu, QWidget* parent)
    : FramelessWidget(parent, Func::Close)
    , ui(new Ui::InteractionWidget)
{
    QWidget* main_widget = FramelessWidget::getMain_Widget();
    ui->setupUi(main_widget);
    main_widget->layout()->addWidget(ui->Interaction_Widget);

    m_emu = emu;

    connect(ui->clearButton, &QPushButton::clicked, this, [this]() {
        m_emu->clearInc();
    });

    connect(ui->updateBreakButton, &QPushButton::clicked, this, [this]() {
        m_emu->setBreaks(m_bks);
    });

    connect(ui->updateCodeButton, &QPushButton::clicked, this, [this]() {
        m_emu->addSingle("\xc7\x06\x02\x10\x65\x87");
    });

}

InteractionWidget::~InteractionWidget()
{
    delete ui;
}
