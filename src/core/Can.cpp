//
// Created by jiang.wenqiang on 2018/6/28.
//

#include "Can.h"
#include "Log.h"

Can::Can(CanConfig &config) : config(config) {
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
    config.to_struct(config_st);
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

bool Can::board_info(CanBoardInfo &info) {
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

bool Can::error_info(CanErrorInfo &error) {
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
        return false;
    }
    VCI_CAN_STATUS status_st;
    unsigned long flag = VCI_ReadCANStatus(config.device_type,
                                           config.device_index,
                                           config.device_channel,
                                           &status_st);
    status = status_st;
    if (flag) {
        qInfo("读取CAN盒寄存器状态成功");
        return true;
    } else {
        qWarning("读取CAN盒寄存器状态失败");
        return false;
    }
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

bool Can::get_reference(unsigned char &val) {
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

bool Can::set_reference() {
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
        can_statu = Can::inited;
        qInfo("复位CAN盒成功");
    } else {
        qCritical("复位CAN盒失败");
    }
    return flag == 1;
}

bool Can::send_frame(CanBufferCell *send_buffer) {
//    if (can_statu != Can::started) {
//        qWarning("不能在此时发送CAN数据帧");
//        return false;
//    }
    if (send_buffer->l == 0) {

    } else {
        unsigned long num = VCI_Transmit(config.device_type,
                                         config.device_index,
                                         config.device_channel,
                                         send_buffer->buffer,
                                         send_buffer->l);
        if (num != 0) {
            qInfo("发送CAN数据帧成功");
            send_buffer->index += 1;
            send_buffer->l = 0;
        } else {
            qInfo("发送CAN数据帧失败");
        }
        return num != 0;
    }
}

bool Can::receive_frame(CanBufferCell *receive_buffer, CanErrorInfo &error) {
//    if (can_statu != Can::started) {
//        qWarning("不能在此时接收CAN数据帧");
//        return false;
//    }
    receive_buffer->l = 0;
    receive_buffer->l = VCI_Receive(config.device_type,
                                    config.device_index,
                                    config.device_channel,
                                    receive_buffer->buffer,
                                    receive_buffer->size,
                                    receive_buffer->delay);
    if (receive_buffer->l < 0xFFFFFFFF) {
        qInfo("接收CAN数据帧成功");
        receive_buffer->index += 1;
    } else {
        error_info(error);
        qCritical("接收CAN数据帧失败");
    }
    return receive_buffer->l != 0xFFFFFFFF;
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