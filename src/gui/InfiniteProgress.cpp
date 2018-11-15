//
// Created by jiang.wenqiang on 2018/11/14.
//

#include "InfiniteProgress.hpp"
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtCore/QMargins>
#include <cmath>

InfiniteProgress::InfiniteProgress(QWidget *parent, int msec) :
        _boarder(),
        _fill(),
        QWidget(parent),
        _timer(),
        _process(0),
        _len(0.3)
{
    _timer.setInterval(msec);
    connect(&_timer, &QTimer::timeout,
            this, &InfiniteProgress::updateProgress, Qt::DirectConnection);
    _timer.start();
    setMinimumWidth(5);
    setFixedHeight(15);
}

void InfiniteProgress::paintEvent(QPaintEvent *event)
{
    auto rect = event->rect().marginsRemoved(QMargins(2, 2, 2, 2));
    QPainter painter;
    painter.begin(this);
    QPen pen(_boarder);
    pen.setWidth(1);
    QBrush brush(_fill);
    painter.setPen(pen);
    painter.drawRect(rect);
    painter.setBrush(_fill);
    if (_process <= _len) {
        painter.drawRect(rect.x(), rect.y(),
                         (int)round(rect.width() * _process), rect.height());
    } else if (_len < _process && _process <= 1.0) {
        painter.drawRect((int)round(rect.width() * (_process - _len)), rect.y(),
                         (int)round(rect.width() * _len), rect.height());
    } else {
        painter.drawRect((int)round(rect.width() * (_process - _len)), rect.y(),
                         (int)round(rect.width() * (1.0 + _len  - _process)) + 1, rect.height());
    }
    _process += 0.01;
    if (_process >= 1.0 + _len) {
        _process = 0;
    }
}

void InfiniteProgress::updateProgress()
{
    this->repaint();
}
