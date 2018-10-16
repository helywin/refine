//
// Created by jiang.wenqiang on 2018/10/15.
//

#include "Collect.hpp"

Collect::Collect(Can *can, Buffer *buffer) :
        _can(can), _buffer(buffer), _manner(FromCan), _file_frame(nullptr),
        _file() {}

void Collect::run()
{
    if (_manner == FromCan) {
        if (_buffer->isFull()) {
            emit error(BufferFull);
            qCritical("Collect::run 缓冲已满");
        }
        int flag = _can->collect(*_buffer);
        if (flag == Can::ReceiveFailed) {
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
