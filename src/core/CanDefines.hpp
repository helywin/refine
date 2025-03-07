//
// Created by jiang.wenqiang on 2018/12/18.
//

#ifndef REFINE_CANDEFINES_HPP
#define REFINE_CANDEFINES_HPP

#include <QtCore/QDataStream>
#include "ControlCan.h"

typedef VCI_CAN_OBJ CanObj;
typedef VCI_INIT_CONFIG CanCfg;
#define CAN_OBJ_BITS ((4+4+5+8+3)*8)

namespace Cd {
    //! \brief 波特率
    enum BaudRate
    {
        BR_5Kbps = 0,       //!< 5Kbps
        BR_10Kbps = 1,      //!< 10Kbps
        BR_20Kbps = 2,      //!< 20Kbps
        BR_40Kbps = 3,      //!< 40Kbps
        BR_50Kbps = 4,      //!< 50Kbps
        BR_80Kbps = 5,      //!< 80Kbps
        BR_100Kbps = 6,     //!< 100Kbps
        BR_125Kbps = 7,     //!< 125Kbps
        BR_200Kbps = 8,     //!< 200Kbps
        BR_250Kbps = 9,     //!< 250Kbps
        BR_400Kbps = 10,    //!< 400Kbps
        BR_500Kbps = 11,    //!< 500Kbps
        BR_666Kbps = 12,    //!< 666Kbps
        BR_800Kbps = 13,    //!< 800Kbps
        BR_1000Kbps = 14    //!< 1000Kbps
    };

    //! \brief 设备类型
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

    //! \brief CAN错误码
    enum Error
    {
        CanOverflow = 0x0001,       //!< CAN控制器内部FIFO溢出
        CanErrorAlarm = 0x0002,     //!< CAN控制器错误报警
        CanPassive = 0x0004,        //!< CAN控制器消极错误
        CanLose = 0x0008,           //!< CAN控制器仲裁丢失
        CanBusError = 0x0010,       //!< CAN控制器总线错误
        CanBusOff = 0x0020,         //!< 总线关闭错误
        DeviceOpened = 0x0100,      //!< 设备已经打开
        DeviceOpen = 0x0200,        //!< 打开设备错误
        DeviceNotOpen = 0x0400,     //!< 设备没有打开
        BufferOverflow = 0x0800,    //!< 缓冲区溢出
        DeviceNotExist = 0x1000,    //!< 此设备不存在
        LoadKernelDll = 0x2000,     //!< 装载动态库失败
        CmdFailed = 0x4000,         //!< 执行命令失败错误码
        BufferCreate = 0x8000,      //!< 内存不足
        CanetePortOpened = 0x10000, //!< 端口已经被打开
        CaneteIndexUsed = 0x20000,  //!< 设备索引号已经被占用
        RefTypeId = 0x30001,        //!< SetReference或GetReference是传递的RefType是不存在
        CreateSocket = 0x30002,     //!< 创建Socket时失败
        OpenConnect = 0x30003,      //!< 打开socket的连接时失败，可能设备连接已经存在
        NoStartup = 0x30004,        //!< 设备没启动
        NoConnected = 0x30005,      //!< 设备无连接
        SendPartial = 0x30006,      //!< 只发送了部分的CAN帧
        SendTooFast = 0x30007       //!< 数据发得太快，Socket缓冲区满了
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

    //! \brief TCP
    enum Tcp
    {
        Client = 0,     //!< 客户端
        Server = 1      //!< 服务端
    };

    //! \brief 报文发送类型
    enum SendType
    {
        NormalSend = 0,           //!< 正常发送，
        SendOnce = 1,             //!< 单次发送，
        SendAndReceive = 2,       //!< 自发自收，
        SendAndReceiveOnce = 3    //!< 单次自发自收，只在为发送帧时有意义。
    };

    //! \brief 滤波类型
    enum Filter
    {
        Single = 0,                         //!< 单滤波
        Double = 1,                         //!< 双滤波
        EG20T_StandardAndExtendFrame = 0,   //!< EG20T-CAN 同时接收标准帧和扩展帧
        EG20T_StandardFrame = 1,            //!< EG20T-CAN 只接收并过滤标准帧，扩展帧不接收
        EG20T_ExtendFrame = 2               //!< EG20T-CAN 只接收并过滤扩展帧，标准帧不接收
    };

    //! \brief 模式
    enum Mode
    {
        Normal = 0,                         //!< 正常模式
        ListenOnly = 1,                     //!< 只听模式
        EG20T_Normal = 0x000,               //!< EG20T-CAN 位运算 正常模式
        EG20T_ListenOnly = 0x001,           //!< EG20T-CAN 位运算 只听模式
        EG20T_SendAndReceive = 0x010,       //!< EG20T-CAN 位运算 自发自收
        EG20T_AutoResend = 0x100            //!< EG20T-CAN 位运算 自动重发
    };


    //! \brief 用于查询波特率配置的表格
    const unsigned char BaudRateTable[15][2] = {
            {0xBF, 0xFF},    //!< 5Kbps
            {0x31, 0x1C},    //!< 10Kbps
            {0x18, 0x1C},    //!< 20Kbps
            {0x87, 0xFF},    //!< 40Kbps
            {0x09, 0x1C},    //!< 50Kbps
            {0x83, 0Xff},    //!< 80Kbps
            {0x04, 0x1C},    //!< 100Kbps
            {0x03, 0x1C},    //!< 125Kbps
            {0x81, 0xFA},    //!< 200Kbps
            {0x01, 0x1C},    //!< 250Kbps
            {0x80, 0xFA},    //!< 400Kbps
            {0x00, 0x1C},    //!< 500Kbps
            {0x80, 0xB6},    //!< 666Kbps
            {0x00, 0x16},    //!< 800Kbps
            {0x00, 0x14}     //!< 1000Kbps
    };

    enum RemoteFlag {
        RemoteFlag_False = 0,
        RemoteFlag_True = 1
    };

    enum ExternFlag {
        ExternFlag_False = 0,
        ExternFlag_True = 1
    };
};

QDataStream &operator<<(QDataStream &stream, const CanObj &obj);
QDataStream &operator>>(QDataStream &stream, CanObj &obj);

void setCanObj(CanObj &obj, unsigned int id,
               Cd::SendType send_type, const QByteArray &data, int data_len = CAN_OBJ_DATA_LEN);

#endif //REFINE_CANDEFINES_HPP
