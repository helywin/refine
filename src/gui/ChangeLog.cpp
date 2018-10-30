//
// Created by jiang.wenqiang on 2018/10/30.
//

#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include "ChangeLog.hpp"

ChangeLog::ChangeLog(QWidget *parent) :
        LostFocus(parent)
{
    setup();
}

void ChangeLog::setup()
{
    setWindowTitle(tr("变更"));
    setWindowFlag(Qt::WindowType::WindowMaximizeButtonHint, true);
    setMinimumWidth(200);
    setMinimumHeight(200);
    resize(350, 450);
    _layout = new QVBoxLayout(this);
    setLayout(_layout);
    _text = new QTextEdit(this);
    _text->setFont(QFont("微软雅黑", 10));
    _text->setLineWrapMode(QTextEdit::WidgetWidth);
    _text->setReadOnly(true);
    QTextCursor cursor = _text->textCursor();
    QFile html(":res/text/changelog.html");
    html.open(QIODevice::ReadOnly);
    if (!html.isOpen()) {
        qDebug("ChangeLog::setup 打不开html文件");
    }
    QTextStream stream(&html);
    stream.setCodec("utf-8");
    QString changelog = stream.readAll();
    cursor.insertHtml(changelog);
    _layout->addWidget(_text);
    _layout->setContentsMargins(0,0,0,0);
}
