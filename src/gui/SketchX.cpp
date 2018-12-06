//
// Created by jiang.wenqiang on 2018/11/30.
//

#include "SketchX.hpp"
#include "Revolve.hpp"
#include "Tribe.hpp"

SketchX::SketchX(Message *message, Revolve *revolve, QWidget *parent) :
        Message(message),
        QOpenGLWidget(parent),
        _tribe(&revolve->tribe()),
        _x_graduate_num(10),
        _points(2000),
        _start(0),
        _end(2000),
        _msec(10)
{
    setup();
}

void SketchX::setup()
{
    setFixedHeight(45);
}

void SketchX::initializeGL()
{
    QOpenGLWidget::initializeGL();
}

void SketchX::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);
}

void SketchX::paintGL()
{
    plotXAxis();
}

#define X_MINIMUM_PIXEL 60
#define X_MAXIMUM_PIXEL 70
#define X_AXIS_LINES_LEN 7

void SketchX::plotXAxis()
{
    if (_tribe->size() == 0) {
        return;
    }
    int left = 1;
    int right = rect().width() - 1;
    int range = right - left;
    int num = 10;
    const int font_size = 10;
    int yt = rect().top() + 5;
    int yb = yt + X_AXIS_LINES_LEN;
    int ytx = yb + 7 + font_size;
//    QColor color(0, 96, 48);
    QColor color(255, 255, 255);
    _painter.begin(this);
    QFont font("Helvetica", font_size);
    font.setStyleHint(QFont::Helvetica, QFont::OpenGLCompatible);
    _painter.setFont(font);
    _painter.setPen(color);
    while (range / num > X_MAXIMUM_PIXEL) {
        num += 1;
    }
    while (range / num < X_MINIMUM_PIXEL && num > 1) {
        num -= 1;
    }
    if (num != _x_graduate_num) {
        _x_graduate_num = num;
    }
    for (int i = 0; i <= _x_graduate_num; ++i) {
        double y_val = _start / 1000.0 * _msec + (_points / 1000.0 * _msec) / _x_graduate_num * i;
        int x = qRound(left + (double(range) / double(_x_graduate_num) * i));
        _painter.drawLine(x, yt, x, yb);
        if (i != _x_graduate_num) {
            _painter.drawText(x, ytx, QString("%1s").arg(y_val, 0, 'f', 2));
        }
    }
    _painter.drawLine(left, yt, right, yt);
    _painter.end();
}
