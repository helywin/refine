//
// Created by jiang.wenqiang on 2018/12/20.
//

#include "CommandPanel.hpp"

CommandPanel::CommandPanel(Message *message, QWidget *parent) :
        QWidget(parent),
        Message(message)
{
    setup();
}

void CommandPanel::setup()
{
    QFont font("微软雅黑", 10);
    _layout = new QVBoxLayout(this);
    _layout->setContentsMargins(5, 5, 0, 5);
    setLayout(_layout);
    _text = new QTextEdit(this);
    _text->setFont(font);
    _command = new QLineEdit(this);
    _command->setFont(font);
    _layout->addWidget(_text);
    _layout->addWidget(_command);
}
