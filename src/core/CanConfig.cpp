//
// Created by jiang.wenqiang on 2018/6/28.
//

#include "CanConfig.h"
#include <QtCore/QTextStream>

//CanConfig& CanConfig::operator=(const CanConfig &cfg) {
//    this->device_type = cfg.device_type;
//    this->device_index = cfg.device_index;
//    this->device_channel = cfg.device_channel;
//    this->acc_code = cfg.acc_code;
//    this->mask_code = cfg.mask_code;
//    this->filter = cfg.filter;
//    this->mode = cfg.mode;
//    this->baud_rate = cfg.baud_rate;
//    this->timer_0 = cfg.timer_0;
//    this->timer_1 = cfg.timer_1;
//}

CanConfig::CanConfig() :
        device_type(4), device_index(0), device_channel(0),
        acc_code(0x00000000), mask_code(0xFFFFFFFF), filter(0),
        mode(0), baud_rate(500), timer_0(0x00), timer_1(0x1C) {}

bool CanConfig::str(QString &s) {
    s.clear();
    QTextStream stream(&s);
    stream << QString("盒类型:\t") << dec << device_type
           << QString("\n设备ID:\t") << dec << device_index
           << QString("\n通道ID:\t") << dec << device_channel
           << QString("\n接收码:\t") << L"0x";
    stream.setFieldWidth(8);
    stream.setPadChar('0');
    stream << acc_code
           << QString("\n屏蔽码:\t") << L"0x" << hex
           << mask_code
           << QString("\n滤波器:\t") << dec << filter
           << QString("\n模  式:\t") << dec << mode
           << QString("\n波特率:\t") << dec << baud_rate << L" kbps"
           << QString("\n定时 0:\t") << L"0x";
    stream.setFieldWidth(2);
    stream << hex << timer_0
           << QString("\n定时 1:\t") << L"0x" << hex
           << timer_1
           << "\n";
}

bool CanConfig::check() {
    return true;
}

void CanConfig::to_struct(VCI_INIT_CONFIG &cfg) {
    cfg.AccCode = this->acc_code;
    cfg.AccMask = this->mask_code;
    cfg.Filter = this->filter;
    cfg.Mode = this->mode;
    cfg.Timing0 = this->timer_0;
    cfg.Timing1 = this->timer_1;
}

CanBoardInfo::CanBoardInfo() :
hardware_version(0), firmware_version(0), driver_version(0),
interface_version(0), interrupt_number(0), can_channels(0),
serial_number("none"), hardware_type("none"), reserved(){}

CanBoardInfo::CanBoardInfo(const VCI_BOARD_INFO &info) {
    *this = info;
}

//CanBoardInfo& CanBoardInfo::operator=(const CanBoardInfo &info) {
//    this->hardware_version = info.hardware_version;
//    this->firmware_version = info.firmware_version;
//    this->driver_version = info.driver_version;
//    this->interface_version = info.interface_version;
//    this->interrupt_number = info.interrupt_number;
//    this->can_channels = info.can_channels;
//    memcpy(this->serial_number, info.serial_number,
//           sizeof(info.serial_number));
//    memcpy(this->hardware_type, info.hardware_type,
//           sizeof(info.hardware_type));
//    memcpy(this->reserved, info.reserved,
//           sizeof(info.reserved));
//    return *this;
//}

CanBoardInfo &CanBoardInfo::operator=(const VCI_BOARD_INFO &info) {
    this->hardware_version = info.hw_Version;
    this->firmware_version = info.fw_Version;
    this->driver_version = info.dr_Version;
    this->interface_version = info.in_Version;
    this->interrupt_number = info.irq_Num;
    this->can_channels = info.can_Num;
    memcpy(this->serial_number, info.str_Serial_Num,
           sizeof(info.str_Serial_Num));
    memcpy(this->hardware_type, info.str_hw_Type,
           sizeof(info.str_hw_Type));
    memcpy(this->reserved, info.Reserved,
           sizeof(info.Reserved));
    return *this;
}

QString CanBoardInfo::str() {
    QString s;
    s += QString("硬件版本:0x%1")
            .arg(hardware_version, 8, 16, QLatin1Char('0'));
    s += QString("\n固件版本:0x%1")
            .arg(firmware_version, 8, 16, QLatin1Char('0'));
    s += QString("\n驱动版本:0x%1")
            .arg(driver_version, 8, 16, QLatin1Char('0'));
    s += QString("\n接口版本:0x%1")
            .arg(interface_version, 8, 16, QLatin1Char('0'));
    s += QString("\n中 断 号:%1")
            .arg(interrupt_number, 5, 10, QLatin1Char(' '));
    s += QString("\n通道数目:%1")
            .arg(can_channels, 3, 10, QLatin1Char(' '));
    s += QString("\n卡序列号:");
    s += QString(serial_number);
    s += QString("\n硬件类型:");
    s += QString(hardware_type);
    s += QString("\n系统保留:%1 %2 %3 %4")
            .arg(reserved[0], 4, 16, QLatin1Char('0'))
            .arg(reserved[1], 4, 16, QLatin1Char('0'))
            .arg(reserved[2], 4, 16, QLatin1Char('0'))
            .arg(reserved[3], 4, 16, QLatin1Char('0'));
    return s;
}

void CanBoardInfo::to_struct(VCI_BOARD_INFO &info) {
    info.hw_Version = this->hardware_version;
    info.fw_Version = this->firmware_version;
    info.dr_Version = this->driver_version;
    info.in_Version = this->interface_version;
    info.irq_Num = this->interrupt_number;
    info.can_Num = this->can_channels;
    memcpy(info.str_Serial_Num, this->serial_number,
           sizeof(info.str_Serial_Num));
    memcpy(info.str_hw_Type, this->hardware_type,
           sizeof(info.str_hw_Type));
    memcpy(info.Reserved, this->reserved,
           sizeof(info.Reserved));
}

CanErrorInfo::CanErrorInfo(const VCI_ERR_INFO &error) {
    *this = error;
}

//CanErrorInfo& CanErrorInfo::operator=(const CanErrorInfo &error) {
//    this->error_code = error.error_code;
//    memcpy(this->passive_error_data, error.passive_error_data,
//           sizeof(error.passive_error_data));
//    this->lost_error_data = error.lost_error_data;
//    return *this;
//}

CanErrorInfo &CanErrorInfo::operator=(const VCI_ERR_INFO &error) {
    this->error_code = error.ErrCode;
    memcpy(this->passive_error_data, error.Passive_ErrData,
           sizeof(error.Passive_ErrData));
    this->lost_error_data = error.ArLost_ErrData;
    return *this;
}

QString CanErrorInfo::str() {
    //todo
}

void CanErrorInfo::to_struct(VCI_ERR_INFO &error) {
    error.ErrCode = this->error_code;
    memcpy(error.Passive_ErrData, this->passive_error_data,
           sizeof(error.Passive_ErrData));
    error.ArLost_ErrData = this->lost_error_data;
}