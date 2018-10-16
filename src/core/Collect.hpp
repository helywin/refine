//
// Created by jiang.wenqiang on 2018/10/15.
//

#ifndef REFINE_COLLECT_HPP
#define REFINE_COLLECT_HPP

#include <QtCore/QThread>
#include <QtCore/QFile>
#include "Buffer.hpp"
#include "Can.hpp"
#include "File.hpp"

class Collect : public QThread
{
Q_OBJECT
public:
    enum Manner
    {
        FromCan,
        FromFile
    };

    enum Error
    {
        BufferFull,
        ConnectionLost,
        CanFailed,
        CrcError
    };
private:
    Can *_can;
    Buffer *_buffer;
    Manner _manner;
    QFile *_file_frame;
    File _file;

public:
    Collect(Can *can, Buffer *buffer);

    inline void setCan(Can *can)
    {
        Q_ASSERT(can != nullptr);
        _can = can;
    }

    inline void setBuffer(Buffer *buffer)
    {
        Q_ASSERT(buffer != nullptr);
        _buffer = buffer;
    }

    inline void setFromCan() { _manner = FromCan; }

    inline bool setFromFile(QFile *frame_file)
    {
        Q_ASSERT(frame_file != nullptr);
        _manner = FromFile;
        _file_frame = frame_file;
        if (_file.loadFrameRecordBegin(*_file_frame, *_buffer)) {
            return true;
        } else {
            _manner = FromCan;
            return false;
        }
    }

protected:
    void run() override;

signals:
    void error(int error);

    void fileEnd();
};


#endif //REFINE_COLLECT_HPP
