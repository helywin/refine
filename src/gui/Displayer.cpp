//
// Created by jiang.wenqiang on 2018/9/28.
//

#include <QtCore/QDebug>
#include "Displayer.hpp"

#define X_AXIS_MIN 0
#define X_AXIS_MAX 2000
#define Y_AXIS_MIN 0
#define Y_AXIS_MAX 5000


Displayer::Displayer(QWidget *parent, Tribe *tribe, Curve *curve) :
        QGLWidget(parent), _tribe(tribe), _curve(curve)
{

}

void Displayer::initializeGL()
{
    qglClearColor(Qt::black);
    setAutoBufferSwap(true);
    enableSmooth();
}

void Displayer::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(X_AXIS_MIN, X_AXIS_MAX, Y_AXIS_MIN, Y_AXIS_MAX, 0, 100);
}

void Displayer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
//    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2);
    int min_len = _tribe->minLen();
    int len = min_len;
    if (len > X_AXIS_MAX) {
        len = X_AXIS_MAX;
    }
    int start_pos = min_len - len;
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

void Displayer::enableSmooth()
{
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    updateGL();
}

void Displayer::disableSmooth()
{
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    updateGL();
}


