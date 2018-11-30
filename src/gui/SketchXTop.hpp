//
// Created by jiang.wenqiang on 2018/11/30.
//

#ifndef REFINE_SKETCHXTOP_HPP
#define REFINE_SKETCHXTOP_HPP

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QPainter>
#include "Message.hpp"

class SketchXTop : public QOpenGLWidget, public Message
{
Q_OBJECT
private:

public:
    explicit SketchXTop(Message *message = nullptr, QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void setup();
};


#endif //REFINE_SKETCHXTOP_HPP
