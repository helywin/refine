//
// Created by jiang.wenqiang on 2018/11/14.
//

#include "Progress.hpp"

Progress::Progress(QWidget *parent) :
        QDialog(parent)
{
    setup();
}

void Progress::setup()
{

}

void Progress::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}
