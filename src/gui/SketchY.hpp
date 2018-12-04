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
    double _y_min;
    double _y_rate;
public:
    explicit SketchY(Message *message, Revolve *revolve, QWidget *parent = nullptr);

    inline void setCurrentIndex(int index, bool is_checked)
    {
        if (is_checked) {
            _current_index = index;
        } else {
            _current_index = -1;
        }
    }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void setup();
    void plotYAxis();
};


#endif //REFINE_SKETCHY_HPP
