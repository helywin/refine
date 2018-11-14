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
    _layout->setMargin(0);
    setLayout(_layout);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->addWidget(_toolbar);
    _layout->addWidget(_viewer);
    _zoom_plus = new QAction(QIcon(":res/icons/zoom+.png"), tr("放大"), this);
    _zoom_minus = new QAction(QIcon(":res/icons/zoom-.png"), tr("缩小"), this);
    _toolbar->addAction(_zoom_plus);
    _toolbar->addAction(_zoom_minus);
}

Sketch &Display::sketch()
{
    return _viewer->sketch();
}



