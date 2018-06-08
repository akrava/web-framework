#pragma once
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

// https://github.com/isomoar/json-editor/blob/master/syntaxhighlightening/highlighter.h

class Highlighter: public QSyntaxHighlighter {
public:
     Highlighter(QTextDocument *parent = 0);
private:
     struct HighlightingRule {
         QRegExp pattern;
         QTextCharFormat format;
     };
     QVector<HighlightingRule> rules;
protected:
    void highlightBlock(const QString &text);
};
