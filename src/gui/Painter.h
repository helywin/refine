//
// Created by jiang.wenqiang on 2018/7/26.
//

#ifndef REFINE_PAINTER_H
#define REFINE_PAINTER_H

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtCore/QTime>

class Painter {
public:
    struct point {
        double x;
        double y;
    };
private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    QVector<QLineF> lines;
    QVector<QPointF> points;
    double fps;
    QTime last;
    int num;
    int ps;
    int line_width;
    int msec;
    double t;
    int x;
    int xx;
    int y;
    int yy;
    QString s;
    int update_time;

public:
    Painter();

    void setParams(int num = 20, int ps = 1000, int width = 1,
                   int msec = 10);

    void paint(QPainter *painter, QPaintEvent *event);
};


#endif //REFINE_PAINTER_H
