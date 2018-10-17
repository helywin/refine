/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/10
 * @brief CAN控制类
 * @details CAN的控制和状态量
 ******************************************************************************/

#include <QtCore/QDebug>
#include "Buffer.hpp"
#include "Can.hpp"

Can::Can() :
        _status(Status::Closed), _config(), _error_info() {}

bool Can::open()
{
    unsigned long flag;
    flag = VCI_OpenDevice(_config.deviceType(),
                          _config.deviceIndex(),
                          _config.reserved());
    if (flag) {
        _status = Status::Opened;
        return true;
    } else {
        getError();
        if (_error_info.errorCode() == Error::DeviceOpened) {
            return true;
        } else {
            _error_info.report();
            return false;
        }
    }
}

bool Can::init()
{
    unsigned long flag;
    flag = VCI_InitCAN(_config.deviceType(),
                       _config.deviceIndex(),
                       _config.deviceChannel(),
                       &_config.config());
    if (flag) {
        _status = Status::Initialized;
        return true;
    } else {
        getError();
        return false;
    }
}

bool Can::start()
{
    unsigned long flag;
    flag = VCI_StartCAN(_config.deviceType(),
                        _config.deviceIndex(),
                        _config.deviceChannel());
    if (flag) {
        _status = Status::Started;
        return true;
    } else {
        getError();
        return false;
    }
}

bool Can::connect()
{
    bool flag = open();
    if (!flag) {
        return false;
    }
    flag = init();
    if (!flag) {
        return false;
    }
    flag = start();
    clear();
    return flag;
}

bool Can::close()
{
    unsigned long flag = 0;
    flag = VCI_CloseDevice(_config.deviceType(),
                           _config.deviceIndex());
    _status = Status::Closed;
    if (!flag) {
        getError();
        _error_info.report();
        return false;
    }
    return true;
}

bool Can::reset()
{
    unsigned long flag = 0;
    flag = VCI_ResetCAN(_config.deviceType(),
                        _config.deviceIndex(),
                        _config.deviceChannel());
    if (!flag) {
        getError();
        _error_info.report();
        return false;
    } else {
        _status = Status::Initialized;
        return true;
    }
}

bool Can::reconnect()
{
    close();
    return connect();
}

int Can::collect(Buffer &buffer, const int delay)
{
    unsigned long length;
    _status |= Status::Collecting;
    length = VCI_Receive(_config.deviceType(),
                         _config.deviceIndex(),
                         _config.deviceChannel(),
                         buffer.head().obj(),
                         buffer.headWholeSize(),
                         delay);
    int rtn;
    if (length > 0 && length != 0xFFFFFFFF) {
        buffer.setHeadDataSize(length);
        buffer.headForward();
        rtn = Succeed;
    } else if (length == 0xFFFFFFFF) {
        getError();
        _error_info.report();
        rtn = Fail;
    } else {
        rtn = Empty;
    }
    _status ^= Status::Collecting;
    return rtn;
}

bool Can::deliver(Buffer &buffer)
{
    unsigned long length;
    _status |= Status::Transmitting;
    length = VCI_Transmit(_config.deviceType(),
                          _config.deviceIndex(),
                          _config.deviceChannel(),
                          buffer.tail().obj(),
                          buffer.tailDataSize());
    bool flag = length == buffer.tailDataSize();
    if (flag) {
        buffer.setTailDataSize(0);
        buffer.tailForward();
    } else {
        getError();
        _error_info.report();
    }
    _status ^= Status::Transmitting;
    return flag;
}

bool Can::command(const unsigned int id, const QString &cmd)
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
    length = VCI_Transmit(_config.deviceType(),
                          _config.deviceIndex(),
                          _config.deviceChannel(),
                          &frame,
                          1);
    _status ^= Status::Command;
    return length == frame.DataLen;
}

bool Can::isConnected()
{
    VCI_BOARD_INFO info;
    unsigned long flag;
    flag = VCI_ReadBoardInfo(_config.deviceType(),
                             _config.deviceIndex(),
                             &info);
    return (bool) flag;
}

void Can::getError()
{
    VCI_ReadErrInfo(_config.deviceType(),
                    _config.deviceIndex(),
                    _config.deviceChannel(),
                    &_error_info.error());
}

void Can::clear() const
{
    VCI_ClearBuffer(_config.deviceType(),
                    _config.deviceIndex(),
                    _config.deviceChannel());
}

int Can::status() const
{
    return _status;
}

Can::Config::Config(unsigned long channel) :
        _device_type(DeviceType::USBCAN2),
        _device_index(0),
        _device_channel(channel),
        _baud_rate(BaudRate::BR_500Kbps),
        _config({0x00000000, 0xFFFFFFFF, 0, 0, 0x00, 0x1C, 0})
{
    Q_ASSERT(channel == 0 || channel == 1);
}

void Can::Config::setBaudRate(BaudRate rate)
{
    _baud_rate = rate;
    _config.Timing0 = BaudRateTable[_baud_rate][0];
    _config.Timing1 = BaudRateTable[_baud_rate][1];
}

int Can::ErrorInfo::report() const
{
    QString str;
    switch (_error.ErrCode) {
        case Error::CanOverflow :
            str = QString("CAN控制器内部FIFO溢出");
            break;
        case Error::CanErrorAlarm :
            str = QString("CAN控制器错误报警");
            break;
        case Error::CanPassive :
            str = QString("CAN控制器消极错误");
            break;
        case Error::CanLose :
            str = QString("CAN控制器仲裁丢失");
            break;
        case Error::CanBusError :
            str = QString("CAN控制器总线错误");
            break;
        case Error::CanBusOff :
            str = QString("总线关闭错误");
            break;
        case Error::DeviceOpened :
            str = QString("设备已经打开");
            break;
        case Error::DeviceOpen :
            str = QString("打开设备错误");
            break;
        case Error::DeviceNotOpen :
            str = QString("设备没有打开");
            break;
        case Error::BufferOverflow :
            str = QString("缓冲区溢出");
            break;
        case Error::DeviceNotExist :
            str = QString("此设备不存在");
            break;
        case Error::LoadKernelDll :
            str = QString("装载动态库失败");
            break;
        case Error::CmdFailed :
            str = QString("执行命令失败错误码");
            break;
        case Error::BufferCreate :
            str = QString("内存不足");
            break;
        case Error::CanetePortOpened :
            str = QString("端口已经被打开");
            break;
        case Error::CaneteIndexUsed :
            str = QString("设备索引号已经被占用");
            break;
        case Error::RefTypeId :
            str = QString("SetReference 或GetReference是传递的RefType 是不存在");
            break;
        case Error::CreateSocket :
            str = QString("创建Socket时失败");
            break;
        case Error::OpenConnect :
            str = QString("打开socket 的连接时失败, 可能设备连接已经存在");
            break;
        case Error::NoStartup :
            str = QString("设备没启动");
            break;
        case Error::NoConnected :
            str = QString("设备无连接");
            break;
        case Error::SendPartial :
            str = QString("只发送了部分的CAN帧");
            break;
        case Error::SendTooFast :
            str = QString("数据发得太快，Socket 缓冲区满了");
            break;
        default:
            str = QString("不在错误码范围内，没有错误");
            break;
    }
    qWarning() << str;
    return _error.ErrCode;
}
