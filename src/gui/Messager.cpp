//
// Created by jiang.wenqiang on 2018/10/23.
//

#include "Messager.hpp"

Messager::Messager(QWidget *parent) :
        QTextEdit(parent)
{
    setup();
}

void Messager::setup()
{
    _show_types = 0xff;
    setFont(QFont("微软雅黑", 10));
    setLineWrapMode(QTextEdit::NoWrap);
    setReadOnly(true);
    _info_format.setForeground(QBrush(Qt::green));
    _warning_format.setForeground(QBrush(Qt::yellow));
    _critical_format.setForeground(QBrush(Qt::red));
    _fatal_format.setForeground(QBrush(Qt::black));
    _debug_format.setForeground(QBrush(Qt::gray));
}


QString Messager::Cell::str() const
{
    QString str = _time.toString("hh:mm:ss") + "  ";
    str += typeStr(_type) + "  ";
    str += _text;
    return str;
}

QString Messager::typeStr(MessageType type)
{
    QString str;
    switch (type) {
        case Info:
            str = QObject::tr("信息");
            break;
        case Warning:
            str = QObject::tr("警告");
            break;
        case Critical:
            str = QObject::tr("严重");
            break;
        case Fatal:
            str = QObject::tr("致命");
            break;
        case Debug:
            str = QObject::tr("调试");
            break;
    }
    return str;
}

void Messager::showMessage(Messager::MessageType type, const QString &msg)
{
    QTextCursor cursor = this->textCursor();
    if (!cursor.atEnd()) {
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
    }
    _messages.append(Cell(type, msg));
    if ((unsigned int)type & (unsigned int)_show_types) {
        _logs.append(_messages.last().str());
        switch (type) {
            case Info:
                cursor.insertText(_logs.last() + "\n", _info_format);
                break;
            case Warning:
                cursor.insertText(_logs.last() + "\n", _warning_format);
                break;
            case Critical:
                cursor.insertText(_logs.last() + "\n", _critical_format);
                break;
            case Fatal:
                cursor.insertText(_logs.last() + "\n", _fatal_format);
                break;
            case Debug:
                cursor.insertText(_logs.last() + "\n", _debug_format);
                break;
        }
    }
}

