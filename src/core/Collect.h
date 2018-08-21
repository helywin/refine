//
// Created by jiang.wenqiang on 2018/8/8.
//

#ifndef REFINE_COLLECT_H
#define REFINE_COLLECT_H

#include <QtCore/QThread>
#include "Buffer.h"
#include "Can.h"

class Collect : public QThread {
Q_OBJECT
public:
    enum Result {
        Succeeded = 0,
        CanError = 1,
        BufferFull = 2
    };
private:
    Can *_can;
    Buffer *_buffer;

public:
    Collect() = delete;

    explicit Collect(Can *can, Buffer *buffer);

    void setCan(Can *can);

    void setBuffer(Buffer *buffer);

protected:
    void run() override;

public slots:

    void start() { QThread::start(); };

signals:

    void result(Result r);
};


#endif //REFINE_COLLECT_H
