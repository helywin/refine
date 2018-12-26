//
// Created by jiang.wenqiang on 2018/12/18.
//

#include <QtCore/QByteArray>
#include "Communicate.hpp"
#include "SendBuffer.hpp"

Communicate::Communicate(Message *message) :
        Message(message),
        _buffer(nullptr),
        _program(),
        _has_program(false) {}

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
    start();
}

void Communicate::sendCommand(const QByteArray &bytes)
{
    setCanObj(*_buffer->end(), 0x611, Cd::SendType::NormalSend, bytes);
    _buffer->increase();
}

void Communicate::run()
{
    if (!_has_program) {
        return;
    }
    emitMessage(Re::Debug, tr("Communicate::run() 开始"));
    QByteArray::iterator start = _program.begin();
    while (start < _program.end()
                   - CAN_OBJ_DATA_LEN
            ) {
        if (_buffer->isFull()) {
            msleep(10);
            continue;
        }
        setCanObj(*_buffer->end(), 0x611, Cd::SendType::NormalSend, start);
        _buffer->increase();
        start += CAN_OBJ_DATA_LEN;
    }
    if (_program.end() - start > 0) {
        setCanObj(*_buffer->end(), 0x611, Cd::SendType::NormalSend, start,
                  (int) (_program.end() - start));  //char本来就是一个字节，所以不用除以sizeof
        _buffer->increase();
    }
    emitMessage(Re::Debug, tr("Communicate::run() 完成"));
    _has_program = false;
}
