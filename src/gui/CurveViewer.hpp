//
// Created by jiang.wenqiang on 2018/10/24.
//

#ifndef REFINE_CURVEVIEWER_HPP
#define REFINE_CURVEVIEWER_HPP

#include <QtCore/QTimer>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollBar>
#include "Message.hpp"
#include "Sketch.hpp"
#include "Global.hpp"

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
    int _msec;
    Re::RunningStatus _status;
    Re::RunningCommand _command;
    QTimer _timer;

    bool _h_scroll_pressed;

public:
    explicit CurveViewer(QWidget *parent, Revolve *revolve, Message *message = nullptr);

    inline Sketch *sketch() { return _sketch; }

    inline SketchY *sketchY() { return _sketch_y; }

    inline SketchXTop *sketchXTop() { return _sketch_xtop; }

    void resetHScroll(int len, bool reset);
    void resetVScroll(int len);

    inline void setMsec(int msec)
    {
        _msec = msec;
        _timer.setInterval(_msec);
    }

    void start();
    void pause();
    void resume();
    void stop();

    void regen(bool zoom = true);

public slots:
    void zoomPlus(double x_rate, double x_start, double y_rate, double y_start);
    void zoomMinus(double x_rate, double x_start,
                   double y_rate, double y_start, Sketch::ZoomEdges edge);
    void parallelMove(double delta_x, double delta_y);
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
    void rollViewer();

private slots:
    void hScrollChanged(int value);
    void vScrollChanged(int value);
};


#endif //REFINE_CURVEVIEWER_HPP
