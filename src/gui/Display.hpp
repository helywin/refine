//
// Created by jiang.wenqiang on 2018/9/28.
//

#ifndef REFINE_DISPLAY_HPP
#define REFINE_DISPLAY_HPP

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFunctions>
#include "Tribe.hpp"
#include "Curve.hpp"

class Display : public QGLWidget, public QGLFunctions
{
Q_OBJECT
private:
    Tribe *_tribe;
    Curve *_curve;

public:
    explicit Display(QWidget *parent = nullptr, Tribe *tribe = nullptr,
                     Curve *curve = nullptr);

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


#endif //REFINE_DISPLAY_HPP
