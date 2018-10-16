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
        FileError
    };
private:
    Can *_can;
    Buffer *_buffer;
    Manner _manner;
    QFile *_frame_file;
    File _file;

public:
    Collect();

    bool beginCollect(Can *can, Buffer *buffer,
                      Manner manner, QFile *frame_file);

    inline void finishCollect() { _file.loadFrameRecordFinish(*_frame_file); }

    inline Manner manner() const { return _manner; }

protected:
    void run() override;

signals:
    void error(int error);

    void fileEnd();
};


#endif //REFINE_COLLECT_HPP
