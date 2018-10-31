//
// Created by jiang.wenqiang on 2018/10/30.
//

#include "Settings.hpp"

Settings::Settings(QWidget *parent) : QDialog(parent)
{
    setup();
}

void Settings::setup()
{
    setWindowTitle(tr("设置"));
    _layout = new QVBoxLayout(this);
    setLayout(_layout);
    _layout->setContentsMargins(0, 0, 0, 0);
    _content = new QSplitter(this);
    _layout->addWidget(_content);
    _list = new QWidget(_content);
    _content->addWidget(_list);
    _setting_page = new QScrollArea(_content);
    _content->addWidget(_setting_page);
    _page = new QWidget(_setting_page);
    _group_look = new QGroupBox(_page);
    _buttons = new QWidget(this);
    _layout->addWidget(_buttons);
    _layout_button = new QHBoxLayout(_buttons);
    _buttons->setLayout(_layout_button);
    _button_ok = new QPushButton(tr("确定"), _buttons);
    _button_cancel = new QPushButton(tr("取消"), _buttons);
    _button_apply = new QPushButton(tr("应用"), _buttons);
    _layout_button->addWidget(_button_ok);
    _layout_button->addWidget(_button_cancel);
    _layout_button->addWidget(_button_apply);
}
