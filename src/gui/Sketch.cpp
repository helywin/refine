//
// Created by jiang.wenqiang on 2018/9/28.
//

#include <QtCore/QDebug>
#include "Sketch.hpp"
#include "Revolve.hpp"

#define X_AXIS_MIN 0
#define X_AXIS_MAX 2000
#define Y_AXIS_MIN 0
#define Y_AXIS_MAX 5000


Sketch::Sketch(QWidget *parent, Revolve *revolve) :
        QGLWidget(parent),
        _tribe(&revolve->tribe()),
        _curve(&revolve->curve()),
        _msec(10)
{
    _timer.setInterval(_msec);
    connect(&_timer, &QTimer::timeout,
            this, &Sketch::updateGL, Qt::DirectConnection);
}

void Sketch::initializeGL()
{
    qglClearColor(Qt::black);
    setAutoBufferSwap(true);
    enableSmooth();
}

void Sketch::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(X_AXIS_MIN, X_AXIS_MAX, Y_AXIS_MIN, Y_AXIS_MAX, 0, 100);
}

void Sketch::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
//    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2);
    int tribe_len = _tribe->len();
    if (!tribe_len) {
        return;
    }
    int len = tribe_len;
    if (len > X_AXIS_MAX) {
        len = X_AXIS_MAX;
    }
    int start_pos = tribe_len - len;
    for (const auto &iter : *_tribe) {
        if (!_curve->at(iter.name()).display()) {
            continue;
        }
        Curve::Cell &cfg = _curve->at(iter.name());
        qglColor(QColor(cfg.color()));
        glBegin(GL_LINES);
        float y_cal = 0;
        for (int i = 0; i < len - 1; ++i) {
            if (i) {
                glVertex2f(i, y_cal);
            } else {
                glVertex2f(i, iter[i + start_pos]);
            }
            y_cal = (iter[i + start_pos + 1] -
                     cfg.rangeOut()[0]) * (Y_AXIS_MAX - Y_AXIS_MIN) /
                    (cfg.rangeOut()[1] - cfg.rangeOut()[0]) + Y_AXIS_MIN;
            glVertex2f(i + 1, y_cal);
        }
        glEnd();
    }
//    glPopMatrix();
//    swapBuffers();
//    swapBuffers();
    glFlush();
}

void Sketch::enableSmooth()
{
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    updateGL();
}

void Sketch::disableSmooth()
{
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    updateGL();
}

void Sketch::start(int msec)
{
    _timer.start();     //采用低优先级的定时器，减少开支
}

void Sketch::pause()
{
    _timer.stop();
}

void Sketch::resume()
{
    _timer.start();
}

void Sketch::stop()
{
    _timer.stop();
}


