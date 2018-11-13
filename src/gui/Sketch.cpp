//
// Created by jiang.wenqiang on 2018/9/28.
//

#include <cmath>
#include <QtCore/QDebug>
#include "Sketch.hpp"
#include "Revolve.hpp"

Sketch::Sketch(QWidget *parent, Revolve *revolve, Message *message) :
        QGLWidget(parent),
        _tribe(&revolve->tribe()),
        Message(message),
        _msec(10),
        _h_scroll(nullptr),
        _mode(Free),
        _x_pos(0),
        _y_pos(0),
        _x_rate(1),
        _y_rate(0),
        _current_index(-1)
{
    _timer.
            setInterval(_msec);
    connect(&_timer, &QTimer::timeout,
            this, &Sketch::updateGL, Qt::DirectConnection);
}

void Sketch::initializeGL()
{
    qglClearColor(Qt::black);
    setAutoBufferSwap(true);
    setSmooth(true);
}

void Sketch::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(X_LEFT - X_RIGHT * X_L_BLANK_RATE,
            X_RIGHT + X_RIGHT * X_R_BLANK_RATE,
            Y_BOTTOM - Y_TOP * Y_BLANK_RATE,
            Y_TOP + Y_TOP * Y_BLANK_RATE,
            0, 100);
}

void Sketch::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    plotXAxis();
    plotYAxis();
    plotCurves();
    plotVernier();
    glFlush();
}

void Sketch::plotCurves()
{
    int start_pos = 0;
    int len = 0;
    if (_mode == DisplayMode::Rolling) {
        if (_tribe->len() < X_RIGHT * _x_rate) {
            len = _tribe->len();
        } else {
            len = (int) (X_RIGHT * _x_rate);
            start_pos = _tribe->len() - len;
        }
    } else if (_mode == DisplayMode::Free) {
        if (_tribe->len() < X_RIGHT * _x_rate) {
            len = _tribe->len();
        } else {
            len = (int) (X_RIGHT * _x_rate);
            start_pos = _h_scroll->value();
        }
    }
    for (int i = 0; i < _tribe->size(); ++i) {
        const Tribe::Cell &iter = (*_tribe)[i];
        if (!(_tribe->styles())[i].display()) {
            continue;
        }
        const Tribe::Style &cfg = _tribe->styles()[i];
        qglColor(QColor((unsigned int) cfg.color()));
        glLineWidth(cfg.width());
        glBegin(GL_LINES);
        float y_cal = 0;
        for (int j = 0; j < len - 1; ++j) {
            if (j) {
                glVertex2f(j, y_cal);
            } else {
                glVertex2f(j, genY(iter[j + start_pos], cfg));
            }
            y_cal = genY(iter[j + start_pos + 1], cfg);
            glVertex2f(j + 1, y_cal);
        }
        glEnd();
    }
}

void Sketch::setSmooth(bool enable)
{
    if (enable) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    } else {
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
    }
    updateGL();
}

void Sketch::start(int msec)
{
    _mode = Rolling;
    _timer.start();     //采用低优先级的定时器，减少开支
    _h_scroll->setDisabled(true);
}

void Sketch::pause()
{
    _timer.stop();
    _h_scroll->setDisabled(false);
    initData();
}

void Sketch::resume()
{
    _timer.start();
    _h_scroll->setDisabled(true);
}

void Sketch::stop()
{
    _timer.stop();
    _mode = Free;
    _h_scroll->setDisabled(false);
    initData();
}

void Sketch::initData()
{
    _h_scroll->setMinimum(0);
    if (_tribe->len() < X_RIGHT * _x_rate) {
        _h_scroll->setMaximum(0);
    } else {
        _h_scroll->setMaximum((_tribe->len() - (int) ceil(X_RIGHT * _x_rate)));
    }
    emitMessage(Debug, tr("重设滚动条大小: %1").arg(_h_scroll->maximum()));
    QGLWidget::updateGL();
}

void Sketch::plotXAxis()
{

}

void Sketch::plotYAxis()
{

}

void Sketch::plotVernier()
{

}




