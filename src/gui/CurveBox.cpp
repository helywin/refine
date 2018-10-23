//
// Created by jiang.wenqiang on 2018/10/23.
//

#include "CurveBox.hpp"

CurveBox::CurveBox(QWidget *parent) :
    QDockWidget(parent)
{
    setup();
}

void CurveBox::setup()
{
    setWindowTitle(tr("曲线"));
}
