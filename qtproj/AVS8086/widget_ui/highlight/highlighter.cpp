#include "highlighter.h"

//! [0]
Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    //分别设置样式
    setRegisterFormat(&rule);
    setSm_mnemonics_AFormat(&rule);
    setSm_symbols_AFormat(&rule);
    setSm_typeNamesFormat(&rule);

//     keywordFormat.setForeground(Qt::darkBlue);
//     keywordFormat.setFontWeight(QFont::Bold);
//     const QStringList keywordPatterns = {
//         QStringLiteral("\\bchar\\b"), QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconst\\b"),
//         QStringLiteral("\\bdouble\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bexplicit\\b"),
//         QStringLiteral("\\bfriend\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bint\\b"),
//         QStringLiteral("\\blong\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"),
//         QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),
//         QStringLiteral("\\bshort\\b"), QStringLiteral("\\bsignals\\b"), QStringLiteral("\\bsigned\\b"),
//         QStringLiteral("\\bslots\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstruct\\b"),
//         QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypename\\b"),
//         QStringLiteral("\\bunion\\b"), QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\bvirtual\\b"),
//         QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bbool\\b")
//     };
//     for (const QString &pattern : keywordPatterns) {
//         rule.pattern = QRegularExpression(pattern);
//         rule.format = keywordFormat;
//         highlightingRules.append(rule);
// //! [0] //! [1]
//     }

//     classFormat.setFontWeight(QFont::Bold);
//     classFormat.setForeground(Qt::darkMagenta);
//     rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
//     rule.format = classFormat;

//     singleLineCommentFormat.setForeground(Qt::red);
//     rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
//     rule.format = singleLineCommentFormat;
//     highlightingRules.append(rule);

//     multiLineCommentFormat.setForeground(Qt::red);

//     quotationFormat.setForeground(Qt::darkGreen);
//     rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
//     rule.format = quotationFormat;
//     highlightingRules.append(rule);
// //! [4]

// //! [5]
//     functionFormat.setFontItalic(true);
//     functionFormat.setForeground(Qt::blue);
//     rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
//     rule.format = functionFormat;
//     highlightingRules.append(rule);
//! [5]

//! [6]
//! 多行注释
    // commentStartExpression = QRegularExpression(QStringLiteral(" /\\*"));
    // commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}
//! [6]

//! [7]
void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    //下面代码是检查多行注释的

// //! [7] //! [8]0
//     setCurrentBlockState(0);
// //! [8]

// //! [9]
//     int startIndex = 0;
//     if (previousBlockState() != 1)
//         startIndex = text.indexOf(commentStartExpression);

// //! [9] //! [10]
//     while (startIndex >= 0) {
// //! [10] //! [11]
//         QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
//         int endIndex = match.capturedStart();
//         int commentLength = 0;
//         if (endIndex == -1) {
//             setCurrentBlockState(1);
//             commentLength = text.length() - startIndex;
//         } else {
//             commentLength = endIndex - startIndex
//                             + match.capturedLength();
//         }
//         setFormat(startIndex, commentLength, multiLineCommentFormat);
//         startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
//     }
}

void Highlighter::setRegisterFormat(HighlightingRule* rule)
{
    registerFormat.setForeground(QColor(200, 0, 0).lighter(160));
    registerFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns = {
        QStringLiteral("\\bAL\\b"), QStringLiteral("\\bCL\\b"), QStringLiteral("\\bDL\\b"),
        QStringLiteral("\\bBL\\b"), QStringLiteral("\\bAH\\b"), QStringLiteral("\\bCH\\b"),
        QStringLiteral("\\bDH\\b"), QStringLiteral("\\bBH\\b"), QStringLiteral("\\bAX\\b"),
        QStringLiteral("\\bCX\\b"), QStringLiteral("\\bDX\\b"), QStringLiteral("\\bBX\\b"),
        QStringLiteral("\\bSP\\b"), QStringLiteral("\\bBP\\b"), QStringLiteral("\\bSI\\b"),
        QStringLiteral("\\bDI\\b")
    };
    for (auto pattern : keywordPatterns)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = registerFormat;
        highlightingRules.append(*rule);
    }

    registerFormat.setForeground(QColor(200, 0, 0).lighter(100));
    registerFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_2 = {
        QStringLiteral("\\bES\\b"), QStringLiteral("\\bCS\\b"),
        QStringLiteral("\\bSS\\b"), QStringLiteral("\\bDS\\b"), QStringLiteral("\\bIP\\b")
    };
    for (auto pattern : keywordPatterns_2)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = registerFormat;
        highlightingRules.append(*rule);
    }

    registerFormat.setForeground(QColor(34, 34, 34));
    registerFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_3 = {QStringLiteral("\\bIP\\b")};
    for (auto pattern : keywordPatterns_3)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = registerFormat;
        highlightingRules.append(*rule);
    }
}

