//#include "framelesswidget.h"

//#include <QApplication>
//#include <QVBoxLayout>
//#include <QScreen>

//FramelessWidget::FramelessWidget(QWidget* parent)
//    : QWidget(parent)
//    , m_widgetLayout(new QVBoxLayout(this))
//    , m_headWidget(new QWidget(this))
//    , m_headWidgetLayout(new QVBoxLayout(m_headWidget))
//    , m_centralWidget(new QWidget(this))
//    , m_centralWidgetLayout(new QVBoxLayout(m_centralWidget))
//    // , m_statusWidget(new QWidget(this))
//    // , m_statusWidgetLayout(new QVBoxLayout(m_statusWidget))
//{
//    m_widgetLayout->setObjectName("FramelessQWidgetLayout");
//    m_headWidget->setObjectName("FramelessHeadQWidget");
//    m_headWidgetLayout->setObjectName("FramelessHeadQWidgetLayout");
//    m_centralWidget->setObjectName("FramelessCentralQWidget");
//    m_centralWidgetLayout->setObjectName("FramelessCentralQWidgetLayout");
//    // m_centralWidget->setObjectName("FramelessStatusQWidget");
//    // m_centralWidgetLayout->setObjectName("FramelessStatusQWidgetLayout");

//    m_widgetLayout->setSpacing(0);
//    m_widgetLayout->setContentsMargins(0, 0, 0, 0);
//    m_headWidgetLayout->setSpacing(0);
//    m_headWidgetLayout->setContentsMargins(1, 1, 1, 0);
//    m_centralWidgetLayout->setSpacing(0);
//    m_centralWidgetLayout->setContentsMargins(1, 0, 1, 1);

//    m_headWidget->setStyleSheet("border-bottom: 1px solid gray;");
//    // m_centralWidget->setStyleSheet("border-bottom: 1px solid gray;");

//    m_widgetLayout->addWidget(m_headWidget);
//    m_widgetLayout->addWidget(m_centralWidget);
//    // m_widgetLayout->addWidget(m_statusWidget);

//    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->resize(600, 400);
//    setHeadFixHeight(30);
//    adjustDisplay();
//}

//FramelessWidget::~FramelessWidget()
//{
//}

//QWidget* FramelessWidget::centralWidget()
//{
//    return m_the_body;
//}

//void FramelessWidget::setCentralWidget(QWidget* w)
//{
//    if (m_the_body != nullptr)
//    {
//        m_centralWidgetLayout->removeWidget(m_the_body);
//        m_the_body->deleteLater();
//    }

//    if (w == nullptr)
//    {
//        m_the_body = nullptr;
//        return;
//    }
//    m_the_body = w;
//    m_centralWidgetLayout->addWidget(m_the_body);
//}

//void FramelessWidget::setHeadWidget(QWidget* w)
//{
//    if (m_the_head != nullptr)
//    {
//        m_headWidgetLayout->removeWidget(m_the_head);
//        m_the_head->deleteLater();
//    }

//    bool hidden = (w == nullptr);
//    m_headWidget->setHidden(hidden);
//    if (hidden)
//    {
//        m_the_head = nullptr;
//        return;
//    }
//    m_the_head = w;
//    m_headWidgetLayout->addWidget(m_the_head);
//}

//void FramelessWidget::setHeadFixHeight(int height)
//{
//    m_headWidget->setFixedHeight(height);
//}

//int FramelessWidget::headFixHeight() const
//{
//    return m_headWidget->height();
//}

//void FramelessWidget::adjustDisplay()
//{
//    int sW = QApplication::primaryScreen()->size().width();
//    int sH = QApplication::primaryScreen()->size().height();
//    int w = this->width();
//    int h = this->height();
//    if (sW < w || sH < h)
//        this->move(1, 1);
//    else
//        this->move(((sW - w) >> 1), ((sH - h) * 0.4));
//}

