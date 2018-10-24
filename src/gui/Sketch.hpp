//
// Created by jiang.wenqiang on 2018/9/28.
//

#ifndef REFINE_SKETCH_HPP
#define REFINE_SKETCH_HPP

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFunctions>
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

public:
    explicit Sketch(QWidget *parent, Revolve *revolve);

    inline void setTribe(Tribe *tribe) { _tribe = tribe; }
    inline void setCurve(Curve *curve) { _curve = curve; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
public:
//    void updateGL() override;
    void enableSmooth();
    void disableSmooth();
};


#endif //REFINE_SKETCH_HPP
