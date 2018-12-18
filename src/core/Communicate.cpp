//
// Created by jiang.wenqiang on 2018/12/18.
//

#include "Communicate.hpp"

Communicate::Communicate(Message *message) :
        Message(message),
        _buffer(nullptr)
{

}

void Communicate::setParams(RecvBuffer *buffer)
{
    _buffer = buffer;
}

void Communicate::burnProgram(QByteArray &&bytes)
{
    _sequence.append(Cell(Burning, bytes));
}

void Communicate::sendCommand(QByteArray &&command)
{
    _sequence.append(Cell(Command, command));
}

void Communicate::run()
{
    for (const auto &iter : _sequence) {
        switch (iter.type()) {
            case Command:
                break;
            case Burning:
                break;
        }
    }
}
