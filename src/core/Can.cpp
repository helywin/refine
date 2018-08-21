//
// Created by jiang.wenqiang on 2018/6/28.
//

#include <QtCore/QDebug>
#include "Can.h"
#include "Log.h"
#include <QtCore/QTextStream>

Can::Can(Config &config) : config(config) {
    can_statu = Can::closed;
}

bool Can::open() {
//    if (can_statu != Can::closed) {
//        qWarning("不能在此时打开CAN盒");
//        return false;
//    }
    unsigned long flag = VCI_OpenDevice(config.device_type,
                                        config.device_index,
                                        0);
    if (flag) {
        can_statu = Can::opened;
        qInfo("打开CAN盒成功");
    } else {
        qCritical("打开CAN盒失败");
    }
    return flag == 1;
}

bool Can::init() {
//    if (can_statu != Can::opened) {
//        qWarning("不能在此时初始化CAN盒");
//        return false;
//    }
    VCI_INIT_CONFIG config_st;
    config.toStruct(config_st);
    unsigned long flag = VCI_InitCAN(config.device_type,
                                     config.device_index,
                                     config.device_channel,
                                     &config_st);
    if (flag) {
        can_statu = Can::inited;
        qInfo("初始化CAN盒成功");
    } else {
        qCritical("初始化CAN盒失败");
    }
    return flag == 1;
}

<<<<<<< HEAD
bool Can::boardInfo(BoardInfo &info) {
//    if (can_statu != Can::inited && can_statu != Can::started) {
//        qWarning("不能在此时获取CAN盒信息");
//        return false;
//    }
    VCI_BOARD_INFO info_st;
    unsigned long flag = VCI_ReadBoardInfo(config.device_type,
                                           config.device_index,
                                           &info_st);
    info = info_st;

    if (flag) {
        qInfo("读取CAN盒设备信息成功");
    } else {
        qWarning("读取CAN盒设备信息失败");
    }
    return flag == 1;
}

bool Can::errorInfo(ErrorInfo &error) {
//    if (can_statu != Can::inited && can_statu != Can::started) {
//        qWarning("不能在此时获取CAN盒错误信息");
//        return false;
//    }
    VCI_ERR_INFO error_st;
    unsigned long flag = VCI_ReadErrInfo(config.device_index,
                                         config.device_index,
                                         config.device_channel,
                                         &error_st);
    error = error_st;
    if (flag) {
        qInfo("读取CAN盒错误信息成功");
    } else {
        qWarning("读取CAN盒错误信息失败");
    }
    return flag == 1;
}

/*
bool Can::get_statu(CanRegStatus &status) {
    if (can_statu != Can::inited && can_statu != Can::started) {
        qWarning("不能在此时获取CAN盒寄存器状态");
=======
bool Can::connect() {
    VCI_CloseDevice(_config->deviceType(),
                    _config->deviceIndex());
    unsigned long flag = 0;
    flag = VCI_OpenDevice(_config->deviceType(),
                          _config->deviceIndex(),
                          _config->reserved());
    if (!flag) {
>>>>>>> 07d1fd7... 发送数据小工具写完,还需测试
        return false;
    }
    VCI_CAN_STATUS status_st;
    unsigned long flag = VCI_ReadCANStatus(_config.device_type,
                                           _config.device_index,
                                           _config.device_channel,
                                           &status_st);
    status = status_st;
    if (flag) {
        qInfo("读取CAN盒寄存器状态成功");
        return true;
    } else {
        qWarning("读取CAN盒寄存器状态失败");
        return false;
    }
<<<<<<< HEAD
=======
    clear();
    flag = VCI_StartCAN(_config->deviceType(),
                        _config->deviceIndex(),
                        _config->deviceChannel());
    _status = Status::Connected;
    return (bool) flag;
>>>>>>> 07d1fd7... 发送数据小工具写完,还需测试
}
*/
bool Can::clear() {
//    if (can_statu != Can::inited && can_statu != Can::started) {
//        qWarning("不能在此时清除CAN盒缓冲区");
//        return false;
//    }
    unsigned long flag = VCI_ClearBuffer(config.device_type,
                                         config.device_index,
                                         config.device_channel);
    if (flag) {
        qInfo("清除CAN盒缓冲区成功");
    } else {
        qWarning("清除CAN盒缓冲区失败");
    }
    return flag == 1;
}

unsigned long Can::cache() {
//    if (can_statu != Can::inited && can_statu != Can::started) {
//        qWarning("不能在此时获取CAN盒缓存的帧数");
//        return 0xFFFFFFFF;
//    }
    unsigned long cache = VCI_GetReceiveNum(config.device_type,
                                            config.device_index,
                                            config.device_channel);
    if (cache) {
        qInfo("获取CAN盒缓冲区有未被读取帧");
    }
    return cache;
}

bool Can::getReference(unsigned char &val) {
//    if (can_statu != Can::inited && can_statu != Can::started) {
//        qWarning("不能在此时获取CAN盒寄存器");
//        return false;
//    }
    unsigned long ref_type;
    switch (config.device_type) {
        case VCI_USBCAN2:
            ref_type = 1;
            break;
        default:
            ref_type = 1;
            break;
    }
    unsigned long flag = VCI_GetReference(config.device_type,
                                          config.device_index,
                                          config.device_channel,
                                          ref_type,
                                          &val);
    if (flag) {
        qInfo("读取CAN盒参数成功");
    } else {
        qWarning("读取CAN盒参数失败");
    }
    return flag == 1;
}

bool Can::setReference() {
    return false;
}

bool Can::start() {
//    if (can_statu != Can::inited) {
//        qWarning("不能在此时启动CAN盒");
//        return false;
//    }
    unsigned long flag = VCI_StartCAN(config.device_type,
                                      config.device_index,
                                      config.device_channel);
    if (flag) {
        can_statu = Can::started;
        qInfo("启动CAN盒成功");
    } else {
        qCritical("启动CAN盒失败");
    }
    return flag == 1;
}

bool Can::reset() {
//    if (can_statu != Can::started) {
//        qWarning("不能在此时复位CAN盒");
//        return false;
//    }
    unsigned long flag = VCI_ResetCAN(config.device_type,
                                      config.device_index,
                                      config.device_channel);
    if (flag) {
<<<<<<< HEAD
        can_statu = Can::inited;
        qInfo("复位CAN盒成功");
    } else {
        qCritical("复位CAN盒失败");
=======
        buffer.setTailDataSize(0);
        buffer.tailForward();
    } else {
        VCI_ERR_INFO error;
        getError(&error);
>>>>>>> 07d1fd7... 发送数据小工具写完,还需测试
    }
    return flag == 1;
}

bool Can::sendFrame(Buffer::Cell *send_buffer) {
//    if (can_statu != Can::started) {
//        qWarning("不能在此时发送CAN数据帧");
//        return false;
//    }
    if (send_buffer->length() == 0) {

    } else {
        unsigned long num = VCI_Transmit(config.device_type,
                                         config.device_index,
                                         config.device_channel,
                                         send_buffer->buffer(),
                                         send_buffer->length());
        if (num != 0) {
            qInfo("发送CAN数据帧成功");
            send_buffer->clear();
        } else {
            qInfo("发送CAN数据帧失败");
        }
        return num != 0;
    }
}

bool Can::receiveFrame(Buffer::Cell *receive_buffer, Can::ErrorInfo &error) {
//    if (can_statu != Can::started) {
//        qWarning("不能在此时接收CAN数据帧");
//        return false;
//    }
    receive_buffer->clear();
    receive_buffer->setLength(VCI_Receive(config.device_type,
                                    config.device_index,
                                    config.device_channel,
                                    receive_buffer->buffer(),
                                    receive_buffer->size(),
                                    -1));
    if (receive_buffer->length() < 0xFFFFFFFF) {
        qInfo("接收CAN数据帧成功");
    } else {
        errorInfo(error);
        qCritical("接收CAN数据帧失败");
    }
    return receive_buffer->length() != 0xFFFFFFFF;
}


bool Can::close() {
//    if (can_statu == Can::closed) {
//        qWarning("根本没打开CAN盒，跳过关闭");
//        return false;
//    }
    unsigned long flag = VCI_CloseDevice(config.device_type,
                                         config.device_index);
    if (flag) {
        can_statu = Can::closed;
        qInfo("关闭CAN盒成功");
    } else {
        qCritical("关闭CAN盒失败");
    }
    return flag == 1;
}

Can::Status Can::statu() {
    return can_statu;
}



//Config& Config::operator=(const Config &cfg) {
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

Can::Config::Config() :
        device_type(4), device_index(0), device_channel(0),
        acc_code(0x00000000), mask_code(0xFFFFFFFF), filter(0),
        mode(0), baud_rate(500), timer_0(0x00), timer_1(0x1C) {}

bool Can::Config::str(QString &s) {
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

bool Can::Config::check() {
    return true;
}

void Can::Config::toStruct(VCI_INIT_CONFIG &cfg) {
    cfg.AccCode = this->acc_code;
    cfg.AccMask = this->mask_code;
    cfg.Filter = this->filter;
    cfg.Mode = this->mode;
    cfg.Timing0 = this->timer_0;
    cfg.Timing1 = this->timer_1;
}

Can::BoardInfo::BoardInfo() :
        hardware_version(0), firmware_version(0), driver_version(0),
        interface_version(0), interrupt_number(0), can_channels(0),
        serial_number("none"), hardware_type("none"), reserved() {}

Can::BoardInfo::BoardInfo(const VCI_BOARD_INFO &info) {
    *this = info;
}

//BoardInfo& BoardInfo::operator=(const BoardInfo &info) {
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

Can::BoardInfo &Can::BoardInfo::operator=(const VCI_BOARD_INFO &info) {
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

QString Can::BoardInfo::str() {
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

<<<<<<< HEAD
void Can::BoardInfo::toStruct(VCI_BOARD_INFO &info) {
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

Can::ErrorInfo::ErrorInfo(const VCI_ERR_INFO &error) {
    *this = error;
}
=======
void Can::clear() {
    VCI_ClearBuffer(_config->deviceType(),
                    _config->deviceIndex(),
                    _config->deviceChannel());
}

Can::Config::Config(unsigned long channel) :
        _device_type(4), _device_index(0), _device_channel(channel),
        _baud_rate(500),
        _config(new VCI_INIT_CONFIG{0x00000000, 0xFFFFFFFF, 0, 0, 0x00,
                                    0x1C, 0}) {}
>>>>>>> 07d1fd7... 发送数据小工具写完,还需测试

//ErrorInfo& ErrorInfo::operator=(const ErrorInfo &error) {
//    this->error_code = error.error_code;
//    memcpy(this->passive_error_data, error.passive_error_data,
//           sizeof(error.passive_error_data));
//    this->lost_error_data = error.lost_error_data;
//    return *this;
//}

Can::ErrorInfo &Can::ErrorInfo::operator=(const VCI_ERR_INFO &error) {
    this->error_code = error.ErrCode;
    memcpy(this->passive_error_data, error.Passive_ErrData,
           sizeof(error.Passive_ErrData));
    this->lost_error_data = error.ArLost_ErrData;
    return *this;
}

QString Can::ErrorInfo::str() {
    //todo
}

void Can::ErrorInfo::toStruct(VCI_ERR_INFO &error) {
    error.ErrCode = this->error_code;
    memcpy(error.Passive_ErrData, this->passive_error_data,
           sizeof(error.Passive_ErrData));
    error.ArLost_ErrData = this->lost_error_data;
}