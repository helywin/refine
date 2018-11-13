//
// Created by jiang.wenqiang on 2018/11/13.
//

#ifndef REFINE_MESSAGE_HPP
#define REFINE_MESSAGE_HPP

#include <QtCore/QString>

class Message
{
public:
    enum MessageType
    {
        Info = 0x01,
        Warning = 0x02,
        Critical = 0x04,
        Fatal = 0x08,
        Debug = 0x10
    };
private:
    Message *_parent;

public:
    explicit Message(Message *parent = nullptr);

protected:
    virtual void emitMessage(int type, const QString &msg);//parent为nullptr必须重载

};


#endif //REFINE_MESSAGE_HPP
