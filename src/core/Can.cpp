/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/10
 * @brief CAN控制类
 * @details CAN的控制和状态量
 ******************************************************************************/

#include <QtCore/QDebug>
#include "RecvBuffer.hpp"
#include "SendBuffer.hpp"
#include "Can.hpp"


/*!
 * @brief 配置波特率
 * @param rate 波特率
 */
void Can::Config::setBaudRate(Cd::BaudRate rate)
{
    _baud_rate = rate;
    _config.Timing0 = Cd::BaudRateTable[_baud_rate][0];
    _config.Timing1 = Cd::BaudRateTable[_baud_rate][1];
}

/*!
 * @brief 构造函数
 */
Can::Can(Message *message) :
        Message(message)
{}

/*!
 * @brief 打开CAN
 * @return 是否成功
 */
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
        return false;
    }
}

/*!
 * @brief 初始化CAN
 * @return 是否成功
 */
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
        return false;
    }
}

/*!
 * @brief 启动CAN
 * @return 是否成功
 */
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
        return false;
    }
}

/*!
 * @brief 一键启动CAN
 * @return 是否成功
 */
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

/*!
 * @brief 关闭CAN
 * @return 是否成功
 */
bool Can::close()
{
    unsigned long flag;
    flag = VCI_CloseDevice(_config.deviceType(),
                           _config.deviceIndex());
    if (flag) {
        _status = Status::Closed;
        return true;
    } else {
        return false;
    }
}

/*!
 * @brief 复位CAN
 * @return 是否成功
 */
bool Can::reset()
{
    unsigned long flag;
    flag = VCI_ResetCAN(_config.deviceType(),
                        _config.deviceIndex(),
                        _config.deviceChannel());
    if (flag) {
        _status = Status::Initialized;
        return true;
    } else {
        return false;
    }
}

/*!
 * @brief 重连CAN
 * @return 是否成功
 */
bool Can::reconnect()
{
    close();
    return connect();
}

/*!
 * @brief CAN采集报文
 * @param buffer 接收缓冲区
 * @param delay 延时
 * @return 采集状态
 * @see Can::Result
 */
int Can::collect(RecvBuffer &buffer, const int delay)
{
    unsigned long length;
    _status |= Status::Collecting;
    length = VCI_Receive(_config.deviceType(),
                         _config.deviceIndex(),
                         _config.deviceChannel(),
                         buffer.headCell().obj(),
                         buffer.headCell().wholeSize(),
                         delay);
    int rtn;
    if (length > 0 && length != 0xFFFFFFFF) {
        buffer.headCell().setDataSize(length);
        buffer.move();
        rtn = Succeed;
    } else {
        buffer.headCell().setDataSize(0);   //改为空报文包还是接收
        buffer.move();
        rtn = Empty;
    }
    _status ^= Status::Collecting;
    return rtn;
}

/*!
 * @brief 发送报文
 * @param buffer 发送缓冲区
 * @return 是否发送成功
 */
bool Can::deliver(SendBuffer &buffer, unsigned long num)
{
    unsigned long length;
    _status |= Status::Transmitting;
    length = VCI_Transmit(_config.deviceType(),
                          _config.deviceIndex(),
                          _config.deviceChannel(),
                          &*(buffer.begin()),
                          num);
    bool flag = length == num;
    if (flag) {
        buffer.move((int) num);
    } else {
        getError();
    }
    _status ^= Status::Transmitting;
    return flag;
}

/*!
 * @deprecated
 * @brief 发送字符串
 * @param id CAN总线ID
 * @param cmd 字符串
 * @return 是否发送成功
 */
/*
bool Can::command(const unsigned int id, const QByteArray &cmd)
{
    Q_ASSERT(id < 0x800);
    _status |= Status::Command;
    CanObj frame = canObj(id, Cd::NormalSend, cmd);
*/
/*
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
*//*


    unsigned length;
    length = VCI_Transmit(_config.deviceType(),
                          _config.deviceIndex(),
                          _config.deviceChannel(),
                          &frame,
                          1);
    _status ^= Status::Command;
    return length == frame.DataLen;
}
*/

