//
// Created by jiang.wenqiang on 2018/11/22.
//

#include "BaudRate.hpp"

BaudRate::BaudRate(QWidget *parent) :
        QWidget(parent),
        _maximum_baud_rate(500)
{
    setup();
}

void BaudRate::setup()
{
    setMaximumWidth(200);
    _layout = new QHBoxLayout(this);
    _layout->setMargin(5);
    _layout->setContentsMargins(2, 2, 2, 2);
    setLayout(_layout);
    _progress = new FiniteProgress(this);
    _progress->setToolTip(tr("当前波特率"));
    _label = new QLabel(this);
    _label->setFont(QFont("微软雅黑"));
    _label->setFixedWidth(70);
    _label->setAlignment(Qt::AlignRight);
    _progress->setMaximum(_maximum_baud_rate);
    _layout->addWidget(_progress, 1);
    _layout->addWidget(_label);
    setBaudRate(0);
}

void BaudRate::setBaudRate(double baud_rate)
{
    if (baud_rate > _maximum_baud_rate) {
        baud_rate = _maximum_baud_rate;
    }
    double rate = baud_rate / _maximum_baud_rate;
    QColor color;
    color.setRedF(rate);
    color.setGreenF(1-rate);
    color.setBlueF(0);
    _progress->setContentColor(color);
//    _progress->setBorderColor(color);
    _progress->setValue((int) baud_rate);
    _label->setText(QString("%1 kbps").arg(baud_rate, 5, 'f', 2, QChar(' ')));
}
