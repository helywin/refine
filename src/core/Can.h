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

    bool board_info(CanBoardInfo &info);

    bool error_info(CanErrorInfo &error);

//    bool get_statu(CanRegStatus &status);

    bool clear();

    unsigned long cache();

    bool get_reference(unsigned char &val);

    bool set_reference();

    bool start();

    bool reset();

    bool receive_frame(CanBufferCell *receive_buffer, CanErrorInfo &error);

    bool send_frame(CanBufferCell *send_buffer);

    bool close();

    Status statu();

};


#endif //PROJECT_CAN_H
