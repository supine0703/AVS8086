#include "codeeditor.h"

#include <QPainter>
#include <QTextBlock>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>

#include "lexer/lexer.h"
#include "parser/parser.h"

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
    , isLineListChanged(false)
    , timer(new QTimer(this))
{
    lineNumberArea = new LineNumberArea(this);

    setTimer();

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);//改变宽度
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);//改变行号
    connect(this, &CodeEditor::cursorPositionChanged, this, [this](){
        highlightCurrentLine();
    });//改变高亮行

    connect(lineNumberArea, &LineNumberArea::clickLineArea, this, &CodeEditor::findLineNumber);

    updateLineNumberAreaWidth(10);
    highlightCurrentLine();//初始化为1

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->setLineWrapMode(QPlainTextEdit::NoWrap);
    this->setStyleSheet("background-color:rgb(34, 34, 34);"
                        "color: rgb(189, 191, 192)");

    this->setCursorWidth(2);

    this->setFrameShape(QFrame::NoFrame);//去除默认白色边框

    connect(this, &CodeEditor::textChanged, this, [this](){
        isLineListChanged = true;
    });

    connect(this->document(), &QTextDocument::contentsChanged, this, [this](){

        if (comFilePath.isEmpty()) return;
        qDebug()<<comFilePath;
        QFile file(comFilePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << this->document()->toPlainText();
            file.close();  // 关闭文件
        } else {
            // 如果文件无法打开，输出错误信息
            qWarning("无法打开文件进行写入！");
        }

        avs8086::lexer::Lexer l(comFilePath);
        avs8086::parser::Parser p(&l);
        p.newAST();
        qDebug() << p.errorInfos();

        errLineList.clear();
        warnLineList.clear();
        static const QRegularExpression re(R"(^\[(.*?)\((\d+),(\d+),(\d+)\)\]>(.*?)$)");

        errLineStringList.clear();
        warnLineStringList.clear();

        for (const auto& s : p.errorInfos())
        {
            errLineStringList.append(s);
            int errLine = 0, errWord = 0, errWordLengh = 0;
            QRegularExpressionMatch match = re.match(s);
            if (match.hasMatch())
            {
                errLine = match.captured(2).toInt();
                errWord = match.captured(3).toInt();
                errWordLengh = match.captured(4).toInt();
            }
            else
            {
                // qDebug()<<s;
                qDebug()<<"读取失败！\n"<< __FILE__ << __LINE__;
                return;
            }

            if (errLine == 0 || errWord == 0 || errWordLengh == 0)
            {
                qDebug()<<"读取错误！\n"<< __FILE__ << __LINE__;;
                return;
            }


            errLineList.append(QString::number(errLine) + " "
                               + QString::number(errWord) + " " + QString::number(errWordLengh));//相当于拷贝副本
        }

        qDebug()<<p.warningInfos();
        for (const auto& s : p.warningInfos())
        {
            warnLineStringList.append(s);
            int warnLine = 0, warnWord = 0, warnWordLengh = 0;
            QRegularExpressionMatch match = re.match(s);
            if (match.hasMatch())
            {
                warnLine = match.captured(2).toInt();
                warnWord = match.captured(3).toInt();
                warnWordLengh = match.captured(4).toInt();
            }
            else
            {

                qDebug()<<"读取失败！\n"<< __FILE__ << __LINE__;
                return;
            }

            if (warnLine == 0 || warnWord == 0 || warnWordLengh == 0)
            {
                qDebug()<<"读取错误！\n"<< __FILE__ << __LINE__;;
                return;
            }


            warnLineList.append(QString::number(warnLine) + " "
                               + QString::number(warnWord) + " " + QString::number(warnWordLengh));//相当于拷贝副本
        }
        emit ssssss();//传递给debug显示
    });

}

QWidget* CodeEditor::getTextWidget()
{
    return lineNumberArea;
}

void CodeEditor::getWarnLineList(QStringList* tmp)
{
    this->warnLineList = *tmp;
}

void CodeEditor::getErrLineList(QStringList* tmp)
{
    this->errLineList = *tmp;
}

void CodeEditor::goToErrLine(int errLine, int errWord, int errWordLengh)
{
    if (errLine > this->document()->blockCount() || errLine <= 0)
    {
        return;//不执行
    }

    QTextBlock textBlock = this->document()->findBlockByLineNumber(errLine - 1);
    QTextCursor cursor(textBlock);

    int endPosition = errWord + errWordLengh;//判断是否超出文本范围

    if (endPosition > textBlock.length()) {
        errWordLengh = textBlock.length() - errWord;
        if (errWordLengh < 0)
        {
            errWordLengh = 0;
            errWord = textBlock.length();
        }
    }

    cursor.movePosition(QTextCursor::StartOfBlock);//将光标移动到文本的开头位置
    this->setTextCursor(cursor);
    // update();
}

