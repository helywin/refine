//
// Created by jiang.wenqiang on 2018/11/7.
//

#include "ByteRangeFrame.hpp"

ByteRangeFrame::ByteRangeFrame(bool checkable, QWidget *parent) :
        _checkable(checkable),
        QFrame(parent)
{
    setup();
}

void ByteRangeFrame::setup()
{
    setBackgroundRole(QPalette::Window);
    setAutoFillBackground(true);
    setFrameShape(QFrame::Box);
    _layout = new QHBoxLayout(this);
    _layout->setContentsMargins(5, 2, 5, 2);
    setLayout(_layout);
    if (_checkable) {
        _check = new QCheckBox(this);
        _check->setChecked(false);
        _check->setMaximumWidth(40);
        _layout->addWidget(_check, 0);
        connect(_check, &QCheckBox::stateChanged,
                this, &ByteRangeFrame::checkChanged, Qt::DirectConnection);
    } else {
        _check = nullptr;
    }
    _byte = new QSpinBox(this);
    _byte->setRange(0, 7);
    _low = new QSpinBox(this);
    _low->setRange(0, 7);
    _low->setValue(0);
    _label = new QLabel("-", this);
    _label->setMaximumWidth(10);
    _high = new QSpinBox(this);
    _high->setRange(0, 7);
    _high->setValue(7);
    _layout->addWidget(_byte, 1);
    _layout->addWidget(_low, 1);
    _layout->addWidget(_label);
    _layout->addWidget(_high, 1);
    if (_checkable) {
        _byte->setDisabled(true);
        _low->setDisabled(true);
        _high->setDisabled(true);
    }
    connect(_low, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ByteRangeFrame::lowChanged, Qt::DirectConnection);
    connect(_high, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ByteRangeFrame::highChanged, Qt::DirectConnection);
    _last_low = 0;
    _last_high = 0;
}

void ByteRangeFrame::setData(const QString &s)
{
    if (_checkable && s == "无") {
        _check->setChecked(false);
        _byte->setDisabled(true);
        _low->setDisabled(true);
        _high->setDisabled(true);
    }
    if (s != "无") {
        if (_checkable) {
            _check->setChecked(true);
        }
        QStringList list;
        list = s.split(';');
        _byte->setValue(list[0].toInt());
        list = list[1].split('~');
        _low->setValue(list[0].toInt());
        _high->setValue(list[1].toInt());
    }
}

QString ByteRangeFrame::data() const
{
    if (!_check || _check->isChecked()) {
        return QString("%1;%2~%3")
                .arg(_byte->value())
                .arg(_low->value())
                .arg(_high->value());
    } else {
        return QString("无");
    }
}

void ByteRangeFrame::checkChanged(int state)
{
    switch (state) {
        case Qt::CheckState::Checked:
            _byte->setDisabled(false);
            _low->setDisabled(false);
            _high->setDisabled(false);
            break;
        case Qt::CheckState::Unchecked:
            _byte->setDisabled(true);
            _low->setDisabled(true);
            _high->setDisabled(true);
            break;
        default:
            break;
    }
}

void ByteRangeFrame::lowChanged(int value)
{
    if (value > _high->value()) {
        if((_checkable && _check->isChecked()) ||
                !_checkable) {
            _low->setValue(_last_low);
            return;
        }
    }
    _last_low = value;
}

void ByteRangeFrame::highChanged(int value)
{
    if (value < _low->value()) {
        if((_checkable && _check->isChecked()) ||
           !_checkable) {
            _high->setValue(_last_high);
            return;
        }
    }
    _last_high = value;
}
