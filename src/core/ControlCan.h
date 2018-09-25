//
// Created by jiang.wenqiang on 2018/9/10.
//

#ifndef REFINE_CONTROLCAN_H
#define REFINE_CONTROLCAN_H

//接口卡类型定义
#define VCI_PCI5121        1
#define VCI_PCI9810        2
#define VCI_USBCAN1        3
#define VCI_USBCAN2        4
#define VCI_USBCAN2A       4
#define VCI_PCI9820        5
#define VCI_CAN232         6
#define VCI_PCI5110        7
#define VCI_CANLITE        8
#define VCI_ISA9620        9
#define VCI_ISA5420        10
#define VCI_PC104CAN       11
#define VCI_CANETUDP       12
#define VCI_CANETE         12
#define VCI_DNP9810        13
#define VCI_PCI9840        14
#define VCI_PC104CAN2      15
#define VCI_PCI9820I       16
#define VCI_CANETTCP       17
#define VCI_PEC9920        18
#define VCI_PCI5010U       19
#define VCI_USBCAN_E_U     20
#define VCI_USBCAN_2E_U    21
#define VCI_PCI5020U       22
#define VCI_EG2OT_CAN      23

//CAN错误码
#define    ERR_CAN_OVERFLOW            0x0001    //CAN控制器内部FIFO溢出
#define    ERR_CAN_ERRALARM            0x0002    //CAN控制器错误报警
#define    ERR_CAN_PASSIVE             0x0004    //CAN控制器消极错误
#define    ERR_CAN_LOSE                0x0008    //CAN控制器仲裁丢失
#define    ERR_CAN_BUSERR              0x0010    //CAN控制器总线错误
#define    ERR_CAN_BUSOFF              0x0020    //总线关闭错误
//通用错误码
#define    ERR_DEVICEOPENED            0x0100    //设备已经打开
#define    ERR_DEVICEOPEN              0x0200    //打开设备错误
#define    ERR_DEVICENOTOPEN           0x0400    //设备没有打开
#define    ERR_BUFFEROVERFLOW          0x0800    //缓冲区溢出
#define    ERR_DEVICENOTEXIST          0x1000    //此设备不存在
#define    ERR_LOADKERNELDLL           0x2000    //装载动态库失败
#define    ERR_CMDFAILED               0x4000    //执行命令失败错误码
#define    ERR_BUFFERCREATE            0x8000    //内存不足


//函数调用返回状态值
#define    STATUS_OK                    1
#define STATUS_ERR                      0

#define CMD_DESIP              0
#define CMD_DESPORT            1
#define CMD_CHGDESIPANDPORT    2
#define CMD_SRCPORT            2
#define CMD_TCP_TYPE           4          //tcp 工作方式，服务器:1 或是客户端:0
#define TCP_CLIENT             0
#define TCP_SERVER             1
//服务器方式下有效
#define CMD_CLIENT_COUNT       5          //连接上的客户端计数
#define CMD_CLIENTS            6          //连接上的客户端
#define CMD_DISCONN_CLINET     7          //断开一个连接

//! \brief 添加基础类型 by jiang.wenqiang
//! \date 2018/6/15
typedef unsigned long DWORD_T;
typedef unsigned long ULONG_T;
typedef signed long LONG_T;
typedef unsigned long HANDLE_T;
typedef unsigned short USHORT_T;
typedef signed short SHORT_T;
typedef unsigned char BYTE_T;
typedef unsigned int UINT_T;
typedef signed int INT_T;
typedef unsigned char UCHAR_T;
typedef signed char CHAR_T;
typedef void *PVOID_T;


typedef struct
{//tagRemoteClient {
    int iIndex;
    DWORD_T port;
    HANDLE_T hClient;
    char szip[32];
} REMOTE_CLIENT;


//! \brief 1.ZLGCAN系列接口卡信息的数据类型。
typedef struct
{//_VCI_BOARD_INFO {
    USHORT_T hw_Version;                //! \brief hardware version
    USHORT_T fw_Version;                //! \brief firmware version
    USHORT_T dr_Version;                //! \brief driver version
    USHORT_T in_Version;                //! \brief interface version
    USHORT_T irq_Num;                   //! \brief interrupt number
    BYTE_T can_Num;                     //! \brief can channels number
    CHAR_T str_Serial_Num[20];          //! \brief serial number
    CHAR_T str_hw_Type[40];             //! \brief hardware type
    USHORT_T Reserved[4];               //! \brief reserved
} VCI_BOARD_INFO;//, *PVCI_BOARD_INFO;

//! \brief 2.定义CAN信息帧的数据类型。
typedef struct
{//_VCI_CAN_OBJ {
    UINT_T ID;                            //! \brief 报文id
    UINT_T TimeStamp;                     //! \brief 收到信息帧时的时间标识
    BYTE_T TimeFlag;                      //! \brief 是否使用时间标识
    BYTE_T SendType;                      //! \brief 送帧类型
    //!  =0 正常发送，
    //!  =1 单次发送，
    //!  =2 自发自收，
    //!  =3 单次自发自收，只在为发送帧时有意义。
    BYTE_T RemoteFlag;                    //! \brief 是否是远程帧
    BYTE_T ExternFlag;                    //! \brief 是否是扩展帧
    BYTE_T DataLen;                       //! \brief 数据长度(<=8)，即Data的长度
    BYTE_T Data[8];                      //! \brief 报文数据
    BYTE_T Reserved[3];                   //! \brief 保留字
} VCI_CAN_OBJ;//, *PVCI_CAN_OBJ;

