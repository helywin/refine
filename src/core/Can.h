//
// Created by jiang.wenqiang on 2018/6/28.
//

#ifndef CORE_CAN_H
#define CORE_CAN_H

#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include "Buffer.h"


class Can {
public:
    class Config;
    class BoardInfo;
    class ErrorInfo;
    class RegStatus;

    enum Status {
        closed = 0,
        opened = 1,
        inited = 2,
        started = 3
    };
protected:
    //! \brief 工作流程的状态
    Status can_statu;

    Config &config;
public:
//    void config_str(QString &str);

    //函数
    explicit Can(Config &config);

    bool open();

    bool init();

    bool boardInfo(BoardInfo &info);

    bool errorInfo(ErrorInfo &error);

//    bool get_statu(CanRegStatus &status);

    bool clear();

    unsigned long cache();

    bool getReference(unsigned char &val);

    bool setReference();

    bool start();

    bool reset();

    bool receiveFrame(Buffer::Cell *receive_buffer, ErrorInfo &error);

    bool sendFrame(Buffer::Cell *send_buffer);

    bool close();

    Status statu();

<<<<<<< HEAD
=======
    void clear();

private:
    void getError(VCI_ERR_INFO *error);
>>>>>>> 07d1fd7... 发送数据小工具写完,还需测试
};

class Can::Config {
public:
    unsigned long device_type;      //! \brief 设备类型
    unsigned long device_index;     //! \brief 设备索引号
    unsigned long device_channel;   //! \brief 第几路
    unsigned long acc_code;         //! \brief 验收码
    unsigned long mask_code;        //! \brief 屏蔽码
    unsigned char filter;           //! \brief 滤波模式
    unsigned char mode;             //! \brief 接收模式
    unsigned int baud_rate;         //! \brief 波特率
    unsigned char timer_0;          //! \brief 根据波特率设置计时器0
    unsigned char timer_1;          //! \brief 根据波特率设置计时器1

    Config();

    bool check();

    bool generate();

    bool str(QString &s);

    Config &operator=(const Config &cfg) = default;

    void toStruct(VCI_INIT_CONFIG &cfg);
};


class Can::BoardInfo {
public:
    unsigned short hardware_version;    //! \brief 硬件版本
    unsigned short firmware_version;    //! \brief 固件版本
    unsigned short driver_version;      //! \brief 驱动版本
    unsigned short interface_version;   //! \brief 接口版本
    unsigned short interrupt_number;    //! \brief 中断号
    unsigned char can_channels;         //! \brief 几路can
    char serial_number[20];             //! \brief 序列号
    char hardware_type[40];             //! \brief 硬件类型
    unsigned short reserved[4];         //! \brief 保留位置

    BoardInfo();

    BoardInfo(const BoardInfo &info) = default;

    BoardInfo(const VCI_BOARD_INFO &info);

    BoardInfo &operator=(const BoardInfo &info) = default;

    BoardInfo &operator=(const VCI_BOARD_INFO &info);

    QString str();

    void toStruct(VCI_BOARD_INFO &info);

};

class Can::ErrorInfo {
public:
    unsigned int error_code;                //! \brief 错误码

    unsigned char passive_error_data[3];    //! \brief 消极错误

    unsigned char lost_error_data;          //! \brief 仲裁丢失错误

    ErrorInfo(const VCI_ERR_INFO &error);

    ErrorInfo() : error_code(0),
                     passive_error_data("00"),
                     lost_error_data('0') {};

    ErrorInfo &operator=(const ErrorInfo &error) = default;

    ErrorInfo &operator=(const VCI_ERR_INFO &error);

    QString str();

    void toStruct(VCI_ERR_INFO &error);
};


class Can::RegStatus {
public:
    unsigned char error_interrupt;      //! \brief 中断记录
    unsigned char reg_mode;             //! \brief 控制模式
    unsigned char reg_status;           //! \brief 控制状态
    unsigned char reg_al_capture;       //! \brief 仲裁丢失
    unsigned char reg_ecc_apture;       //! \brief 错误寄存器
    unsigned char reg_ew_limit;         //! \brief 错误警告限制
    unsigned char reg_re_counter;       //! \brief 接收错误
    unsigned char reg_te_counter;       //! \brief 发送错误
    unsigned long reserved;             //! \brief 保留字

    RegStatus(VCI_CAN_STATUS &status);
    RegStatus &operator=(const RegStatus &reg) = default;
    RegStatus &operator=(const VCI_CAN_STATUS &reg);
    void str(QString &s);
    void toStruct(VCI_CAN_STATUS &status);
};

<<<<<<< HEAD
#endif //PROJECT_CAN_H
=======



#endif //REFINE_CAN_H
>>>>>>> 07d1fd7... 发送数据小工具写完,还需测试
