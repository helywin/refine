//
// Created by jiang.wenqiang on 2018/10/24.
//

#include <QtWidgets/QAction>
#include "MarkBox.hpp"

MarkBox::MarkBox(QWidget *parent) : QDockWidget(parent)
{
    setup();
}

void MarkBox::setup()
{
    setWindowTitle(tr("标注"));
}