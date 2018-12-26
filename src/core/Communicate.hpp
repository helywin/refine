//
// Created by jiang.wenqiang on 2018/12/18.
//

#ifndef REFINE_COMMUNICATE_HPP
#define REFINE_COMMUNICATE_HPP

#include <QtCore/QThread>
#include "Message.hpp"

class SendBuffer;

class Communicate : public QThread, public Message
{
Q_OBJECT
private:
    SendBuffer *_buffer = nullptr;
    QByteArray _program;
    bool _has_program = false;

public:
    explicit Communicate(Message *message = nullptr);
    void setParams(SendBuffer *buffer);
    void burnProgram(QByteArray &&bytes);
    void sendCommand(const QByteArray &bytes);

    void begin() {}
    void stop() {}

protected:
    void run() override;
};


#endif //REFINE_COMMUNICATE_HPP
