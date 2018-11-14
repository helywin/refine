//
// Created by jiang.wenqiang on 2018/10/24.
//

#include <QtCore/QDebug>
#include "CurveViewer.hpp"
#include "Sketch.hpp"


CurveViewer::CurveViewer(QWidget *parent, Revolve *revolve, Message *message) :
        QWidget(parent),
        _sketch(new Sketch(this, revolve, this)),
        Message(message)
{
    setup();
}

void CurveViewer::setup()
{
    _layout = new QVBoxLayout(this);
    setLayout(_layout);
    _h_scroll = new QScrollBar(Qt::Orientation::Horizontal, this);
    _layout->addWidget(_sketch);
    _layout->addWidget(_h_scroll);
    _layout->setMargin(0);
    _layout->setContentsMargins(0,0,0,0);
    _sketch->setScroll(_h_scroll);
    connect(_h_scroll, &QScrollBar::valueChanged,
            this, &CurveViewer::valueChanged, Qt::DirectConnection);
    _h_scroll->setMaximum(0);
}

void CurveViewer::valueChanged(int value)
{
    _sketch->update();
}
