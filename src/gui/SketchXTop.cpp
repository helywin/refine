//
// Created by jiang.wenqiang on 2018/11/30.
//

#include "SketchXTop.hpp"

SketchXTop::SketchXTop(Message *message, QWidget *parent) :
        Message(message),
        QOpenGLWidget(parent)
{
    setup();
}

void SketchXTop::setup()
{
    setFixedHeight(45);
}

void SketchXTop::initializeGL()
{
    QOpenGLWidget::initializeGL();
}

void SketchXTop::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);
}

void SketchXTop::paintGL()
{
    QOpenGLWidget::paintGL();
}