void Highlighter::setSm_mnemonics_AFormat(HighlightingRule* rule)
{
    sm_symbols_AFormat.setForeground(QColor(23, 74, 228).lighter(160));
    sm_symbols_AFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns = {
        QStringLiteral("\\bMOV\\b"), QStringLiteral("\\bPUSH\\b"), QStringLiteral("\\bPOP\\b"),
        QStringLiteral("\\bXCHG\\b"), QStringLiteral("\\bXLAT\\b"), QStringLiteral("\\bLEA\\b"),
        QStringLiteral("\\bLOS\\b"), QStringLiteral("\\bLES\\b"), QStringLiteral("\\bLAHF\\b"),
        QStringLiteral("\\bSAHF\\b"), QStringLiteral("\\bPUSHF\\b"), QStringLiteral("\\bPOPF\\b"),
        QStringLiteral("\\bIN\\b"), QStringLiteral("\\bOUT\\b"), QStringLiteral("\\bADD\\b"),
        QStringLiteral("\\bADC\\b"), QStringLiteral("\\bINC\\b"), QStringLiteral("\\bSUB\\b"), QStringLiteral("\\bSBB\\b"),
        QStringLiteral("\\bDEC\\b"), QStringLiteral("\\bNEG\\b"), QStringLiteral("\\bCMP\\b"),
        QStringLiteral("\\bMUL\\b"), QStringLiteral("\\bIMUL\\b"), QStringLiteral("\\bDIV\\b"),
        QStringLiteral("\\bIDIV\\b"), QStringLiteral("\\bCBW\\b"), QStringLiteral("\\bCWD\\b"),
        QStringLiteral("\\bAAA\\b"), QStringLiteral("\\bDAA\\b"), QStringLiteral("\\bAAS\\b"),
        QStringLiteral("\\bDAS\\b"), QStringLiteral("\\bLOOP\\b"), QStringLiteral("\\bLOOPE\\b"), QStringLiteral("\\bLOOPZ\\b"),
        QStringLiteral("\\bLOOPNE\\b"), QStringLiteral("\\bLOOPNZ\\b"), QStringLiteral("\\bJCXZ\\b"),
        QStringLiteral("\\bINT\\b"), QStringLiteral("\\bINTO\\b"), QStringLiteral("\\bIRET\\b"),
        QStringLiteral("\\bCLC\\b"), QStringLiteral("\\bSTC\\b"), QStringLiteral("\\bCMC\\b"),
        QStringLiteral("\\bCLD\\b"), QStringLiteral("\\bSTD\\b"), QStringLiteral("\\bCLT\\b"),
        QStringLiteral("\\bSTI\\b"), QStringLiteral("\\bWAIT\\b"), QStringLiteral("\\bESC\\b"), QStringLiteral("\\bLOCK\\b"),
        QStringLiteral("\\bHLT\\b"), QStringLiteral("\\bNOP\\b"), QStringLiteral("\\bDAS\\b"),
        QStringLiteral("\\bAAM\\b"), QStringLiteral("\\bAAD\\b"), QStringLiteral("\\bAND\\b"),
        QStringLiteral("\\bOR\\b"), QStringLiteral("\\bXOR\\b"), QStringLiteral("\\bNOT\\b"),
        QStringLiteral("\\bTEST\\b"), QStringLiteral("\\bSAL\\b"), QStringLiteral("\\bSAR\\b"),
        QStringLiteral("\\bSHL\\b"), QStringLiteral("\\bROL\\b"), QStringLiteral("\\bROR\\b"), QStringLiteral("\\bRCL\\b"),
        QStringLiteral("\\bRCR\\b"), QStringLiteral("\\bMOVSB\\b"), QStringLiteral("\\bMOVSW\\b"),
        QStringLiteral("\\bCMPSB\\b"), QStringLiteral("\\bCMPSW\\b"), QStringLiteral("\\bSCASB\\b"),
        QStringLiteral("\\bSCASW\\b"), QStringLiteral("\\bLOOSB\\b"), QStringLiteral("\\bLOOSW\\b"),
        QStringLiteral("\\bSTOSB\\b"), QStringLiteral("\\bSTOSW\\b"), QStringLiteral("\\bREP\\b"),
        QStringLiteral("\\bREPE\\b"),  QStringLiteral("\\bREPZ\\b"), QStringLiteral("\\bREPNE\\b"), QStringLiteral("\\bREPNZ\\b"),
        QStringLiteral("\\bCALL\\b"), QStringLiteral("\\bRET\\b"), QStringLiteral("\\bJMP\\b"),
        QStringLiteral("\\bJA\\b"), QStringLiteral("\\bJNBE\\b"), QStringLiteral("\\bJAE\\b"),
        QStringLiteral("\\bJNB\\b"), QStringLiteral("\\bJB\\b"), QStringLiteral("\\bJNAE\\b"),
        QStringLiteral("\\bJBE\\b"), QStringLiteral("\\bJNA\\b"), QStringLiteral("\\bJC\\b"),
        QStringLiteral("\\bJNC\\b"), QStringLiteral("\\bJE\\b"), QStringLiteral("\\bJZ\\b"), QStringLiteral("\\bJNE\\b"),
        QStringLiteral("\\bJNZ\\b"), QStringLiteral("\\bJG\\b"), QStringLiteral("\\bJNLE\\b"),
        QStringLiteral("\\bJGE\\b"), QStringLiteral("\\bJNL\\b"), QStringLiteral("\\bJL\\b"),
        QStringLiteral("\\bJNGE\\b"), QStringLiteral("\\bJLE\\b"), QStringLiteral("\\bJNG\\b"),
        QStringLiteral("\\bJO\\b"), QStringLiteral("\\bJNO\\b"), QStringLiteral("\\bJNP\\b"),
        QStringLiteral("\\bJPO\\b"), QStringLiteral("\\bJP\\b"), QStringLiteral("\\bJPE\\b"), QStringLiteral("\\bJNS\\b"),
        QStringLiteral("\\bJS\\b")};
    for (auto pattern : keywordPatterns)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_symbols_AFormat;
        highlightingRules.append(*rule);
    }

    sm_symbols_AFormat.setForeground(QColor(26, 26, 116).lighter(160));
    sm_symbols_AFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_2 = {
        QStringLiteral("\\bINCLUDE\\b"), QStringLiteral("\\bORG\\b"), QStringLiteral("\\bEQU\\b"),
        QStringLiteral("\\bDB\\b"), QStringLiteral("\\bDW\\b"), QStringLiteral("\\bDD\\b"),
        QStringLiteral("\\bDQ\\b"), QStringLiteral("\\bDT\\b")
    };
    for (auto pattern : keywordPatterns_2)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_symbols_AFormat;
        highlightingRules.append(*rule);
    }
}