QString CodeEditor::get() const
{
    return this->document()->toPlainText();
}

void CodeEditor::findLineNumber(int lineNumber)//调试断点
{
    QTextBlock block = firstVisibleBlock();
    int visableLineNumber = block.blockNumber();
    int maxLineNumber = this->document()->blockCount();
    // qDebug()<<maxLineNumber;
    if (visableLineNumber + lineNumber + 1 > maxLineNumber)
    {
        return;//大于最大行号直接退出
    }
    if (!breakPoints.contains(visableLineNumber + lineNumber + 1))
    {
        breakPoints.insert(visableLineNumber + lineNumber + 1);
    }
    else
    {
        breakPoints.remove(visableLineNumber + lineNumber + 1);
    }

    update();//
}

void CodeEditor::updateUnderlines()
{
    QTextDocument* document = this->document();
    QTextCursor cursor(document);

    // 遍历文档中的所有文本块
    for (QTextBlock block = document->begin(); block != document->end(); block = block.next()) {
        cursor.setPosition(block.position());

        // 遍历文本块中的所有字符
        for (int i = 0; i < block.length() - 1; ++i) {
            // 设置光标位置
            cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);

            // 获取当前字符的格式
            QTextCharFormat format = cursor.charFormat();

            // 清除下划线样式
            if (format.underlineStyle() != QTextCharFormat::NoUnderline) {
                format.setUnderlineStyle(QTextCharFormat::NoUnderline);
                cursor.mergeCharFormat(format);
            }
        }
    }
}

bool CodeEditor::openFileSignalsSlot(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "错误", "打开文件失败！");
        return false;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    this->document()->setPlainText(content);
    return true;
}

bool CodeEditor::saveFileSlot(QString path)
{
    QFile file(path);
    qDebug()<<path;
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "错误", "打开文件失败！");
        return false;
    }

    QTextStream out(&file);
    out << (this->document()->toPlainText());
    QFileInfo check_file(path);
    if (check_file.exists())
    {
        QMessageBox::information(this, "成功", "保存文件成功！");
    }

    return true;
}

void CodeEditor::showTextSignalSlot(QString path, QString name, QTreeWidgetItem* item)//双击打开
{

}

void CodeEditor::setCompileFile(QString path)
{
    comFilePath = path;
}

void CodeEditor::updatepoints()
{
    if (!warnLineList.isEmpty())
    {
        int warnLine = 0, warnWord = 0, warnWordLengh = 0;
        for (int i = 0; i < warnLineList.length(); i++)
        {
            QStringList tmp = warnLineList.at(i).split(" ");
            warnLine = tmp.at(0).toInt();warnWord = tmp.at(1).toInt();
            warnWordLengh = tmp.at(2).toInt();
            if (warnLine == 0 || warnWord == 0 || warnWordLengh == 0)
            {
                qDebug()<<"CodeEditor:读取错误！\n"<< __FILE__ << __LINE__;
                return;
            }
            setUnderLine(UColor::YELLOW, warnLine, warnWord, warnWordLengh);
        }

        this->resetWarnLineList = warnLineList;//备份用于恢复
    }

    if (!errLineList.isEmpty())
    {
        int errLine = 0, errWord = 0, errWordLengh = 0;
        for (int i = 0; i < errLineList.length(); i++)
        {
            QStringList tmp = errLineList.at(i).split(" ");
            errLine = tmp.at(0).toInt();errWord = tmp.at(1).toInt();
            errWordLengh = tmp.at(2).toInt();
            if (errLine == 0 || errWord == 0 || errWordLengh == 0)
            {
                qDebug()<<"读取错误！\n"<< __FILE__ << __LINE__;
                return;
            }
            // qDebug()<<errLine<<errWord<<errWordLengh;
            setUnderLine(UColor::RED, errLine, errWord, errWordLengh);
        }
        this->resetErrLineList = errLineList;//备份用于恢复
    }
}

void CodeEditor::setUnderLine(UColor color, int line, int column, int length) {
    QTextDocument* document = this->document();
    if (line > document->blockCount() || line <= 0) {
        return;
    }

    QTextBlock textBlock = document->findBlockByLineNumber(line - 1);
    QTextLayout* layout = textBlock.layout();

    // 检查错误单词的边界，确保不超出文本范围
    int endPosition = column + length;
    if (endPosition > textBlock.length()) {
        length = textBlock.length() - column - 1;
    }

    QTextCharFormat format;
    format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    switch (color) {
    case UColor::RED:
        format.setUnderlineColor(Qt::red);
        break;
    case UColor::YELLOW:
        format.setUnderlineColor(Qt::yellow);
        break;
    default:
        format.setUnderlineColor(Qt::red); // 暂时其他情况使用红色
        break;
    }

    // 创建一个新的格式范围列表，以避免影响其他行
    QList<QTextLayout::FormatRange> newFormats = layout->formats();

    QTextLayout::FormatRange underlinedRange;
    underlinedRange.start = column - 1;
    underlinedRange.length = length;
    underlinedRange.format = format;

    // 添加新的下划线范围到列表
    newFormats.append(underlinedRange);

    // 应用新的格式范围列表到布局
    layout->setFormats(newFormats);

    // 标记文档内容为脏，以便更新显示
    QTextDocument* nonConstDocument = const_cast<QTextDocument*>(textBlock.document());
    nonConstDocument->markContentsDirty(textBlock.position(), textBlock.length());
}


