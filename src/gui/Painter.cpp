//
// Created by jiang.wenqiang on 2018/7/26.
//

#include "Painter.h"
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>

void Painter::paint(QPainter *painter, QPaintEvent *event) {
    painter->fillRect(event->rect(), background);
    circlePen.setWidth(line_width);
    painter->setPen(circlePen);
    double x = 0;
    double y = 50;
    double step = 0.5;
    static double offset = 0;
//    ps = event->rect().width();
    lines.clear();
    while (x < ps) {
        double x1 = x;
        double y1 = y;
        x += step;
        y = sin(x / 8 + offset) / 3;
        for (int j = 0; j < num; ++j) {
            lines.append(QLineF(x / ps * event->rect().width() * 0.98,
                                (y + j + 0.5) * event->rect().height() / num,
                                x1 / ps * event->rect().width() * 0.98,
                                (y1 + j + 0.5) * event->rect().height() / num));
        }
    }
    painter->drawLines(lines);
    offset += 0.008 * msec;
    update_time += 1;
    if (update_time == 5) {
        double inter = last.msecsTo(QTime::currentTime());
        last = QTime::currentTime();
        s = QString("msec: %1").arg(inter == 0 ? 0 : inter / 5);
        update_time = 0;
    }


    painter->setPen(textPen);
    painter->setFont(textFont);
    painter->fillRect(35, event->rect().height() - 80, 200, 40, background);
    painter->drawText(50, event->rect().height() - 50, s);
//    painter->restore();
}

Painter::Painter() {
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, Qt::white);

    background = QBrush(QColor(64, 32, 64));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::white);
    line_width = 2;
    circlePen.setWidth(line_width);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(30);
    textFont.setFamily(QString("微软雅黑"));
    fps = 0;
    last = QTime::currentTime();
    num = 20;
    msec = 10;
    ps = 1000;
    t = 0;
    x = 0;
    xx = 0;
    y = 0;
    yy = 0;
    update_time = 0;
}

void Painter::setParams(int num, int ps, int width, int msec) {
    this->num = num;
    this->ps = ps;
    this->line_width = width;
    this->msec = msec;
    points.clear();
    qDebug("清空点数");
    t = 0;
}
