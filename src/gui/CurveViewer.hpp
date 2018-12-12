//
// Created by jiang.wenqiang on 2018/10/24.
//

#ifndef REFINE_CURVEVIEWER_HPP
#define REFINE_CURVEVIEWER_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollBar>
#include "Message.hpp"

class Sketch;
class SketchXTop;
class SketchX;
class SketchY;
class Revolve;
class Tribe;

class CurveViewer : public QWidget, public Message
{
Q_OBJECT
private:
    SketchY *_sketch_y;
    SketchXTop *_sketch_xtop;
    SketchX *_sketch_x;
    Sketch *_sketch;
    QGridLayout *_layout;
    QWidget *_widget_sketch;
    QGridLayout *_layout_sketch;
    QScrollBar *_h_scroll;
    QScrollBar *_v_scroll;
    Tribe *_tribe;
    bool _zoom_x;
    bool _zoom_y;

public:
    explicit CurveViewer(QWidget *parent, Revolve *revolve, Message *message = nullptr);

    inline Sketch *sketch() { return _sketch; }

    inline SketchY *sketchY() { return _sketch_y; }

    void resetHScroll(int len, bool reset);
    void resetVScroll(int len);

public slots:
    void zoomPlus(double x_rate, double x_start, double y_rate, double y_start);
    void zoomMinus(double x_rate, double x_start,
                   double y_rate, double y_start, int edge);
//    void zoomPlusFixed();
//    void zoomMinusFixed();
    void zoomDefault();
    void zoomMinimum();
    void setXZoom(bool flag);
    void setYZoom(bool flag);

private:
    void setup();
    void zoomX(double rate, double start);
    void zoomY(double rate, double start);
    void zoomXMinusEdgeLeft(double rate);
    void zoomXMinusEdgeRight(double rate);
    void zoomYMinusEdgeBottom(double rate);
    void zoomYMinusEdgeTop(double rate);
    void zoomXDefault();
    void zoomYDefault();
    void zoomXMinimum();
    void zoomYMinimum();

private slots:
    void hScrollChanged(int value);
    void vScrollChanged(int value);
};


#endif //REFINE_CURVEVIEWER_HPP
