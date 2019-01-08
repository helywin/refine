//
// Created by jiang.wenqiang on 2018/12/18.
//

#include <QtCore/QByteArray>
#include "Communicate.hpp"
#include "SendBuffer.hpp"
#include <QtCore/QDebug>

Communicate::Communicate(Message *message) :
        Message(message)
{}

void Communicate::setParams(SendBuffer *buffer)
{
    _buffer = buffer;
}

void Communicate::burnProgram(QByteArray &&bytes)
{
    if (_has_program) {
        return;
    }
    _program = bytes;
    _has_program = true;
//    qDebug() << "program: " << bytes;
    start();
}

void Communicate::sendCommand(const QByteArray &bytes)
{
    int len = bytes.length();
    if (len > 8) {
        len = 8;
    }
    for (const auto id : _send_id) {
        setCanObj(*_buffer->end(), id, Cd::SendType::NormalSend, bytes, len);
        _buffer->increase();
    }
}

void Communicate::run()
{
    if (!_has_program) {
        return;
    }
    emitMessage(Re::Debug, tr("Communicate::run() 开始"));
    QByteArray::iterator start = _program.begin();
    while (start < _program.end() - CAN_OBJ_DATA_LEN) {
        if (_buffer->isFull()) {
            msleep(10);
            continue;
        }
        setCanObj(*_buffer->end(), _burn_id, Cd::SendType::NormalSend, start);
        _buffer->increase();
        start += CAN_OBJ_DATA_LEN;
    }
    if (_program.end() - start > 0) {   //烧录最后的几个字节
        setCanObj(*_buffer->end(), _burn_id, Cd::SendType::NormalSend, start,
                  (int) (_program.end() - start));  //char本来就是一个字节，所以不用除以sizeof
        _buffer->increase();
    }
    emitMessage(Re::Debug, tr("Communicate::run() 完成"));
    _has_program = false;
}
