//
// Created by jiang.wenqiang on 2018/8/8.
//

#ifndef REFINE_TRANSMIT_H
#define REFINE_TRANSMIT_H

#include <QtCore/QThread>
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
private:
    Can *_can;
    Buffer *_buffer;
public:
    Transmit() = delete;

    Transmit(Can *can, Buffer *buffer);

    void setCan(Can *can);

    void setBuffer(Buffer *buffer);

protected:
    void run() override;

signals:

    void result(Result flag);
};


#endif //REFINE_TRANSMIT_H
