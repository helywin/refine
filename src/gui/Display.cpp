//
// Created by jiang.wenqiang on 2018/9/28.
//

#include "Display.hpp"

#define X_AXIS_POINT 1000
#define Y_AXIS_POINT 5000

Display::Display(QWidget *parent) : QGLWidget(parent)
{
}

void Display::initializeGL()
{
    qglClearColor(Qt::black);
}

void Display::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0, X_AXIS_POINT, 0, Y_AXIS_POINT, 0, 100);
}

void Display::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2);
    int min_len = _tribe->minLen();
    int len =  min_len;
    if (len > X_AXIS_POINT) {
        len = X_AXIS_POINT;
    }
    int start_pos = min_len - len;
    for (const auto &iter : *_tribe) {
        glBegin(GL_LINES);
        for (int i = 0; i < len - 1; ++i) {
            glVertex2f(i , iter[i + start_pos]);
            glVertex2f(i + 1, iter[i + start_pos + 1]);
        }
        glEnd();
    }
    glFlush();
}

