//
// Created by jiang.wenqiang on 2018/7/26.
//

#include "Painter.h"
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>
#include <QtCore/QTime>

void Painter::paint(QPainter *painter, QPaintEvent *event) {
    painter->fillRect(event->rect(), background);
    qDebug() << event->rect();
    static double t = 0;
    constexpr double pi2 = 3.141592653*2;
    double k = sin(pi2*t);
    t += pi2/1000;
    static int x = 0;
    static int xx = 0;
    static int y = 0;
    static int yy = 0;
    points.append({t,k});
//    painter->save();
    painter->setPen(circlePen);
    int start = 0;
    if(points.size() < 1000) {
        start = 0;
    } else {
        start = points.size() - 1000;
    }

    for (int i = start; i < points.size() - 2; ++i) {
        if (i == start) {
            x = (int)((points[i].x - points[start].x) /pi2 * event->rect().width());
            xx = (int)((points[i+1].x - points[start].x) /pi2 * event->rect().width());
            y = (int)(points[i].y * event->rect().height()/2 + (double)event->rect().height()/2);
            yy = (int)(points[i+1].y * event->rect().height()/2 + (double)event->rect().height()/2);
        } else {
            x = qMove(xx);
            xx = (int)((points[i+1].x - points[start].x) /pi2 * event->rect().width());
            y = qMove(yy);
            y = (int)(points[i+1].y * event->rect().height()/2 + (double)event->rect().height()/2);
        }
        painter->drawLine(x,y,xx,yy);
    }
//    painter->restore();
}

Painter::Painter() {
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, Qt::white);

    background = QBrush(QColor(64, 32, 64));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::white);
    circlePen.setWidth(2);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);
}