void CodeEditor::setTimer()
{
    connect(timer, &QTimer::timeout, this, [this](){
        resetPoints();
        updatepoints();
    });

    timer->start(1100);
}

void CodeEditor::resetPoints()
{
    if (!resetWarnLineList.isEmpty() && isLineListChanged)
    {
        int warnLine = 0, warnWord = 0, warnWordLengh = 0;
        for (int i = 0; i < resetWarnLineList.length(); i++)
        {
            QStringList tmp = resetWarnLineList.at(i).split(" ");
            warnLine = tmp.at(0).toInt();warnWord = tmp.at(1).toInt();
            warnWordLengh = tmp.at(2).toInt();
            if (warnLine == 0 || warnWord == 0 || warnWordLengh == 0)
            {
                qDebug()<<"resetPoints1:读取错误！\n"<< __FILE__ << __LINE__;
                return;
            }
            reset(warnLine);
        }
        resetWarnLineList.clear();
        isLineListChanged = false;
    }
    if (!resetErrLineList.isEmpty() && isLineListChanged)
    {
        // qDebug()<<"1";
        int errLine = 0, errWord = 0, errWordLengh = 0;
        for (int i = 0; i < resetErrLineList.length(); i++)
        {
            // qDebug()<<resetErrLineList;
            QStringList tmp = resetErrLineList.at(i).split(" ");
            errLine = tmp.at(0).toInt();errWord = tmp.at(1).toInt();
            errWordLengh = tmp.at(2).toInt();
            if (errLine == 0 || errWord == 0 || errWordLengh == 0)
            {
                qDebug()<<"resetPoints2:读取错误！\n" << __FILE__ << __LINE__;
                return;
            }
            reset(errLine);
        }
        resetErrLineList.clear();
        isLineListChanged = false;
    }
}

void CodeEditor::reset(int line)
{
    QTextDocument* document = this->document();
    if (line > document->blockCount() || line <= 0) {
        return;
    }

    QTextBlock textBlock = document->findBlockByLineNumber(line - 1);
    QTextLayout* layout = textBlock.layout();

    // 移除指定行的所有下划线格式
    for (int i = 0; i < formats.size(); ++i) {
        if (formats[i].format.underlineStyle() != QTextCharFormat::NoUnderline) {
            formats[i].format.setUnderlineStyle(QTextCharFormat::NoUnderline);
        }
    }

    layout->setFormats(formats);
    QTextDocument* nonConstDocument = const_cast<QTextDocument*>(textBlock.document());
    nonConstDocument->markContentsDirty(textBlock.position(), textBlock.length());
}

int CodeEditor::lineNumberAreaWidth()//获取行号的宽度
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 48 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
//    int space = 50 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * 4;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)//给行号预留位置
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)//更新行号区域的显示
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)//设置行号的显示区域
{
    QPlainTextEdit::resizeEvent(e);


    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()//当前高亮行
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {//判断是否为只读
        QTextEdit::ExtraSelection selection;
        
        QTextCursor cursor = this->textCursor();
        QTextBlock block = cursor.block();//获取当前的block
        if (block.isValid())
        {
            QTextCharFormat textFormat;
        }

        QColor lineColor = QColor(38, 40, 46).lighter(160);//设置更亮一些
        lineColor.setAlpha(128);//设置透明度

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();//获取当前文本光标位置
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);//应用
}


void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
//    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();//获取第一个可见文本块
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            if (block == this->textCursor().block())
            {
                QString number = QString::number(blockNumber + 1);
                painter.setPen(QColor(170, 170, 170).lighter(160));
                painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                 Qt::AlignHCenter, number);
            }
            else
            {
                QString number = QString::number(blockNumber + 1);
                painter.setPen(QColor(100, 100, 100));
                painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                 Qt::AlignHCenter, number);
            }

            if(breakPoints.contains(blockNumber + 1))//画断点
            {
                painter.setBrush(QBrush(QColor(255, 0, 0), Qt::SolidPattern)); // 设置实心笔刷颜色
                painter.setPen(Qt::NoPen); // 不绘制边框
                painter.drawEllipse(7, top + 6, 10, 10);
            }
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void LineNumberArea::mousePressEvent(QMouseEvent* event)
{
    clickLineArea((event->pos().y() - 3) / fontMetrics().height());//获取当前可见的第几行
}
