//
// Created by jiang.wenqiang on 2018/10/24.
//

#include <QtCore/QDebug>
#include "CurveViewer.hpp"
#include "Sketch.hpp"
#include "SketchY.hpp"
#include "SketchX.hpp"
#include "SketchXTop.hpp"
#include "Revolve.hpp"
#include "Tribe.hpp"

CurveViewer::CurveViewer(QWidget *parent, Revolve *revolve, Message *message) :
        QWidget(parent),
        Message(message),
        _sketch_y(new SketchY(this, revolve, this)),
        _sketch_x(new SketchX(this, revolve ,this)),
        _sketch_xtop(new SketchXTop(this, this)),
        _sketch(new Sketch(this, revolve, this)),
        _tribe(&revolve->tribe())
{
    setup();
}

void CurveViewer::setup()
{
    _layout = new QGridLayout(this);
    _widget_sketch = new QWidget(this);
    _layout_sketch = new QGridLayout(_widget_sketch);
    _layout_sketch->setContentsMargins(0, 0, 0, 0);
    _widget_sketch->setLayout(_layout_sketch);
    _layout_sketch->addWidget(_sketch_y, 0, 0, 3, 1);
    _layout_sketch->addWidget(_sketch_xtop, 0, 1, 1, 1);
    _layout_sketch->addWidget(_sketch, 1, 1, 1, 1);
    _layout_sketch->addWidget(_sketch_x, 2, 1, 1, 1);
    auto right_blank = new QOpenGLWidget(this);
    right_blank->setFixedWidth(15);
    _layout_sketch->addWidget(right_blank, 0, 2, 3, 1);
    _layout_sketch->setSpacing(0);
    setLayout(_layout);
    _h_scroll = new QScrollBar(Qt::Orientation::Horizontal, this);
    _v_scroll = new QScrollBar(Qt::Orientation::Vertical, this);
    _layout->addWidget(_widget_sketch, 0, 0);
    _layout->addWidget(_h_scroll, 1, 0);
    _layout->addWidget(_v_scroll, 0, 1);
    _layout->setMargin(0);
    _layout->setContentsMargins(0, 0, 0, 0);
    _h_scroll->setMaximum(0);
    _v_scroll->setMaximum(0);
    _v_scroll->setVisible(false);
    connect(_h_scroll, &QScrollBar::valueChanged,
            this, &CurveViewer::hScrollChanged, Qt::DirectConnection);
    connect(_sketch, &Sketch::zoomXPlus, this, &CurveViewer::zoomXPlus);
    connect(_sketch, &Sketch::zoomXMinus, this, &CurveViewer::zoomXMinus);
    connect(_sketch, &Sketch::zoomXDefault, this, &CurveViewer::zoomXDefault);
    connect(_sketch, &Sketch::zoomXMinimum, this, &CurveViewer::zoomXMinimum);
}

void CurveViewer::hScrollChanged(int value)
{
    _sketch->setXStart(value);
    _sketch_x->setXStart(value);
    _sketch->update();
    _sketch_x->update();
}

void CurveViewer::resetHScroll(int len, bool reset)
{
    _h_scroll->setMinimum(0);
    int points = _sketch->xPoints();
    if (len <= points) {
        _h_scroll->setMaximum(0);
    } else {
        _h_scroll->setMaximum(len - points);
    }
    _h_scroll->setPageStep(points);
    if (reset) {
        _h_scroll->setValue(0);
        update();
    } else {
        _h_scroll->setValue(_h_scroll->maximum());
    }
    emitMessage(Debug, tr("重设滚动条大小: %1").arg(_h_scroll->maximum()));
}

void CurveViewer::resetVScroll(int len)
{

}

void CurveViewer::zoomXPlus()
{
    double rate = _sketch->xRate() / 1.5;
    if (rate < 1e-2){
        rate = 1e-2;
    }
    _sketch->setXRate(rate);
    _h_scroll->setMaximum(_tribe->len() - _sketch->xPoints());
    _h_scroll->setPageStep(_sketch->xPoints());
    _sketch_x->setPoints(_sketch->xPoints());
    _sketch_x->update();
    _sketch->update();
}

void CurveViewer::zoomXMinus()
{
    double rate = _sketch->xRate() * 1.5;
    if(rate > _tribe->len() / (double)Sketch::X_POINTS) {
        rate = _tribe->len() / (double)Sketch::X_POINTS;
    }
    _sketch->setXRate(rate);
    int max = _tribe->len() - _sketch->xPoints();
    if (_h_scroll->value() > max) {
        _h_scroll->setValue(max);
    }
    _h_scroll->setMaximum(max);
    _h_scroll->setPageStep(_sketch->xPoints());
    _sketch_x->setPoints(_sketch->xPoints());
    _sketch_x->update();
    _sketch->update();
}

void CurveViewer::zoomXDefault()
{
    _sketch->setXRate(1);
    int max = _tribe->len() - _sketch->xPoints();
    if (_h_scroll->value() > max) {
        _h_scroll->setValue(max);
    }
    _h_scroll->setMaximum(max);
    _h_scroll->setPageStep(_sketch->xPoints());
    _sketch_x->setPoints(_sketch->xPoints());
    _sketch_x->update();
    _sketch->update();
}

void CurveViewer::zoomXMinimum()
{
    _sketch->setXRate(_tribe->len() / (double)Sketch::X_POINTS);
    int max = _tribe->len() - _sketch->xPoints();
    if (_h_scroll->value() > max) {
        _h_scroll->setValue(max);
    }
    _h_scroll->setMaximum(max);
    _h_scroll->setPageStep(_sketch->xPoints());
    _sketch_x->setPoints(_sketch->xPoints());
    _sketch_x->update();
    _sketch->update();
}

void CurveViewer::zoomYPlus()
{

}

void CurveViewer::zoomYMinus()
{

}

void CurveViewer::zoomYDefault()
{

}

void CurveViewer::zoomYMinimum()
{

}
