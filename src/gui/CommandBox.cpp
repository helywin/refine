//
// Created by jiang.wenqiang on 2018/10/23.
//

#include <QtWidgets/QAction>
#include "CommandBox.hpp"
#include "CommandPanel.hpp"

CommandBox::CommandBox(Message *message, QWidget *parent) :
        QDockWidget(parent),
        Message(message)
{
    setup();
}

void CommandBox::setup()
{
    this->setWindowTitle(tr("命令"));
//    _content = new QWidget(this);
//    _layout = new QVBoxLayout(_content);
//    _content->setLayout(_layout);
    _panel = new CommandPanel(this, this);
    setWidget(_panel);
//    _layout->setContentsMargins(5,0,0,0);
}

