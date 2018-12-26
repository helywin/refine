//
// Created by jiang.wenqiang on 2018/11/13.
//

#ifndef REFINE_MESSAGE_HPP
#define REFINE_MESSAGE_HPP

#include <QtCore/QString>
#include "Global.hpp"

using Re::MessageTypes;

class Message
{
public:
private:
    Message *_parent;

public:
    explicit Message(Message *parent = nullptr);

protected:
    virtual void emitMessage(MessageTypes type, const QString &msg);//parent为nullptr必须重载

};


#endif //REFINE_MESSAGE_HPP
