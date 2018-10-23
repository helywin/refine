//
// Created by jiang.wenqiang on 2018/10/23.
//

#include "Toolbox.hpp"

Toolbox::Toolbox(QWidget *parent) :
        QDockWidget(parent)
{
    setup();
}

void Toolbox::setup()
{
    this->setWindowTitle(tr("工具箱"));
}
