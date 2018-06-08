#include "json_highlighter.h"
#include <QRegExp>
#include <QDebug>
#include <QTextCharFormat>
#include <QTextDocument>

// https://github.com/isomoar/json-editor/blob/master/syntaxhighlightening/highlighter.cpp

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
   HighlightingRule rule;

   //numbers
   rule.pattern = QRegExp("([-0-9.]+)(?!([^\"]*\"[\\s]*\\:))");
   rule.format.setForeground(QColor(0,0,102));
   rules.append(rule);

   //key
   rule.pattern = QRegExp ("(\"[^\"]*\")");
   rule.format.setForeground(QColor(51, 51, 0));
   rules.append(rule);

   //value
   rule.pattern = QRegExp(":\\s*([\"](?:[^\"])*[\"])");
   rule.format.setForeground(QColor(102, 0, 102));
   rules.append(rule);

   //reserved words
   rule.pattern = QRegExp("(true|false|null)(?!\"[^\"]*\")");
   rule.format.setForeground(QColor(153, 31, 0));
   rules.append(rule);
}

void Highlighter::highlightBlock(const QString &text) {
    foreach (const HighlightingRule &rule, rules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);

        while (index >= 0) {
            index = expression.pos(1);
            int length = expression.cap(1).length();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
