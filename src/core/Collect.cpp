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
        _file() {}

bool Collect::begin()
{
    if (_manner == FromFile) {
        if (_file.loadFrameRecordBegin(*_frame_file, *_buffer)) {
            return true;
        } else {
            _manner = FromCan;
            emit error(FileError);
            return false;
        }
    }
    return true;
}

void Collect::setParams(Can *can, Buffer *buffer, Collect::Manner manner,
                        QFile *frame_file)
{
    _can = can;
    _buffer = buffer;
    _manner = manner;
    _frame_file = frame_file;
}

void Collect::run()
{
    if (_manner == FromCan) {
        int flag = _can->collect(*_buffer);
        if (flag == Can::Fail) {
            if (!_can->isConnected()) {
                emit error(ConnectionLost);
                qCritical("Collect::run CAN没连接");
            } else {
                emit error(CanFailed);
                qCritical("Collect::run CAN采集失败");
            }
        }
    } else if (_manner == FromFile) {
        if (!_file.loadFrameRecord(*_buffer)) {
            emit fileEnd();
        }
    }
}
