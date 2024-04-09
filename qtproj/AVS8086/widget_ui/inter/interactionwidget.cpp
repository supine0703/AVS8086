#include "interactionwidget.h"
#include "ui_interactionwidget.h"

#include "emulatorwidget.h"

InteractionWidget::InteractionWidget(emulatorwidget* emu, QWidget* parent)
    : FramelessWidget("inter", parent, Func::Close)
    , ui(new Ui::InteractionWidget)
    , inputEdit(new CodeEditor(this))
    , breaksEdit(new CodeEditor(this))
    , breaksEditLight(new Highlighter(breaksEdit->document()))
    , inputEditLight(new Highlighter(inputEdit->document()))
{
    QWidget* main_widget = FramelessWidget::getMain_Widget();
    ui->setupUi(main_widget);
    main_widget->layout()->addWidget(ui->Interaction_Widget);

    breaksEdit->setReadOnly(true);

    ui->splitter->insertWidget(0, breaksEdit);
    ui->splitter->insertWidget(1, inputEdit);

    ui->inputEdit->deleteLater();
    ui->breaksEdit->deleteLater();

    m_emu = emu;

    connect(ui->clearButton, &QPushButton::clicked, this, [this]() {
        m_emu->clearInc();
    });

    connect(ui->updateBreakButton, &QPushButton::clicked, this, [this]() {
        m_emu->setBreaks(breaksEdit->breakLine());
    });

    connect(ui->updateCodeButton, &QPushButton::clicked, this, [this]() {
        m_emu->addSingle("\xc7\x06\x02\x10\x65\x87");
    });

}

InteractionWidget::~InteractionWidget()
{
    delete ui;
}

void InteractionWidget::mouseMoveEvent(QMouseEvent* event)
{
    event->accept();
    FramelessWidget::mouseMoveEvent(event);
}

void InteractionWidget::mousePressEvent(QMouseEvent* event)
{
    event->accept();
    FramelessWidget::mousePressEvent(event);
}

