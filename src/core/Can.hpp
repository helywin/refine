/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/10
 * @brief CAN控制类
 * @details CAN的控制和状态量
 ******************************************************************************/

#ifndef REFINE_CAN_HPP
#define REFINE_CAN_HPP


#include <QtCore/QString>
#include "ControlCan.h"
#include "Log.hpp"

class Buffer;

/*!
 * @brief 控制CAN的类
 */

class Can
{
public:
public:
    enum BaudRate
    {
        BR_5Kbps = 0,
        BR_10Kbps = 1,
        BR_20Kbps = 2,
        BR_40Kbps = 3,
        BR_50Kbps = 4,
        BR_80Kbps = 5,
        BR_100Kbps = 6,
        BR_125Kbps = 7,
        BR_200Kbps = 8,
        BR_250Kbps = 9,
        BR_400Kbps = 10,
        BR_500Kbps = 11,
        BR_666Kbps = 12,
        BR_800Kbps = 13,
        BR_1000Kbps = 14
    };

    enum DeviceType
    {
        PCI5121 = 1,
        PCI9810 = 2,
        USBCAN1 = 3,
        USBCAN2 = 4,
        USBCAN2A = 4,
        PCI9820 = 5,
        CAN232 = 6,
        PCI5110 = 7,
        CANLITE = 8,
        ISA9620 = 9,
        ISA5420 = 10,
        PC104CAN = 11,
        CANETUDP = 12,
        CANETE = 12,
        DNP9810 = 13,
        PCI9840 = 14,
        PC104CAN2 = 15,
        PCI9820I = 16,
        CANETTCP = 17,
        PEC9920 = 18,
        PCI5010U = 19,
        USBCAN_E_U = 20,
        USBCAN_2E_U = 21,
        PCI5020U = 22,
        EG2OT_CAN = 23
    };
    enum Error : unsigned int
    {
        CanOverflow = 0x0001,       //! \brief CAN控制器内部FIFO溢出
        CanErrorAlarm = 0x0002,     //! \brief CAN控制器错误报警
        CanPassive = 0x0004,        //! \brief CAN控制器消极错误
        CanLose = 0x0008,           //! \brief CAN控制器仲裁丢失
        CanBusError = 0x0010,       //! \brief CAN控制器总线错误
        CanBusOff = 0x0020,         //! \brief 总线关闭错误
        DeviceOpened = 0x0100,      //! \brief 设备已经打开
        DeviceOpen = 0x0200,        //! \brief 打开设备错误
        DeviceNotOpen = 0x0400,     //! \brief 设备没有打开
        BufferOverflow = 0x0800,    //! \brief 缓冲区溢出
        DeviceNotExist = 0x1000,    //! \brief 此设备不存在
        LoadKernelDll = 0x2000,     //! \brief 装载动态库失败
        CmdFailed = 0x4000,         //! \brief 执行命令失败错误码
        BufferCreate = 0x8000,      //! \brief 内存不足
        CanetePortOpened = 0x10000, //! \brief 端口已经被打开
        CaneteIndexUsed = 0x20000,  //! \brief 设备索引号已经被占用
        RefTypeId = 0x30001,        //! \brief SetReference 或GetReference是传递的RefType 是不存在
        CreateSocket = 0x30002,     //! \brief 创建Socket 时失败
        OpenConnect = 0x30003,      //! \brief 打开socket 的连接时失败, 可能设备连接已经存在
        NoStartup = 0x30004,        //! \brief 设备没启动
        NoConnected = 0x30005,      //! \brief 设备无连接
        SendPartial = 0x30006,      //! \brief 只发送了部分的CAN 帧
        SendTooFast = 0x30007       //! \brief 数据发得太快，Socket 缓冲区满了
    };

    enum Cmd
    {
        DestinationIp = 0,
        DestinationPort = 1,
        ChangeDestinationIpAndPort = 2,
        SourcePort = 2,
        TcpType = 4,
        ClientCount = 5,
        Clients = 6,
        DisconnectClient = 7
    };

    enum Tcp
    {
        Client = 0,
        Server = 1
    };

    enum SendType : unsigned char
    {
        NormalSend = 0,           //! \brief 正常发送，
        SendOnce = 1,             //! \brief 单次发送，
        SendAndReceive = 2,       //! \brief 自发自收，
        SendAndReceiveOnce = 3    //! \brief 单次自发自收，只在为发送帧时有意义。
    };

