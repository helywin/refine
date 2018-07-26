//
// Created by jiang.wenqiang on 2018/7/26.
//

#include "Painter.h"
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <omp.h>

void Painter::paint(QPainter *painter, QPaintEvent *event) {
    painter->fillRect(event->rect(), background);
    constexpr double pi2 = 3.141592653*2;
    double k = sin(pi2*t);
    t += pi2/1000*freq;
    static int x = 0;
    static int xx = 0;
    static int y = 0;
    static int yy = 0;
    points.append({t,k});
//    painter->save();
    painter->setPen(circlePen);
    int start = 0;
    ps = event->rect().width();
    if(points.size() < ps) {
        start = 0;
    } else {
        start = points.size() - ps;
    }

    for (int i = start; i < points.size() - 2; ++i) {
        if (i == start) {
            x = (int)((points[i].x - points[start].x) /pi2/freq * event->rect().width())*1000/ps;
            xx = (int)((points[i+1].x - points[start].x) /pi2/freq * event->rect().width())*1000/ps;
            y = (int)(points[i].y * event->rect().height()/num);
            yy = (int)(points[i+1].y * event->rect().height()/num);
        } else {
            x = xx;
            xx = (int)((points[i+1].x - points[start].x) /pi2/freq * event->rect().width())*1000/ps;
            y = yy;
            yy = (int)(points[i+1].y * event->rect().height()/num);
        }
        for (int j = 1; j <= num; ++j) {
            painter->drawLine(x,y + event->rect().height()/num*j,xx,yy + event->rect().height()/num*j);
        }

    }
    static QString s;
    static int update_time = 0;
    update_time += 1;
    if (update_time == 10){
        double inter = last.msecsTo(QTime::currentTime());
        last = QTime::currentTime();
        s = QString("fps: %1").arg(inter == 0?0:10000/inter);
        update_time = 0;
    }


    painter->setPen(textPen);
    painter->setFont(textFont);
    painter->fillRect(35,event->rect().height() - 80, 200, 40, background);
    painter->drawText(50,event->rect().height() - 50,s);
//    painter->restore();
}

Painter::Painter() {
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, Qt::white);

    background = QBrush(QColor(64, 32, 64));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::white);
    circlePen.setWidth(line_width);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(30);
    textFont.setFamily(QString("微软雅黑"));
    fps = 0;
    last = QTime::currentTime();
    freq = 5;
    num = 20;
    ps = 1000;
    line_width = 1;
    t = 0;
}

void Painter::set_param(int freq, int num, int ps, int width) {
    this->freq = freq;
    this->num = num;
    this->ps = ps;
    points.clear();
    qDebug("清空点数");
    t = 0;
}
