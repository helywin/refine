//
// Created by jiang.wenqiang on 2018/7/5.
//

#ifndef CORE_COLLECT_H
#define CORE_COLLECT_H

#include <QtCore/QThread>
#include <QtCore/QObject>
#include <QtCore/QTime>
#include "Can.h"
#include "CanBuffer.h"


class Collect : public QThread {
Q_OBJECT
public:
    enum Fail {
        Start = 1,
        Time = 2,
        Connect = 3,
    };
private:
    Can &can;

    CanBuffer &buffer;

    int minute;

    bool pause_flag;

    bool stop_flag;

//    bool reconnect_flag;

    unsigned int reconnect;

    unsigned int fail_connect;

public:
    Collect() = delete;

    Collect(Can &can, CanBuffer &buffer, int minute = 5);

protected:
    void run() Q_DECL_OVERRIDE;

public:
    void pause();

    void resume();

    void stop();

signals:

    void get();

    void fail(Fail code);

};


#endif //CORE_COLLECT_H
