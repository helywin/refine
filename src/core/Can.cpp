//
// Created by jiang.wenqiang on 2018/9/10.
//

#include <QtCore/QDebug>
#include "Buffer.hpp"
#include "Can.hpp"

Can::Can(Can::Config *config) :
        _status(Status::Closed), _config(config) {}

void Can::setConfig(Can::Config *config)
{
    _config = config;
}

bool Can::open()
{
    unsigned long flag;
    flag = VCI_OpenDevice(_config->deviceType(),
                          _config->deviceIndex(),
                          _config->reserved());
    if (flag) {
        _status = Status::Opened;
    }
    return (bool) flag;
}

bool Can::init()
{
    unsigned long flag;
    flag = VCI_InitCAN(_config->deviceType(),
                       _config->deviceIndex(),
                       _config->deviceChannel(),
                       _config->config());
    if (flag) {
        _status = Status::Initialized;
    }
    return (bool) flag;
}

bool Can::start()
{
    unsigned long flag;
    flag = VCI_StartCAN(_config->deviceType(),
                        _config->deviceIndex(),
                        _config->deviceChannel());
    if (flag) {
        _status = Status::Started;
    }
    return (bool) flag;
}

bool Can::connect()
{
    VCI_CloseDevice(_config->deviceType(),
                    _config->deviceIndex());
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
                       _config->config());
    if (!flag) {
        return false;
    }
    clear();
    flag = VCI_StartCAN(_config->deviceType(),
                        _config->deviceIndex(),
                        _config->deviceChannel());
    return (bool) flag;
}

bool Can::close()
{
    unsigned long flag = 0;
    flag = VCI_CloseDevice(_config->deviceType(),
                           _config->deviceIndex());
    _status = Status::Closed;
    return (bool) flag;
}

bool Can::reset()
{
    unsigned long flag = 0;
    flag = VCI_ResetCAN(_config->deviceType(),
                        _config->deviceIndex(),
                        _config->deviceChannel());
    if (flag) {
        _status = Status::Initialized;
    }
    return bool(flag);
}

bool Can::reconnect()
{
    close();
    bool flag = connect();
    return flag;
}

bool Can::collect(Buffer &buffer, int delay)
{
    unsigned long length;
    _status |= Status::Collecting;
    length = VCI_Receive(_config->deviceType(),
                         _config->deviceIndex(),
                         _config->deviceChannel(),
                         buffer.head(),
                         buffer.headWholeSize(),
                         delay);
    bool flag = length < 0xFFFFFFFF && length > 0;
    if (flag) {
        buffer.setHeadDataSize(length);
        buffer.headForward();
    } else {
        VCI_ERR_INFO error;
        getError(&error);
    }
    // todo 
    _status ^= Status::Collecting;
    return flag;
}

bool Can::deliver(Buffer &buffer)
{
    unsigned long length;
    _status |= Status::Transmitting;
    length = VCI_Transmit(_config->deviceType(),
                          _config->deviceIndex(),
                          _config->deviceChannel(),
                          buffer.tail(),
                          buffer.tailDataSize());
    bool flag = length == buffer.tailDataSize();
    if (flag) {
        buffer.setTailDataSize(0);
        buffer.tailForward();
    } else {
        VCI_ERR_INFO error;
        getError(&error);
    }
    _status ^= Status::Transmitting;
    return flag;
}

bool Can::command(unsigned int id, QString &&cmd)
{
    Q_ASSERT(id < 0x800);
    _status |= Status::Command;
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
    _status ^= Status::Command;
    return length == frame.DataLen;
}

bool Can::isConnected()
{
    VCI_BOARD_INFO info;
    unsigned long flag;
    flag = VCI_ReadBoardInfo(_config->deviceType(),
                             _config->deviceIndex(),
                             &info);
    return (bool) flag;
}

void Can::getError(VCI_ERR_INFO *error)
{
    Q_ASSERT(error != nullptr);
    VCI_ReadErrInfo(_config->deviceType(),
                    _config->deviceIndex(),
                    _config->deviceChannel(),
                    error);
}

void Can::clear() const
{
    VCI_ClearBuffer(_config->deviceType(),
                    _config->deviceIndex(),
                    _config->deviceChannel());
}

int Can::status() const
{
    return _status;
}

Can::Config::Config(unsigned long channel) :
        _device_type(4), _device_index(0), _device_channel(channel),
        _baud_rate(500),
        _config(new VCI_INIT_CONFIG{0x00000000, 0xFFFFFFFF, 0, 0, 0x00,
                                    0x1C, 0})
{
    Q_ASSERT(channel == 0 || channel == 1);
}
