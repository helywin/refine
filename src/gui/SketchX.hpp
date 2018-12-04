//
// Created by jiang.wenqiang on 2018/11/30.
//

#ifndef REFINE_SKETCHX_HPP
#define REFINE_SKETCHX_HPP

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QPainter>
#include "Message.hpp"

class Tribe;
class Revolve;

class SketchX : public QOpenGLWidget, public Message
{
Q_OBJECT
private:
    int _x_graduate_num;
    Tribe *_tribe;
    QPainter _painter;

public:
    explicit SketchX(Message *message, Revolve *revolve, QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void setup();

    void plotXAxis();
};


#endif //REFINE_SKETCHX_HPP
