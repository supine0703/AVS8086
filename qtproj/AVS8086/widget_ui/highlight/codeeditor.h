#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QSet>
#include <QTimer>
#include <QTreeWidgetItem>
// #include "debugwidget.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;//用于显示行号

//![codeeditordefinition]

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);//绘制行号区域的函数
    int lineNumberAreaWidth();//行号宽度
    QWidget* getTextWidget();//获取文本窗口

    void getWarnLineList(QStringList* tmp);
    void getErrLineList(QStringList* tmp);

    void goToErrLine(int errLine, int errWord, int errWordLengh);

    QStringList errLineList;
    QStringList warnLineList;
    QStringList errLineStringList;
    QStringList warnLineStringList;

    QString get() const;

signals:
    void ssssss();

private :
    enum UColor{//下划线的颜色 暂时就两个
        RED,
        YELLOW
    };

protected:
    void resizeEvent(QResizeEvent *event) override;//处理CodeEditor的大小变化的事件

public slots:
    void updateLineNumberAreaWidth(int newBlockCount);//更新行号宽度
    void highlightCurrentLine();//高亮显示
    void updateLineNumberArea(const QRect &rect, int dy);//
    void findLineNumber(int lineNumer);//查找当前点击的行号
    void updateUnderlines();
    bool openFileSignalsSlot(QString path);
    bool saveFileSlot(QString path);
    void showTextSignalSlot(QString path,QString name,QTreeWidgetItem *item);
    void setCompileFile(QString path);

private:
    LineNumberArea *lineNumberArea;//一个用于显示行号的指针
    int lineClickNumber;

    QSet<int> breakPoints;//存储断点行号

    void updatepoints();//使用QTimer定时扫描
    void reset(int line);
    void resetPoints();//清除所有的下划线样式

    QVector<QTextLayout::FormatRange> formats;

    void setUnderLine(UColor color, int line, int column, int length);

    void setTimer();

    // QStringList errLineList;
    // QStringList warnLineList;
    QStringList resetErrLineList;
    QStringList resetWarnLineList;

    bool isLineListChanged;

    QTimer* timer;


    QString comFilePath;

};


class LineNumberArea : public QWidget
{
    Q_OBJECT

public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor)
        , codeEditor(editor)
    {}

    QSize sizeHint() const override//返回LineNumberArea的建议大小
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }


protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;

signals:
    void clickLineArea(int lineNumber);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif
