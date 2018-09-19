//
// Created by jiang.wenqiang on 2018/9/11.
//

#ifndef REFINE_COLLECT_HPP
#define REFINE_COLLECT_HPP

#include <QtCore/QThread>
#include <QtCore/QFile>
#include "File.hpp"

class Can;
class Buffer;

class Collect : public QThread
{
Q_OBJECT
public:
    enum CollectManner
    {
        FromCan,
        FromFile
    };

    enum ErrorCode
    {
        BufferFull,
        ConnectionLost,
        CanFailed,
        CrcError
    };

    enum CollectControl
    {
        Resume,
        Suspend,
        Interrupt,
        Stop,
    };

private:
    Can *_can;
    Buffer *_buffer;
    CollectManner _manner;
    unsigned long _delay;
    File *_file;
    CollectControl _control;
    QFile *_data;

public:
    Collect() = delete;
    Collect(Can *can, Buffer *buffer);
    void setMode(CollectManner manner, unsigned long delay = 10,
                 File *file = nullptr);
    void setDelay(unsigned long delay);
    void setFile(File *file);
    void reset();

    void startCollection();
    void suspendCollection();
    void resumeCollection();
    void interruptCollection();
    void stopCollection();

protected:
    void run() override;

signals:
    void error(int code);
    void framesGot();
    void collectionFinish();
};


#endif //REFINE_COLLECT_HPP
