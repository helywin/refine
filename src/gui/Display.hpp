//
// Created by jiang.wenqiang on 2018/9/28.
//

#ifndef REFINE_DISPLAY_HPP
#define REFINE_DISPLAY_HPP

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFunctions>
#include "Tribe.hpp"

class Display : public QGLWidget, public QGLFunctions
{
Q_OBJECT
private:
    Tribe *_tribe;

public:
    explicit Display(QWidget *parent = nullptr);

    inline void setTribe(Tribe *tribe) { _tribe = tribe; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
public:
//    void updateGL() override;
};


#endif //REFINE_DISPLAY_HPP
