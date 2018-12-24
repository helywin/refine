/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/16
 * @brief 发送报文
 * @details 模拟采集时接收的报文，发送到CAN总线上
 ******************************************************************************/

#include "Transmit.hpp"
#include "Can.hpp"
#include "SendBuffer.hpp"


Transmit::Transmit(Message *message) :
        Message(message),
        _buffer(nullptr),
        _can(nullptr),
        _msec(50),
        _frame_num(100),
        _command(Re::NoCommand),
        _status(Re::Stop)
{

}

void Transmit::setParams(Can *can, SendBuffer *buffer, unsigned long msec, int frame_num)
{
    _can = can;
    _buffer = buffer;
    _msec = msec;
    _frame_num = frame_num;
}

void Transmit::run()
{
    while (1 | _status == Re::Running) {
        msleep(_msec);
        if (!_buffer->size()) {
            continue;
        }
        int num = qMin(_frame_num, _buffer->size());
        _can->deliver(*_buffer, num);
        _buffer->move(num);

    }
}
