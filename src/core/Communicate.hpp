//
// Created by jiang.wenqiang on 2018/12/18.
//

#ifndef REFINE_COMMUNICATE_HPP
#define REFINE_COMMUNICATE_HPP

#include <QtCore/QThread>
#include <QtCore/QVector>
#include "Message.hpp"

class SendBuffer;

class Communicate : public QThread, public Message
{
Q_OBJECT
private:
    SendBuffer *_buffer = nullptr;
    QByteArray _program;
    bool _has_program = false;
    unsigned int _burn_id = 0x611;
    QVector<unsigned int> _send_id = {0x611};

public:
    explicit Communicate(Message *message = nullptr);
    void setParams(SendBuffer *buffer);
    void burnProgram(QByteArray &&bytes);
    void sendCommand(const QByteArray &bytes);
    inline void setSendId(const QVector<unsigned int> &ids) { _send_id = ids; }
    inline void setBurnId(unsigned int id) { _burn_id = id; }

    void begin() {}
    void stop() {}

protected:
    void run() override;
};


#endif //REFINE_COMMUNICATE_HPP
