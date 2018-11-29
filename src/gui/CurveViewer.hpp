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
class SketchY;
class Revolve;

class CurveViewer : public QWidget, public Message
{
Q_OBJECT
private:
    SketchY *_sketch_y;
    Sketch *_sketch;
    QGridLayout *_layout;
    QWidget *_widget_sketch;
    QHBoxLayout *_layout_sketch;
    QScrollBar *_h_scroll;
    QScrollBar *_v_scroll;
public:
    explicit CurveViewer(QWidget *parent, Revolve *revolve,
            Message *message = nullptr);

    inline Sketch &sketch() { return *_sketch; }

    inline SketchY &sketchY() { return *_sketch_y; }
private:
    void setup();
private slots:
    void valueChanged(int value);
};


#endif //REFINE_CURVEVIEWER_HPP
