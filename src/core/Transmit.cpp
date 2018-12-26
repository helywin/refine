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

void Transmit::setParams(Can *can, SendBuffer *buffer)
{
    _can = can;
    _buffer = buffer;
}

#define OVER_TIME 2000

void Transmit::run()
{
    emitMessage(Re::Debug, "Transmit::run()");
    int over_time = 0;
    while (_status == Re::Running) {
        msleep(_msec);
        if (_command == Re::CommandStop) {
            _status = Re::Stop;
            break;
        }
        if (over_time > OVER_TIME) {
            emit overTime();
            _status = Re::Stop;
            break;
        }
        if (_buffer->size() <= 0) {
            over_time += _msec;
            continue;
        }
        unsigned long num = (unsigned long) qMin(_frame_num, _buffer->size());
        _can->deliver(*_buffer, num);
        emitMessage(Re::Debug, tr("发送了: %1").arg(num));
    }
    emitMessage(Re::Debug, "Transmit::run() exit");
}
