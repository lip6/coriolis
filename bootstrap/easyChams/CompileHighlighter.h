// File : CompileHighlighter.h
// Date : 01.10.2009
// Author : Dupuis Damien
//
#ifndef COMPILEHIGHLIGHTER_H
#define COMPILEHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>

class CompileHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

    public:
        CompileHighlighter ( QTextDocument *parent = 0 );

    protected:
        void highlightBlock ( const QString &text ); 

    private:
        struct CompileHighlighterRule {
            QTextCharFormat format;
            QRegExp         pattern;
        };
        QVector<CompileHighlighterRule> _rules;

        QTextCharFormat _compileFormat;
        QTextCharFormat _warningFormat;
        QTextCharFormat _errorFormat;
        QTextCharFormat _installFormat;
        QTextCharFormat _installingFormat;
        QTextCharFormat _uptodateFormat;
        QTextCharFormat _doingFormat;
};


#endif
