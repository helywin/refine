//
// Created by jiang.wenqiang on 2018/10/24.
//

#include "CurveViewer.hpp"
#include "Sketch.hpp"


CurveViewer::CurveViewer(QWidget *parent, Revolve *revolve) :
        QWidget(parent), _sketch(new Sketch(this, revolve))
{
    setup();
}

void CurveViewer::setup()
{
    _layout = new QVBoxLayout(this);
    setLayout(_layout);
    _scroller = new QScrollBar(Qt::Orientation::Horizontal, this);
    _layout->addWidget(_sketch);
    _layout->addWidget(_scroller);
    _layout->setMargin(0);
}