    enum Filter : unsigned char
    {
        Single = 0,     //! \brief 单滤波
        Double = 1,     //! \brief 双滤波
        EG20T_StandardAndExtendFrame = 0,   //! \brief EG20T-CAN 同时接收标准帧和扩展帧
        EG20T_StandardFrame = 1,    //! \brief EG20T-CAN 只接收并过滤标准帧，扩展帧不接收
        EG20T_ExtendFrame = 2       //! \brief EG20T-CAN 只接收并过滤扩展帧，标准帧不接收
    };

    enum Mode : unsigned char
    {
        Normal = 0,     //! \brief 正常模式
        ListenOnly = 1, //! \brief 只听模式
        EG20T_Normal = 0x000,       //! \brief EG20T-CAN 位运算 正常模式
        EG20T_ListenOnly = 0x001,   //! \brief EG20T-CAN 位运算 只听模式
        EG20T_SendAndReceive = 0x010,    //! \brief EG20T-CAN 位运算 自发自收
        EG20T_AutoResend = 0x100    //! \brief EG20T-CAN 位运算 自动重发
    };

    enum Status
    {
        Closed = 0x00,
        Opened = 0x01,
        Initialized = 0x02,
        Started = 0x04,
        Collecting = 0x08,
        Transmitting = 0x10,
        Command = 0x20
    };

    enum Result
    {
        Fail = -1,
        Empty = 0,
        Succeed = 1
    };


    class Config
    {
    public:
        const unsigned char BaudRateTable[15][2] = {
                {0xBF, 0xFF},
                {0x31, 0x1C},
                {0x18, 0x1C},
                {0x87, 0xFF},
                {0x09, 0x1C},
                {0x83, 0Xff},
                {0x04, 0x1C},
                {0x03, 0x1C},
                {0x81, 0xFA},
                {0x01, 0x1C},
                {0x80, 0xFA},
                {0x00, 0x1C},
                {0x80, 0xB6},
                {0x00, 0x16},
                {0x00, 0x14}
        };
    private:
        unsigned long _device_type;      //! \brief 设备类型
        unsigned long _device_index;     //! \brief 设备索引号
        unsigned long _device_channel;   //! \brief 第几路
        unsigned int _baud_rate;         //! \brief 波特率
        VCI_INIT_CONFIG _config;
    public:
        explicit Config(unsigned long channel = 0);
        Config(const Config &config) = delete;
        Config &operator=(const Config &config) = delete;

        inline unsigned long deviceType() const { return _device_type; }

        inline unsigned long deviceIndex() const { return _device_index; }

        inline unsigned long deviceChannel() const { return _device_channel; }

        inline unsigned long reserved() const { return _config.Reserved; }

        inline const VCI_INIT_CONFIG &config() const { return _config; }

        inline VCI_INIT_CONFIG &config() { return _config; }

        inline void
        setDeviceType(unsigned long type) { _device_channel = type; }

        inline void
        setDeviceIndex(unsigned long index) { _device_index = index; }

        inline void
        setDeviceChannel(unsigned long channel) { _device_channel = channel; }

        void setBaudRate(BaudRate rate);

        inline void
        setAccCode(unsigned long acc_code) { _config.AccCode = acc_code; }

        inline void
        setAccMask(unsigned long acc_mask) { _config.AccMask = acc_mask; }

        inline void
        setReserved(unsigned long reserved) { _config.Reserved = reserved; }

        inline void
        setFilter(unsigned char filter) { _config.Filter = filter; }

        inline void setMode(unsigned char mode) { _config.Mode = mode; }
    };

    class ErrorInfo
    {
    private:
        VCI_ERR_INFO _error;
    public:
        ErrorInfo() = default;

        inline VCI_ERR_INFO &error() { return _error; }

        inline const VCI_ERR_INFO &error() const { return _error; }

        int report() const;

        inline int errorCode() const { return _error.ErrCode; }
    };

private:
    int _status;
    Config _config;
    ErrorInfo _error_info;

public:
    explicit Can();

    inline Config &config() { return _config; }

    inline const Config &config() const { return _config; }

    bool open();
    bool init();
    bool start();
    bool connect();
    bool close();
    bool reset();
    bool reconnect();
    int collect(Buffer &buffer, int delay = 0);
    bool deliver(Buffer &buffer);
    bool command(unsigned int id, const QString &cmd);

    inline bool command(unsigned int id, QString &&cmd)
    {
        return command(id, cmd);
    }

    bool isConnected();
    void clear() const;
    int status() const;

private:
    void getError();
};


#endif //REFINE_CAN_HPP