void Highlighter::setSm_symbols_AFormat(HighlightingRule* rule)
{
    sm_symbols_AFormat.setForeground(QColor(0, 0, 0));
    sm_symbols_AFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns = {QStringLiteral("\\bEOF\\b")};
    for (auto pattern : keywordPatterns)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_symbols_AFormat;
        highlightingRules.append(*rule);
    }


    sm_symbols_AFormat.setForeground(QColor(192, 220, 170));
    sm_symbols_AFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_2 = {
        QStringLiteral("\\b0X[0-9A-F]*?\\b"),
        QStringLiteral("\\b[0-9A-F]+?H\\b"),
        QStringLiteral("\\b([0-7]+?O)|([0-9]+?D?)\\b"),

        QStringLiteral("\\b(0X|[0-9])([0-9A-F]*?)([HDOB]?)\\b"),QStringLiteral("\\b\\\"\\b"),QStringLiteral(R"([\'\"].*?[\'\"])"),
        // QStringLiteral("\\b[~*/%+-&^|]\\b"),


        // QStringLiteral("\\b\\~\\b"), QStringLiteral("\\b\\*\\b"), QStringLiteral("\\b\\/\\b"),
        // QStringLiteral("\\b%\\b"), QStringLiteral("\\b+\\b"), QStringLiteral("\\b-\\b"),
        // QStringLiteral("\\b<<\\b"), QStringLiteral("\\b>>\\b"), QStringLiteral("\\b&\\b"),
        // QStringLiteral("\\b^\\b"), QStringLiteral("\\b|\\b"), QStringLiteral("\\b<\\b"),
        // QStringLiteral("\\b>\\b"), QStringLiteral("\\b<=\\b"), QStringLiteral("\\b>=\\b"),
        // QStringLiteral("\\b==\\b"), QStringLiteral("\\b!=CL\\b"), QStringLiteral("\\b=\\b"),
        // QStringLiteral("\\b,\\b"), QStringLiteral("\\b:\\b"), QStringLiteral("\\b?\\b"),
        // QStringLiteral("\\b<<\\b"), QStringLiteral("\\b>>\\b"), QStringLiteral("\\b&\\b"),
        // QStringLiteral("\\b$\\b"), QStringLiteral("\\b#\\b"), QStringLiteral("\\b(\\b"),
        // QStringLiteral("\\b)\\b"), QStringLiteral("\\w[\\w"), QStringLiteral("\\w]\\w")
    };
    for (auto pattern : keywordPatterns_2)
    {
        rule->pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_symbols_AFormat;
        highlightingRules.append(*rule);
    }

    sm_symbols_AFormat.setForeground(QColor(0, 128, 0));
    sm_symbols_AFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_3 = {QStringLiteral(";.*(?:\\n|$)")};
    for (auto pattern : keywordPatterns_3)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_symbols_AFormat;
        highlightingRules.append(*rule);
    }

    sm_symbols_AFormat.setForeground(QColor(156, 220, 221).lighter(160));
    sm_symbols_AFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_5 = {QStringLiteral("[\\[\\]]")};
    for (auto pattern : keywordPatterns_5)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_symbols_AFormat;
        highlightingRules.append(*rule);
    }

    sm_symbols_AFormat.setForeground(QColor(61, 137, 214).lighter(140));
    sm_symbols_AFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_4 = {QStringLiteral("[\\[\\]]")};
    for (auto pattern : keywordPatterns_4)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_symbols_AFormat;
        highlightingRules.append(*rule);
    }
}

