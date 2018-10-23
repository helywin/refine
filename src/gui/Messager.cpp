//
// Created by jiang.wenqiang on 2018/10/23.
//

#include "Messager.hpp"

const int Messager::SEC[4] = { 1000, 3000, 6000, 30000 };

Messager::Messager(QWidget *parent) :
    QDialog(parent)
{
    setup();
}

void Messager::setup()
{
    this->setWindowFlag(Qt::ToolTip, true);
}

void Messager::info(QString &&msg)
{

}

void Messager::warning(QString &&msg)
{

}

void Messager::critical(QString &&msg)
{

}

void Messager::fatal(QString &&msg)
{

}
