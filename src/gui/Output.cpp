//
// Created by jiang.wenqiang on 2018/10/23.
//

#include "Output.hpp"

Output::Output(QWidget *parent) :
        QDockWidget(parent)
{
    setup();
}

void Output::setup()
{
    setWindowTitle(tr("输出"));
}
