//
// Created by jiang.wenqiang on 2018/7/26.
//

#ifndef REFINE_PAINTER_H
#define REFINE_PAINTER_H

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>

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

public:
    Painter();

    void paint(QPainter *painter, QPaintEvent *event);
};


#endif //REFINE_PAINTER_H
