//
// Created by jiang.wenqiang on 2018/10/23.
//

#include <QtWidgets/QAction>
#include "Commandbox.hpp"

Commandbox::Commandbox(QWidget *parent) :
        QDockWidget(parent)
{
    setup();
}

void Commandbox::setup()
{
    this->setWindowTitle(tr("命令"));
    _content = new QWidget(this);
    _layout = new QVBoxLayout(_content);
    _content->setLayout(_layout);
    setWidget(_content);
    _layout->setContentsMargins(5,0,0,0);
    _tab = new QTabWidget(this);
    _tab->addTab(new QWidget, "烧录");
    _tab->addTab(new QWidget, "烧录");
    _tab->addTab(new QWidget, "烧录");
    _layout->addWidget(_tab);
}

