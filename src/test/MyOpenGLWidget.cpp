//
// Created by jiang.wenqiang on 2018/11/23.
//

#include <QtCore/QtMath>
#include <QtCore/QDebug>
#include <QtCore/QVector>
#include "MyOpenGLWidget.hpp"

#define VSIZE 1000

QVector<double> vertices_vector;

enum VAO_ID
{
    VAO1, VAO2, VAO_NUMS
};

GLuint *VAOS;

void MyOpenGLWidget::initializeGL()
{
    for (int i = 0; i < VSIZE; ++i) {
        vertices_vector.append(i * qCos(8 * 2 * M_PI / VSIZE * i));
        vertices_vector.append(i * qSin(8 * 2 * M_PI / VSIZE * i));
    }

    for (int i = VSIZE; i < VSIZE * 2; ++i) {
        vertices_vector.append(-(i - VSIZE) * qCos(8 * 2 * M_PI / VSIZE * (i - VSIZE)));
        vertices_vector.append((i - VSIZE) * qSin(-8 * 2 * M_PI / VSIZE * (i - VSIZE)));
    }
    VAOS = new GLuint[2];
    initializeOpenGLFunctions();
    glClearColor(1, 1, 1, 1);
    auto buffers = new GLuint[2];
    glGenBuffers(2, buffers);
    glGenVertexArrays(2, VAOS);
    auto vertices = vertices_vector.data();
    qDebug() << "buffers[1] " << buffers[1];
    qDebug() << "sizeof(vertices) " << sizeof(vertices);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * VSIZE * 2, vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(VAOS[VAO1]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * VSIZE * 2, vertices + VSIZE * 2, GL_DYNAMIC_DRAW);

    glBindVertexArray(VAOS[VAO2]);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
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
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);

    glBindVertexArray(VAOS[VAO1]);
//    glPointSize(5);
//    glDrawArrays(GL_POINTS, 0, 100);
    glDrawArrays(GL_LINE_STRIP, 0, VSIZE);
    glBindVertexArray(VAOS[VAO2]);
//    glPointSize(5);
//    glDrawArrays(GL_POINTS, 0, 100);
    glDrawArrays(GL_LINE_STRIP, 0, VSIZE);
    glDisableClientState(GL_VERTEX_ARRAY);
    glFlush();
//    glBegin(GL_LINES);
//    glVertex2f(0,0);
//    glVertex2f(10,10);
//    glEnd();
}

MyOpenGLWidget::MyOpenGLWidget() : QOpenGLWidget()
{
    setFixedSize(500, 500);
//    setStyleSheet("QOpenGLWidget {background: rgba(1,1,1,1);}");
    auto p = palette();
    p.setColor(QPalette::Background, Qt::transparent);
    p.setColor(QPalette::Foreground, Qt::transparent);
    setPalette(p);
}