//! \brief 3.定义CAN控制器状态的数据类型。
typedef struct
{//_VCI_CAN_STATUS {
    UCHAR_T ErrInterrupt;
    UCHAR_T regMode;
    UCHAR_T regStatus;
    UCHAR_T regALCapture;
    UCHAR_T regECCapture;
    UCHAR_T regEWLimit;
    UCHAR_T regRECounter;
    UCHAR_T regTECounter;
    DWORD_T Reserved;
} VCI_CAN_STATUS;//, *PVCI_CAN_STATUS;

//! \brief 4.定义错误信息的数据类型。
typedef struct
{//_VCI_ERR_INFO {
    UINT_T ErrCode;
    BYTE_T Passive_ErrData[3];
    BYTE_T ArLost_ErrData;
} VCI_ERR_INFO;//, *PVCI_ERR_INFO;

//! \brief 5.定义初始化CAN的数据类型
typedef struct
{//_VCI_INIT_CONFIG {
    DWORD_T AccCode;
    DWORD_T AccMask;
    DWORD_T Reserved;
    UCHAR_T Filter;
    UCHAR_T Timing0;
    UCHAR_T Timing1;
    UCHAR_T Mode;
} VCI_INIT_CONFIG;//, *PVCI_INIT_CONFIG;

typedef struct
{//_tagChgDesIPAndPort {
    char szpwd[10];
    char szdesip[20];
    int desport;
    BYTE_T blistenonly;
} CHGDESIPANDPORT;

///////// new add struct for filter /////////
typedef struct
{//_VCI_FILTER_RECORD {
    DWORD_T ExtFrame;    //是否为扩展帧
    DWORD_T Start;
    DWORD_T End;
} VCI_FILTER_RECORD;//, *PVCI_FILTER_RECORD;


#define EXTERNC        extern "C"

EXTERNC DWORD_T __stdcall
VCI_OpenDevice(DWORD_T DeviceType,
               DWORD_T DeviceInd,
               DWORD_T Reserved);

EXTERNC DWORD_T __stdcall
VCI_CloseDevice(DWORD_T DeviceType,
                DWORD_T DeviceInd);

EXTERNC DWORD_T __stdcall
VCI_InitCAN(DWORD_T DeviceType,
            DWORD_T DeviceInd,
            DWORD_T CANInd,
            VCI_INIT_CONFIG *pInitConfig);

EXTERNC DWORD_T __stdcall
VCI_ReadBoardInfo(DWORD_T DeviceType,
                  DWORD_T DeviceInd,
                  VCI_BOARD_INFO *pInfo);

EXTERNC DWORD_T __stdcall
VCI_ReadErrInfo(DWORD_T DeviceType,
                DWORD_T DeviceInd,
                DWORD_T CANInd,
                VCI_ERR_INFO *pErrInfo);

EXTERNC DWORD_T __stdcall
VCI_ReadCANStatus(DWORD_T DeviceType,
                  DWORD_T DeviceInd,
                  DWORD_T CANInd,
                  VCI_CAN_STATUS *pCANStatus);

EXTERNC DWORD_T __stdcall
VCI_GetReference(DWORD_T DeviceType,
                 DWORD_T DeviceInd,
                 DWORD_T CANInd,
                 DWORD_T RefType,
                 PVOID_T pData);

EXTERNC DWORD_T __stdcall
VCI_SetReference(DWORD_T DeviceType,
                 DWORD_T DeviceInd,
                 DWORD_T CANInd,
                 DWORD_T RefType,
                 PVOID_T pData);

EXTERNC ULONG_T __stdcall
VCI_GetReceiveNum(DWORD_T DeviceType,
                  DWORD_T DeviceInd,
                  DWORD_T CANInd);

EXTERNC DWORD_T __stdcall
VCI_ClearBuffer(DWORD_T DeviceType,
                DWORD_T DeviceInd,
                DWORD_T CANInd);

EXTERNC DWORD_T __stdcall
VCI_StartCAN(DWORD_T DeviceType,
             DWORD_T DeviceInd,
             DWORD_T CANInd);

EXTERNC DWORD_T __stdcall
VCI_ResetCAN(DWORD_T DeviceType,
             DWORD_T DeviceInd,
             DWORD_T CANInd);

EXTERNC ULONG_T __stdcall
VCI_Transmit(DWORD_T DeviceType,
             DWORD_T DeviceInd,
             DWORD_T CANInd,
             VCI_CAN_OBJ *pSend,
             ULONG_T Len);

EXTERNC ULONG_T __stdcall
VCI_Receive(DWORD_T DeviceType,
            DWORD_T DeviceInd,
            DWORD_T CANInd,
            VCI_CAN_OBJ *pReceive,
            ULONG_T Len,
            INT_T WaitTime = -1);


#endif //REFINE_CONTROLCAN_H
