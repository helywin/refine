/*******************************************************************************
 * @file Revolve.cpp
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 底层调度
 * @details 底层功能模块全部由Revolve实现调度，GUI不参与任何变量初始化和传递
 ******************************************************************************/

#include <QtCore/QDebug>
#include "Collect.hpp"

Collect::Collect() :
        _can(nullptr),
        _buffer(nullptr),
        _manner(FromCan),
        _frame_file(nullptr),
        _file(),
        _status(Stop),
        _cmd(None),
        _msec(10) {}


void Collect::setParams(Can *can, Buffer *buffer, Collect::Manner manner,
                        unsigned long msec, QFile *frame_file)
{
    _can = can;
    _buffer = buffer;
    _manner = manner;
    _frame_file = frame_file;
    _msec = msec;
}

#define NO_FRAME_TIMES 100  //隔多少次会报接收空白帧

void Collect::run()
{
#if 0
    static int cnt = 0;
    if (_manner == FromCan) {
        int flag = _can->collect(*_buffer);
//        qDebug() << (*_buffer->last()).str();
        if (flag == Can::Empty) {
            if (!_can->isConnected()) {
                emit error(ConnectionLost);
            } else {
                if (cnt == NO_FRAME_TIMES - 1) {
                    emit error(NoFrame);
                }
                cnt += 1;
                cnt %= NO_FRAME_TIMES;
            }
        } else {
            cnt = 0;
        }
    } else if (_manner == FromFile) {
        if (!_file.loadFrameRecord(*_buffer)) {
            emit fileEnd();
        }
    }
#endif
    while (_cmd != CommandStop) {
        msleep(_msec);
        if (_cmd == CommandPause) {
            if (_status == Running) {
                _status = Pause;
            }
            continue;
        }
        if (_cmd == CommandResume &&
            _status == Pause) {
            _can->clear();
            _status = Running;
        }

        if (_manner == FromCan) {
            static int cnt = 0;
            int flag = _can->collect(*_buffer);
//        qDebug() << (*_buffer->last()).str();
            if (flag == Can::Empty) {
                if (!_can->isConnected()) {
                    info(ErrorConnection);
                } else {
                    if (cnt == NO_FRAME_TIMES - 1) {
                        info(WarnNoFrame);
                    }
                    cnt += 1;
                    cnt %= NO_FRAME_TIMES;
                }
            } else {
                cnt = 0;
            }
        } else if (_manner == FromFile) {
        }
    }
}

void Collect::begin()
{
    _cmd = None;
//    if (_manner == FromFile) {
//        if (!_file.loadFrameRecordBegin(*_frame_file, *_buffer)) {
//            _manner = FromCan;
//            info(ErrorFile);
//        }
//    }
    _buffer->reset();
    _can->clear();
    start();
    _status = Running;
}

void Collect::stop()
{
    _cmd = CommandStop;
    while (isRunning()) {}
    _status = Stop;
//    if (_manner == FromFile) {
//        _file.loadFrameRecordFinish(*_frame_file);
//    }
}