/*!
 * @brief 判断是否连接
 * @return 是否连接
 */
bool Can::isConnected()
{
    VCI_BOARD_INFO info;
    unsigned long flag;
    flag = VCI_ReadBoardInfo(_config.deviceType(),
                             _config.deviceIndex(),
                             &info);
    if (!flag) {
        getError();
//        reportError();
    }
    return (bool) flag;
}

/*!
 * @brief 读取错误标志
 */
int Can::getError()
{
    VCI_ReadErrInfo(_config.deviceType(),
                    _config.deviceIndex(),
                    _config.deviceChannel(),
                    &_error_info.error());
    return _error_info.errorCode();
}

/*!
 * @brief 清空缓冲区
 */
void Can::clear() const
{
    VCI_ClearBuffer(_config.deviceType(),
                    _config.deviceIndex(),
                    _config.deviceChannel());
}

/*!
 * @brief 返回当前CAN状态
 * @return 状态
 * @deprecated
 */
int Can::status() const
{
    return _status;
}

void Can::reportError()
{
    QString str;
    switch (_error_info.errorCode()) {
        case Cd::Error::CanOverflow :
            str = QString("CAN控制器内部FIFO溢出");
            break;
        case Cd::Error::CanErrorAlarm :
            str = QString("CAN控制器错误报警");
            break;
        case Cd::Error::CanPassive :
            str = QString("CAN控制器消极错误");
            break;
        case Cd::Error::CanLose :
            str = QString("CAN控制器仲裁丢失");
            break;
        case Cd::Error::CanBusError :
            str = QString("CAN控制器总线错误");
            break;
        case Cd::Error::CanBusOff :
            str = QString("总线关闭错误");
            break;
        case Cd::Error::DeviceOpened :
            str = QString("设备已经打开");
            break;
        case Cd::Error::DeviceOpen :
            str = QString("打开设备错误");
            break;
        case Cd::Error::DeviceNotOpen :
            str = QString("设备没有打开");
            break;
        case Cd::Error::BufferOverflow :
            str = QString("缓冲区溢出");
            break;
        case Cd::Error::DeviceNotExist :
            str = QString("此设备不存在");
            break;
        case Cd::Error::LoadKernelDll :
            str = QString("装载动态库失败");
            break;
        case Cd::Error::CmdFailed :
            str = QString("执行命令失败");
            break;
        case Cd::Error::BufferCreate :
            str = QString("内存不足");
            break;
        case Cd::Error::CanetePortOpened :
            str = QString("端口已经被打开");
            break;
        case Cd::Error::CaneteIndexUsed :
            str = QString("设备索引号已经被占用");
            break;
        case Cd::Error::RefTypeId :
            str = QString("SetReference或GetReference是传递的RefType是不存在");
            break;
        case Cd::Error::CreateSocket :
            str = QString("创建Socket时失败");
            break;
        case Cd::Error::OpenConnect :
            str = QString("打开socket的连接时失败，可能设备连接已经存在");
            break;
        case Cd::Error::NoStartup :
            str = QString("设备没启动");
            break;
        case Cd::Error::NoConnected :
            str = QString("设备无连接");
            break;
        case Cd::Error::SendPartial :
            str = QString("只发送了部分的CAN帧");
            break;
        case Cd::Error::SendTooFast :
            str = QString("数据发得太快，Socket缓冲区满了");
            break;
        default:
            str = QString("没有错误码，也可能没启动CAN");
            break;
    }
    emitMessage(Re::Warning, str);
}

int Can::receivedNumber()
{
    return static_cast<int>(VCI_GetReceiveNum(_config.deviceType(),
                                              _config.deviceIndex(),
                                              _config.deviceChannel()));
}

bool Can::updateConfig()
{
    if (isConnected()) {
        return false;
    } else {
        _config = _config_latest;
        return true;
    }
}
