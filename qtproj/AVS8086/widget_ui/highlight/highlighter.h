#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

//! [0]
class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;//块高亮使用的函数

private:
    struct HighlightingRule//规则的集合，可以定义多个高亮规则
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    // QRegularExpression commentStartExpression;
    // QRegularExpression commentEndExpression;

    QTextCharFormat registerFormat;
    QTextCharFormat sm_mnemonics_AFormat;
    QTextCharFormat sm_symbols_AFormat;
    QTextCharFormat sm_typeNamesFormat;

    void setRegisterFormat(HighlightingRule* rule);
    void setSm_mnemonics_AFormat(HighlightingRule* rule);
    void setSm_symbols_AFormat(HighlightingRule* rule);
    void setSm_typeNamesFormat(HighlightingRule* rule);
};
//! [0]

#endif // HIGHLIGHTER_H
