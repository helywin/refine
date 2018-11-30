//
// Created by jiang.wenqiang on 2018/11/30.
//

#include "SketchX.hpp"

SketchX::SketchX(Message *message, QWidget *parent) :
        Message(message),
        QOpenGLWidget(parent)
{
    setup();
}

void SketchX::setup()
{
    setFixedHeight(45);
}

void SketchX::initializeGL()
{
    QOpenGLWidget::initializeGL();
}

void SketchX::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);
}

void SketchX::paintGL()
{
    QOpenGLWidget::paintGL();
}
