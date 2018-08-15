//
// Created by jiang.wenqiang on 2018/8/3.
//

#include "Can.h"

Can::Can(const Can::Config *config) :
        _status(Status::Disconnected), _config(config) {}

void Can::setConfig(const Can::Config *config) {
    _config = config;
}

bool Can::connect() {
    unsigned long flag = 0;
    flag = VCI_OpenDevice(_config->deviceType(),
                          _config->deviceIndex(),
                          _config->reserved());
    if (!flag) {
        return false;
    }
    flag = VCI_InitCAN(_config->deviceType(),
                       _config->deviceIndex(),
                       _config->deviceChannel(),
                       _config->initConfig());
    if (!flag) {
        return false;
    }
    flag = VCI_StartCAN(_config->deviceType(),
                        _config->deviceIndex(),
                        _config->deviceChannel());
    _status = Status::Connected;
    return (bool) flag;
}

bool Can::disconnect() {
    unsigned long flag = 0;
    flag = VCI_CloseDevice(_config->deviceType(),
                           _config->deviceIndex());
    _status = Status::Disconnected;
    return (bool) flag;
}

bool Can::reset() {
    unsigned long flag = 0;
    flag = VCI_ResetCAN(_config->deviceType(),
                        _config->deviceIndex(),
                        _config->deviceChannel());
    return bool(flag);
}

bool Can::reconnect() {
    disconnect();
    bool flag = connect();
    if (flag) {
        _status = Status::Connected;
    }
    return flag;
}

bool Can::collect(Buffer &buffer, int delay) {
    unsigned long length;
    length = VCI_Receive(_config->deviceType(),
                         _config->deviceIndex(),
                         _config->deviceChannel(),
                         buffer.head(),
                         buffer.headWholeSize(),
                         delay);
    bool flag = length < 0xFFFFFFFF;
    if (flag) {
        buffer.setHeadDataSize(length);
        buffer.headForward();
    } else {
        VCI_ERR_INFO error;
        getError(&error);
    }
    return flag;
}

bool Can::deliver(Buffer &buffer) {
    unsigned long length;
    length = VCI_Transmit(_config->deviceType(),
                          _config->deviceIndex(),
                          _config->deviceChannel(),
                          buffer.tail(),
                          buffer.tailDataSize());
    bool flag = length == buffer.tailDataSize();
    if (flag) {
        buffer.setTailDataSize(0);
        buffer.tailForward();
    }
    return flag;
}

bool Can::command(unsigned int id, QString &&cmd) {
    Q_ASSERT(id < 0x800);
    VCI_CAN_OBJ frame;
    frame.ID = id;
    frame.TimeStamp = 0;
    frame.TimeFlag = 1;
    frame.SendType = 0;
    frame.RemoteFlag = 0;
    frame.ExternFlag = 0;
    frame.DataLen = 0;

    for (int i = 0; i < 8; ++i) {
        if (i == cmd.size()) {
            frame.DataLen = (unsigned char) cmd.size();
            frame.Data[i] = '\0';
            break;
        } else {
            frame.Data[i] = (unsigned char) cmd.toStdString()[i];
        }
    }
    if (frame.DataLen == 0) {
        frame.DataLen = 8;
    }

    unsigned length;
    length = VCI_Transmit(_config->deviceType(),
                          _config->deviceIndex(),
                          _config->deviceChannel(),
                          &frame,
                          1);
    return length == frame.DataLen;
}

bool Can::isConnected() {
    VCI_BOARD_INFO info;
    unsigned long flag;
    flag = VCI_ReadBoardInfo(_config->deviceType(),
                             _config->deviceIndex(),
                             &info);
    return (bool) flag;
}

void Can::getError(PVCI_ERR_INFO error) {
    VCI_ReadErrInfo(_config->deviceType(),
                    _config->deviceIndex(),
                    _config->deviceChannel(),
                    error);
}


Can::Config::Config(unsigned long channel) :
        _device_type(4), _device_index(0), _device_channel(channel),
        _baud_rate(500),
        _config(new VCI_INIT_CONFIG{0x00000000, 0xFFFFFFFF, 0, 0, 0, 0x00,
                                    0x1C}) {}

Can::Config::~Config() {
    delete _config;
}
