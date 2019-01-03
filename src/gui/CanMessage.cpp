//
// Created by jiang.wenqiang on 2019/1/2.
//

#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QScrollBar>
#include "CanMessage.hpp"

CanMessage::CanMessage(QWidget *parent) :
        QPlainTextEdit(parent)
{
    setup();
}

void CanMessage::setup()
{
    _send_format.setForeground(QBrush(Qt::red));
    _recv_format.setForeground(QBrush(Qt::black));
    setReadOnly(true);
}

void CanMessage::paintEvent(QPaintEvent *e)
{
    QPlainTextEdit::paintEvent(e);
    QPainter painter(this->viewport());
    QFont font("微软雅黑", 20);
    QPen pen(QColor(0, 0, 0, 120), 2);
//    painter.begin(this);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(rect(), tr("%1行").arg(verticalScrollBar()->maximum()
                                           + verticalScrollBar()->pageStep()),
                     QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
    painter.end();
}

void CanMessage::insertMessage(const QString &msg, CanMessage::MessageType type)
{
    QTextCursor cursor = this->textCursor();
    cursor.clearSelection();
    if (!cursor.atEnd()) {
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        setTextCursor(cursor);
    }
    switch (type) {
        case Send:
            cursor.insertText(msg, _send_format);
            break;
        case Receive:
            cursor.insertText(msg, _recv_format);
            break;
    }
    QScrollBar *scroll_v = verticalScrollBar();
    QScrollBar *scroll_h = horizontalScrollBar();
    if (scroll_v) {
        scroll_v->setSliderPosition(scroll_v->maximum());
    }
    if (scroll_h) {
        scroll_h->setSliderPosition(scroll_h->minimum());
    }
}


