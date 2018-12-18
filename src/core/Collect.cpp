/*******************************************************************************
 * @file Revolve.cpp
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 底层调度
 * @details 底层功能模块全部由Revolve实现调度，GUI不参与任何变量初始化和传递
 ******************************************************************************/

#include <QtCore/QDebug>
#include "Collect.hpp"

Collect::Collect(Message *message) :
        Message(message),
        _can(nullptr),
        _buffer(nullptr),
        _manner(FromCan),
        _frame_file(nullptr),
        _file(),
        _status(Re::Stop),
        _cmd(Re::NoCommand),
        _msec(10),
        _frames_loop(0) {}


void Collect::setParams(Can *can, RecvBuffer *buffer, Collect::Manner manner,
                        unsigned long msec, const QString &name)
{
    _can = can;
    _buffer = buffer;
    _manner = manner;
    if (!name.isEmpty()) {
        _frame_file.setFileName(name);
    }
    _msec = msec;
}

#define NO_FRAME_TIMES 100  //隔多少次会报接收空白帧
#define CAN_OBJ_BITS ((4+4+5+8+3)*8)

void Collect::run()
{
    int time = 0;
    while (_cmd != Re::CommandStop) {
        msleep(_msec);
        if (time >= 500) {
            double kbps = (CAN_OBJ_BITS * _frames_loop) / (double)time;
//            emitMessage(Debug, QString("波特率: %1").arg(kbps));
            emit baudRate(kbps);
            time = 0;
            _frames_loop = 0;
        }
        time += _msec;
        if (_cmd == Re::CommandPause) {
            if (_status == Re::Running) {
                _status = Re::Pause;
            }
            continue;
        }
        if (_cmd == Re::CommandResume &&
            _status == Re::Pause) {
            _can->clear();
            _status = Re::Running;
        }

        if (_manner == FromCan) {
            static int cnt = 0;
            int flag = _can->collect(*_buffer);
            if (flag == Can::Empty) {
                if (!_can->isConnected()) {
                    info(ErrorConnection);
                    return;
                } else {
                    if (cnt == NO_FRAME_TIMES - 1) {
                        info(WarnNoFrame);
                    }
                    cnt += 1;
                    cnt %= NO_FRAME_TIMES;
                }
            } else {
                _frames_loop += (*_buffer->last()).dataSize();
                cnt = 0;
            }
        }
    }
}

void Collect::begin()
{
    _cmd = Re::NoCommand;
//    if (_manner == FromFile) {
//        if (!_file.loadFrameRecordBegin(*_frame_file, *_buffer)) {
//            _manner = FromCan;
//            info(ErrorFile);
//        }
//    }
    _buffer->reset();
    _can->clear();
    start();
    _status = Re::Running;
}

void Collect::stop()
{
    _cmd = Re::CommandStop;
    while (isRunning()) {
        qDebug() << "Collect::stop waiting";
    }
    _status = Re::Stop;
//    if (_manner == FromFile) {
//        _file.loadFrameRecordFinish(*_frame_file);
//    }
}
