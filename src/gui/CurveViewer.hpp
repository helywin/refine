//
// Created by jiang.wenqiang on 2018/10/24.
//

#ifndef REFINE_CURVEVIEWER_HPP
#define REFINE_CURVEVIEWER_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollBar>
#include "Message.hpp"
class Sketch;
class Revolve;

class CurveViewer : public QWidget, public Message
{
Q_OBJECT
private:
    Sketch *_sketch;
    QGridLayout *_layout;
    QScrollBar *_h_scroll;
    QScrollBar *_v_scroll;
public:
    explicit CurveViewer(QWidget *parent, Revolve *revolve,
            Message *message = nullptr);

    inline Sketch &sketch() { return *_sketch; }
private:
    void setup();
private slots:
    void valueChanged(int value);
};


#endif //REFINE_CURVEVIEWER_HPP
