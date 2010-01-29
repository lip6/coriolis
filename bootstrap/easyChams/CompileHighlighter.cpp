// File : CompileHighlighter.cpp
// Date : 01.10.2009
// Author : Dupuis Damien
//

#include "CompileHighlighter.h"

CompileHighlighter::CompileHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    // defines rules
    CompileHighlighterRule rule;

    //building/linking/scanning rule : blue Bold
    _doingFormat.setForeground(Qt::blue);
    _doingFormat.setFontWeight(QFont::Bold);
    rule.format  = _doingFormat;
    rule.pattern = QRegExp("(^Building.*$|^Linking.*$|^Scanning.*$|^Built target.*$)");
    _rules.append(rule);

    //compile rule : darkBlue Normal
    _compileFormat.setForeground(Qt::darkBlue);
    rule.format  = _compileFormat;
    rule.pattern = QRegExp("(^\\[.*$)");
    _rules.append(rule);

    //install rule : cyan Bold
    _installFormat.setForeground(Qt::cyan);
    _installFormat.setFontWeight(QFont::Bold);
    rule.format  = _installFormat;
    rule.pattern = QRegExp("(^Install the project.*$)");
    _rules.append(rule);

    //--install rule : darkCyan
    _installingFormat.setForeground(Qt::darkCyan);
    rule.format  = _installingFormat;
    rule.pattern = QRegExp("(^-- Install.*$)");
    _rules.append(rule);
    
    //--uptodate : darkCyan Italic
    _uptodateFormat.setForeground(Qt::darkCyan);
    _uptodateFormat.setFontItalic(true);
    rule.format  = _uptodateFormat;
    rule.pattern = QRegExp("(^-- Up-to-date.*$)");
    _rules.append(rule);

    //warning rule
    _warningFormat.setForeground(QColor("orange"));
    rule.format  = _warningFormat;
    rule.pattern = QRegExp("(^/.*In function.*$|^/.*warning:.*$)");
    _rules.append(rule);

    //error rule
    _errorFormat.setForeground(Qt::red);
    rule.format  = _errorFormat;
    rule.pattern = QRegExp("(^/.*error:.*$|^make\\[.*$)");
    _rules.append(rule);
}

void CompileHighlighter::highlightBlock(const QString &text) {
    foreach (const CompileHighlighterRule &rule, _rules) {
        QRegExp expression (rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);
}

