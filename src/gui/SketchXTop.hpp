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
    int _vernier_pos;
    bool _vernier_visible;
    double _time;
public:
    explicit SketchXTop(Message *message = nullptr, QWidget *parent = nullptr);

    inline void setVernierVisible(bool visible) { _vernier_visible = visible; }

public slots:

    inline void vernierTextMove(int pos, double time)
    {
        _vernier_pos = pos;
        _time = time;
        update();
    };

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void setup();
};


#endif //REFINE_SKETCHXTOP_HPP
