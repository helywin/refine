//
// Created by jiang.wenqiang on 2018/10/24.
//

#include <QtCore/QDebug>
#include "CurveViewer.hpp"
#include "Sketch.hpp"
#include "SketchY.hpp"


CurveViewer::CurveViewer(QWidget *parent, Revolve *revolve, Message *message) :
        QWidget(parent),
        _sketch_y(new SketchY(this, revolve, this)),
        _sketch(new Sketch(this, revolve, this)),
        Message(message)
{
    setup();
}

void CurveViewer::setup()
{
    _layout = new QGridLayout(this);
    _widget_sketch = new QWidget(this);
    _layout_sketch = new QHBoxLayout(_widget_sketch);
    _layout_sketch->setContentsMargins(0, 0, 0, 0);
    _widget_sketch->setLayout(_layout_sketch);
    _layout_sketch->addWidget(_sketch_y);
    _layout_sketch->addWidget(_sketch);
    setLayout(_layout);
    _h_scroll = new QScrollBar(Qt::Orientation::Horizontal, this);
    _v_scroll = new QScrollBar(Qt::Orientation::Vertical, this);
    _layout_sketch->setSpacing(0);
    _layout->addWidget(_widget_sketch, 0, 0);
    _layout->addWidget(_h_scroll, 1, 0);
    _layout->addWidget(_v_scroll, 0, 1);
    _layout->setMargin(0);
    _layout->setContentsMargins(0, 0, 0, 0);
    _sketch->setScroll(_h_scroll);
    connect(_h_scroll, &QScrollBar::valueChanged,
            this, &CurveViewer::valueChanged, Qt::DirectConnection);
    connect(_sketch_y, &SketchY::graduateNumChanged, _sketch, &Sketch::setGraduateNum);
    _h_scroll->setMaximum(0);
    _v_scroll->setMaximum(0);
}

void CurveViewer::valueChanged(int value)
{
    _sketch->update();
}
