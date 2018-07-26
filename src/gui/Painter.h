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
    struct point{double x; double y;};
private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    QVector <QLine> lines;
    QVector <Painter::point> points;
    double fps;
    QTime last;
    int freq;
    int num;
    int ps;
    int line_width;
    double t;

public:
    Painter();

    void set_param(int freq = 5, int num = 20, int ps = 1000, int width = 1);

    void paint(QPainter *painter, QPaintEvent *event);
};


#endif //REFINE_PAINTER_H
