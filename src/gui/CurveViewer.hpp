//
// Created by jiang.wenqiang on 2018/10/24.
//

#ifndef REFINE_CURVEVIEWER_HPP
#define REFINE_CURVEVIEWER_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QScrollBar>

class Sketch;
class Revolve;

class CurveViewer : public QWidget
{
Q_OBJECT
private:
    Sketch *_sketch;
    QVBoxLayout *_layout;
    QScrollBar *_scroller;
public:
    explicit CurveViewer(QWidget *parent, Revolve *revolve);

    inline Sketch &sketch() { return *_sketch; }
private:
    void setup();
};


#endif //REFINE_CURVEVIEWER_HPP
