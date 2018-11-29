//
// Created by jiang.wenqiang on 2018/11/29.
//

#include "SketchY.hpp"
#include "Sketch.hpp"
#include "Revolve.hpp"

SketchY::SketchY(Message *message, Revolve *revolve, QWidget *parent) :
        Message(message),
        QOpenGLWidget(parent),
        _tribe(&revolve->tribe()),
        _current_index(-1),
        _graduate_num(10)
{
    setup();
}

void SketchY::initializeGL()
{
    QOpenGLWidget::initializeGL();
}

void SketchY::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);
}

void SketchY::paintGL()
{
    plotYAxis();
}

void SketchY::setup()
{
    setFixedWidth(80);
}

#define MINIMUM_PIXEL 40
#define MAXIMUM_PIXEL 70
#define Y_AXIS_LINES_LEN 7


void SketchY::plotYAxis()
{
    int top = qRound(rect().height() * (Y_BLANK_RATE / (1 + 2 * Y_BLANK_RATE)));
    int bottom = qRound(rect().height() * (1 - (Y_BLANK_RATE / (1 + 2 * Y_BLANK_RATE))));
    int range = bottom - top;
    int num = _graduate_num;
    int xr = rect().right();
    int xl = xr - Y_AXIS_LINES_LEN;
    int xt = qRound(rect().width() * 0.1);
    if (_current_index == -1) {
        _painter.begin(this);
        _painter.setPen(QColor(Qt::white));
        _painter.drawLine(xr, top, xr, bottom);
        _painter.end();
        return;
    }
    const Tribe::Style &style = _tribe->style(_current_index);
    _painter.begin(this);
    QFont font("Helvetica", 10);
    font.setStyleHint(QFont::Helvetica, QFont::OpenGLCompatible);
    _painter.setFont(font);
    _painter.setPen(style.color());
    if (style.rangeOut()[1] - style.rangeOut()[0] < _graduate_num) {
        _graduate_num = style.rangeOut()[1] - style.rangeOut()[0];
    }
    while (range / _graduate_num < MINIMUM_PIXEL && _graduate_num > 1) {
        _graduate_num -= 1;
    }
    while (range / _graduate_num > MAXIMUM_PIXEL) {
        _graduate_num += 1;
    }
    if (num != _graduate_num) {
        emit graduateNumChanged(_graduate_num);
    }

    _painter.drawText(xt, top / 2 + 5, style.name());
    _painter.drawText(xt, bottom + top / 2 + 5, style.unit());
    for (int i = 0; i < _graduate_num + 1; ++i) {
        double y_val = style.rangeOut()[1] -
                       double(style.rangeOut()[1] - style.rangeOut()[0]) / _graduate_num * i;
        int y = qRound(top + (double(range) / double(_graduate_num) * i));
        _painter.drawLine(xl, y, xr, y);
        _painter.drawText(xt, y + 5, QString("%1").arg(y_val, 0, 'f', 2));
    }
    _painter.drawLine(xr, top, xr, bottom);
    _painter.end();
}
