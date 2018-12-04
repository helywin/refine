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
        _graduate_num(10),
        _y_rate(1.0)
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
    setMinimumHeight(150);
    setFixedWidth(60);
}

#define MINIMUM_PIXEL 40
#define MAXIMUM_PIXEL 50
#define Y_AXIS_LINES_LEN 7


void SketchY::plotYAxis()
{
    int top = 45;
    int bottom = rect().height() - 45 - 1;
    int range = bottom - top;
    int num = 10;
    int xr = rect().right();
    int xl = xr - Y_AXIS_LINES_LEN;
    int xt = qRound(rect().width() * 0.1);
    if (_current_index == -1) {
        if (_tribe->size() == 0) {
            return;
        }
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
    bool is_logic;
    if (_y_rate == 1.0) {
        is_logic = style.rangeOut()[1] - style.rangeOut()[0] < num;
        _y_min = style.rangeOut()[0];
    } else {
        is_logic = false;
    }
//    qDebug() << "SketchY::plotYAxis() is_logic: " << is_logic;
    if (is_logic) {
        num = style.rangeOut()[1] - style.rangeOut()[0];
    }
    while (range / num > MAXIMUM_PIXEL && !is_logic) {
        num += 1;
    }
    while (range / num < MINIMUM_PIXEL && num > 1) {
        num -= 1;
    }
    if (num != _graduate_num) {
        _graduate_num = num;
    }

    _painter.drawText(xt, top / 2 + 5, style.name());
    _painter.drawText(xt, bottom + top / 2 + 5, style.unit());
    for (int i = 0; i <= _graduate_num; ++i) {
        double y_val = _y_min +
                       double(style.rangeOut()[1] - style.rangeOut()[0])
                       * _y_rate / _graduate_num * i;
        int y = qRound(top + (double(range) / double(_graduate_num) * i));
        _painter.drawLine(xl, y, xr, y);
        _painter.drawText(xt, y + 5, QString("%1").arg(y_val, 0, 'f', 1));
    }
    _painter.drawLine(xr, top, xr, bottom);
    _painter.end();
}
