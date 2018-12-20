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
        emitMessage(Re::Debug, tr("Communicate::burnProgram 程序尚在烧录"));
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
    QByteArray::iterator start = _program.begin();
    while (start < _program.end()
//    - CAN_OBJ_DATA_LEN
            ) {
        setCanObj(*_buffer->end(), 0x611, Cd::SendType::NormalSend, start);
        _buffer->increase();
        start += CAN_OBJ_DATA_LEN;
    }
    _has_program = false;
}
