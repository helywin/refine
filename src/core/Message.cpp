//
// Created by jiang.wenqiang on 2018/11/13.
//

#include "Message.hpp"

Message::Message(Message *parent) :
    _parent(parent) {}

void Message::emitMessage(Re::MessageTypes type, const QString &msg)
{
    if(_parent) {
        _parent->emitMessage(type, msg);
    }
}
