//
// Created by jiang.wenqiang on 2018/11/29.
//

#ifndef REFINE_SKETCHY_HPP
#define REFINE_SKETCHY_HPP

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QPainter>
#include "Message.hpp"

class Revolve;
class Tribe;

class SketchY : public QOpenGLWidget, public Message
{
Q_OBJECT
public:

private:
    Tribe *_tribe;
    QPainter _painter;
    int _current_index;
    int _graduate_num;
public:
    explicit SketchY(Message *message, Revolve *revolve, QWidget *parent = nullptr);

    inline void setCurrentIndex(int index, bool is_checked)
    {
        if (is_checked) {
            _current_index = index;
        }
    }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void setup();
    void plotYAxis();

signals:
    void graduateNumChanged(int num);
};


#endif //REFINE_SKETCHY_HPP
