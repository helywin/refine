//
// Created by jiang.wenqiang on 2018/9/11.
//

#ifndef REFINE_COLLECT_HPP
#define REFINE_COLLECT_HPP

#include <QtCore/QThread>
#include <QtCore/QFile>

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
    QFile *_file;
    CollectControl _control;

public:
    Collect() = delete;
    Collect(Can *can, Buffer *buffer);

    inline void setMode(CollectManner manner, unsigned long delay = 10,
                        QFile *file = nullptr)
    {
        _manner = manner;
        _delay = delay;
        _file = file;
    }

    inline void setDelay(unsigned long delay) { _delay = delay; }

    inline void setFile(QFile *file) { _file = file; }

    inline void reset() {}

    inline void startCollection() { start(HighestPriority); }

    inline void suspendCollection() { _control = Suspend; }

    inline void resumeCollection() { _control = Resume; }

    inline void interruptCollection() { _control = Interrupt; }

    inline void stopCollection() { _control = Stop; }

protected:
    void run() override;

signals:
    void error(int code);
    void framesGot();
    void collectionFinish();
};


#endif //REFINE_COLLECT_HPP
