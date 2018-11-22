//
// Created by jiang.wenqiang on 2018/11/14.
//

#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include "FiniteProgress.hpp"

FiniteProgress::FiniteProgress(QWidget *parent) :
        QWidget(parent),
        _maximum(100),
        _minimum(0),
        _value(30),
        _border_width(1),
        _border_color(Qt::black),
        _content_color(Qt::black) {}

void FiniteProgress::paintEvent(QPaintEvent *event)
{
    auto rect = event->rect().marginsRemoved(QMargins(1, 2, 1, 2));
    QPainter painter;
    painter.begin(this);
    QPen pen(_border_color);
    pen.setWidth(_border_width);
    QBrush brush(_content_color);
    painter.setPen(pen);
    painter.drawRect(rect);
    pen.setColor(_content_color);
    painter.setPen(pen);
    painter.setBrush(brush);
    int x = qRound((_value - _minimum) * rect.width() / (double) (_maximum - _minimum));
    if (x > rect.width() - _border_width * 2) {
        x = rect.width() - _border_width * 2;
    }
    if (x > 0) {
        painter.drawRect(rect.x() + _border_width,
                         rect.y() + _border_width,
                         x, rect.height() - _border_width * 2);
    }
    painter.end();
}
