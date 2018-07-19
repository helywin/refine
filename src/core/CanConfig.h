//
// Created by jiang.wenqiang on 2018/6/28.
//

#ifndef CORE_CANCONFIG_H
#define CORE_CANCONFIG_H

#include "ControlCan.h"
#include <QtCore/QString>

class CanConfig {
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

    CanConfig();

    bool check();

    bool generate();

    bool str(QString &s);

    CanConfig &operator=(const CanConfig &cfg) = default;

    void to_struct(VCI_INIT_CONFIG &cfg);
};


class CanBoardInfo {
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

    CanBoardInfo();

    CanBoardInfo(const CanBoardInfo &info) = default;

    CanBoardInfo(const VCI_BOARD_INFO &info);

    CanBoardInfo &operator=(const CanBoardInfo &info) = default;

    CanBoardInfo &operator=(const VCI_BOARD_INFO &info);

    QString str();

    void to_struct(VCI_BOARD_INFO &info);

};

class CanErrorInfo {
public:
    unsigned int error_code;                //! \brief 错误码

    unsigned char passive_error_data[3];    //! \brief 消极错误

    unsigned char lost_error_data;          //! \brief 仲裁丢失错误

    CanErrorInfo(const VCI_ERR_INFO &error);

    CanErrorInfo() : error_code(0),
                     passive_error_data("00"),
                     lost_error_data('0') {};

    CanErrorInfo &operator=(const CanErrorInfo &error) = default;

    CanErrorInfo &operator=(const VCI_ERR_INFO &error);

    QString str();

    void to_struct(VCI_ERR_INFO &error);
};

/*
class CanRegStatus {
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

    CanRegStatus(VCI_CAN_STATUS &status);
    CanRegStatus &operator=(const CanRegStatus &reg);
    CanRegStatus &operator=(const VCI_CAN_STATUS &reg);
    void str(QString &s);
    void to_struct(VCI_CAN_STATUS &status);
};
*/


#endif //CORE_CANCONFIG_H
