//
// Created by jiang.wenqiang on 2018/11/30.
//

#ifndef REFINE_SKETCHXTOP_HPP
#define REFINE_SKETCHXTOP_HPP

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QPainter>
#include "Message.hpp"

class Revolve;
class Tribe;

class SketchXTop : public QOpenGLWidget, public Message
{
Q_OBJECT
private:
    QPainter _painter;
    Tribe *_tribe;
    int _vernier_pos;
    bool _vernier_visible;
    double _time;
    int _current_index;

public:
    explicit SketchXTop(Message *message, Revolve *revolve, QWidget *parent = nullptr);

    inline void setVernierVisible(bool visible) { _vernier_visible = visible; }

public slots:

    inline void vernierTextMove(int pos, double time)
    {
        _vernier_pos = pos;
        _time = time;
        update();
    };

    inline void setCurrentIndex(int index) { _current_index = index; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void setup();

    void currentIndexOverflow();
};


#endif //REFINE_SKETCHXTOP_HPP
