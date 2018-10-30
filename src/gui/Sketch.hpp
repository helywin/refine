//
// Created by jiang.wenqiang on 2018/9/28.
//

#ifndef REFINE_SKETCH_HPP
#define REFINE_SKETCH_HPP

#include <QtCore/QTimer>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFunctions>
#include <QtWidgets/QScrollBar>
#include "Tribe.hpp"
#include "Curve.hpp"

class Revolve;
/*!
 * @brief 显示曲线的控件类
 */

class Sketch : public QGLWidget, public QGLFunctions
{
Q_OBJECT
private:
    Tribe *_tribe;
    Curve *_curve;
    QTimer _timer;
    int _msec;
    QScrollBar *_scroller;

public:
    explicit Sketch(QWidget *parent, Revolve *revolve);

    inline void setTribe(Tribe *tribe) { _tribe = tribe; }
    inline void setCurve(Curve *curve) { _curve = curve; }
    inline void setScroller(QScrollBar *scroller) { _scroller = scroller; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
public:
//    void updateGL() override;
    void enableSmooth();
    void disableSmooth();

    void start(int msec = 10);
    void pause();
    void resume();
    void stop();
};


#endif //REFINE_SKETCH_HPP
