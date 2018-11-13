//
// Created by jiang.wenqiang on 2018/10/23.
//

#include "Display.hpp"
#include "CurveViewer.hpp"
#include "Sketch.hpp"

Display::Display(QWidget *parent, Revolve *revolve, Message *message) :
        QWidget(parent),
        _viewer(new CurveViewer(this, revolve, this)),
        Message(message)
{
    setup();
}

void Display::setup()
{
    _toolbar = new QToolBar(this);
    _layout = new QVBoxLayout(this);
    setLayout(_layout);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->addWidget(_toolbar);
    _layout->addWidget(_viewer);
    _toolbar->addAction("放大");
}

Sketch &Display::sketch()
{
    return _viewer->sketch();
}



