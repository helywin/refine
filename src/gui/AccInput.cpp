//
// Created by jiang.wenqiang on 2019/1/9.
//

#include "QtGui/QKeyEvent"
#include "AccInput.hpp"
#include "Style.hpp"

AccInput::AccInput(QWidget *parent) :
        QComboBox(parent)
{
    setup();
}

void AccInput::setup()
{
    _validator = new QRegExpValidator(QRegExp("0x[0-9a-fA-F]{0,8}"), this);
    setValidator(_validator);
    setFont(St::font_mono);
    setEditable(true);
}

void AccInput::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Return ||   //防止按下回车新增项目
        e->key() == Qt::Key_Enter) {
        e->ignore();
        return;
    }
    QComboBox::keyPressEvent(e);
}

void AccInput::addAcc(unsigned int value)
{
    addItem(toStr(value), value);
    _last_value = value;
}

void AccInput::focusOutEvent(QFocusEvent *e)
{
    QComboBox::focusOutEvent(e);
    unsigned int v = currentText().toUInt(nullptr, 16);
    auto str = toStr(v);
    setCurrentText(str);
    if (_last_value != v) {
        _last_value = v;
        if (findText(str, Qt::MatchFixedString) == -1) {
            addItem(str, _last_value);
        }
        valueChanged(_last_value);
    }
}
