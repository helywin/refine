//
// Created by jiang.wenqiang on 2018/11/23.
//

#include <QtCore/QtMath>
#include <QtCore/QDebug>
#include "MyOpenGLWidget.hpp"

#define VSIZE 100

GLdouble vertices[VSIZE][2];

enum VAO_ID
{
    VAO1, VAO_NUMS
};

GLuint VAOS[VAO_NUMS];

void MyOpenGLWidget::initializeGL()
{
    for (int i = 0; i < VSIZE; ++i) {
        vertices[i][0] = i * qCos(8 * 2 * M_PI / VSIZE * i);
        vertices[i][1] = i * qSin(8 * 2 * M_PI / VSIZE * i);
    }
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    GLuint buffers[1];
    glGenBuffers(1, buffers);
    glGenVertexArrays(VAO_NUMS, VAOS);
    glBindVertexArray(VAOS[VAO1]);
    qDebug() << buffers[0];
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(0);
    GLuint vposition = 0;
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(10, 10, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(-VSIZE, VSIZE, -VSIZE, VSIZE, 0, 10);
}

bool first = true;

void MyOpenGLWidget::paintGL()
{
//    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
//    glEnableClientState(GL_VERTEX_ARRAY);

//    glVertexPointer(2, GL_FLOAT, GL_FALSE, vertices);
    glBindVertexArray(VAOS[VAO1]);
    glPointSize(5);
//    glDrawArrays(GL_POINTS, 0, 100);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, VSIZE);
//    glDisableClientState(GL_VERTEX_ARRAY);
    glFlush();
//    glBegin(GL_LINES);
//    glVertex2f(0,0);
//    glVertex2f(10,10);
//    glEnd();
}
