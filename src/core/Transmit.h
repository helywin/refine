//
// Created by jiang.wenqiang on 2018/8/8.
//

#ifndef REFINE_TRANSMIT_H
#define REFINE_TRANSMIT_H

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include "Can.h"
#include "Buffer.h"

class Transmit : public QThread {
Q_OBJECT
public:
    enum Result {
        Succeeded = 0,
        CanError = 1,
        BufferEmpty = 2
    };

    enum Command {
        Pause = 0,
        Resume = 1,
        Stop = 2
    };
private:
    Can *_can;
    Buffer *_buffer;
    unsigned long _interval;
    Command _cmd;
public:
    Transmit() = delete;

    Transmit(Can *can, Buffer *buffer);

    void setCan(Can *can);

    void setBuffer(Buffer *buffer);

    void setInterval(unsigned long interval);

    void pause();

    void resume();

    void stop();

protected:
    void run() override;

signals:

    void result(Result flag);
};


#endif //REFINE_TRANSMIT_H