void Highlighter::setSm_typeNamesFormat(HighlightingRule* rule)
{
    sm_typeNamesFormat.setForeground(QColor(220, 220, 170).lighter(160));
    sm_typeNamesFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns = {
        QStringLiteral("\\bRegister\\b")
    };
    for (auto pattern : keywordPatterns)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_typeNamesFormat;
        highlightingRules.append(*rule);
    }

    sm_typeNamesFormat.setForeground(QColor(220, 220, 0).lighter(160));
    sm_typeNamesFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_1 = {
        QStringLiteral("\\bIllegal\\b")
    };
    for (auto pattern : keywordPatterns_1)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_typeNamesFormat;
        highlightingRules.append(*rule);
    }

    sm_typeNamesFormat.setForeground(QColor(220, 0, 170).lighter(160));
    sm_typeNamesFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_2 = {
        QStringLiteral("\\bLabel\\b")
    };
    for (auto pattern : keywordPatterns_2)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_typeNamesFormat;
        highlightingRules.append(*rule);
    }

    sm_typeNamesFormat.setForeground(QColor(0, 220, 170).lighter(160));
    sm_typeNamesFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_3 = {
        QStringLiteral("\\bString\\b")
    };
    for (auto pattern : keywordPatterns_3)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_typeNamesFormat;
        highlightingRules.append(*rule);
    }

    sm_typeNamesFormat.setForeground(QColor(110, 220, 170).lighter(160));
    sm_typeNamesFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_4 = {
        QStringLiteral("\\bInteger\\b")
    };
    for (auto pattern : keywordPatterns_4)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_typeNamesFormat;
        highlightingRules.append(*rule);
    }

    sm_typeNamesFormat.setForeground(QColor(110, 110, 170).lighter(160));
    sm_typeNamesFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_5 = {
        QStringLiteral("\\bFloat\\b")
    };
    for (auto pattern : keywordPatterns_5)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_typeNamesFormat;
        highlightingRules.append(*rule);
    }

    sm_typeNamesFormat.setForeground(QColor(110, 220, 170).lighter(160));
    sm_typeNamesFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_6 = {
        QStringLiteral("\\bMAKE_\\b")
    };
    for (auto pattern : keywordPatterns_6)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_typeNamesFormat;
        highlightingRules.append(*rule);
    }

    sm_typeNamesFormat.setForeground(QColor(110, 220, 170).lighter(160));
    sm_typeNamesFormat.setFontWeight(QFont::Bold);//设置加粗
    const QStringList keywordPatterns_7 = {
        QStringLiteral("\\bLOAD_\\b")
    };
    for (auto pattern : keywordPatterns_7)
    {
        rule->pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule->format = sm_typeNamesFormat;
        highlightingRules.append(*rule);
    }
}
//! [11]
