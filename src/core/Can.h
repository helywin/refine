//
// Created by jiang.wenqiang on 2018/6/28.
//

#ifndef CORE_CAN_H
#define CORE_CAN_H

#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include "CanConfig.h"
#include "CanBuffer.h"


class Can {
public:
    enum Status {
        closed = 0,
        opened = 1,
        inited = 2,
        started = 3
    };
protected:
    //! \brief 工作流程的状态
    Status can_statu;

    CanConfig &config;
public:
//    void config_str(QString &str);

    //函数
    explicit Can(CanConfig &config);

    bool open();

    bool init();

    bool boardInfo(CanBoardInfo &info);

    bool errorInfo(CanErrorInfo &error);

//    bool get_statu(CanRegStatus &status);

    bool clear();

    unsigned long cache();

    bool getReference(unsigned char &val);

    bool setReference();

    bool start();

    bool reset();

    bool receiveFrame(CanBufferCell *receive_buffer, CanErrorInfo &error);

    bool sendFrame(CanBufferCell *send_buffer);

    bool close();

    Status statu();

};


#endif //PROJECT_CAN_H
