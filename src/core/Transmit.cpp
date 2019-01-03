/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/16
 * @brief 发送报文
 * @details 模拟采集时接收的报文，发送到CAN总线上
 ******************************************************************************/

#include <QtCore/QDebug>
#include <QtCore/QTime>
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

//#define OVER_TIME 2000

void Transmit::run()
{
//    emitMessage(Re::Debug, "Transmit::run()");
//    qDebug() << "Transmit::run()";
//    int over_time = 0;
    QTime last = QTime::currentTime();
    while (_status != Re::Stop) {
        unsigned long time = 0;
        time = (unsigned long)last.msecsTo(QTime::currentTime());
        last = QTime::currentTime();
        if (time > _msec) {
            time = _msec;
        }
        msleep(_msec - time);
        if (_command == Re::CommandStop) {
            _command = Re::NoCommand;
            _status = Re::Stop;
            break;
        }
#ifdef OVER_TIME
        if (over_time > OVER_TIME) {
            emit overTime();
            _status = Re::Stop;
            break;
        }
#endif
        if (_buffer->isEmpty()) {
//            over_time += _msec;
            continue;
        }
        int num = qMin(_frame_num, _buffer->size());
        for (int i = 0; i < num; ++i) {
            _can->deliver(*_buffer, 1);
        }
//        emitMessage(Re::Debug, tr("发送了: %1").arg(num));
//        qDebug() << tr("发送了: %1").arg(num);
    }
//    emitMessage(Re::Debug, "Transmit::run() exit");
//    qDebug() << "Transmit::run() exit";
}
