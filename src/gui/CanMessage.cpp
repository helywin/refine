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

}

void CanMessage::paintEvent(QPaintEvent *e)
{
    QPlainTextEdit::paintEvent(e);
    QPainter painter(this->viewport());
    QFont font("微软雅黑", 20);
    QPen pen(QColor(0, 0, 0, 120), 2);
    painter.begin(this);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(rect(), tr("%1行").arg(verticalScrollBar()->maximum()
                                           + verticalScrollBar()->pageStep()),
                     QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
    painter.end();
}
