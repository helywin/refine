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
#include "Message.hpp"
#include "CanDefines.hpp"
class RecvBuffer;
class SendBuffer;

/*!
 * @brief 控制CAN的类
 */

class Can : public Message
{
public:
    //! \brief 目前状态
    enum Status
    {
        Closed = 0x00,              //!< 关闭
        Opened = 0x01,              //!< 打开
        Initialized = 0x02,         //!< 初始化
        Started = 0x04,             //!< 启动
        Collecting = 0x08,          //!< 读取
        Transmitting = 0x10,        //!< 发送
        Command = 0x20              //!< 命令
    };

    /*!
     * @brief 采集结果
     * @see collect
     */
    enum Result
    {
        Fail = -1,      //!< 采集失败
        Empty = 0,      //!< 没有任何报文
        Succeed = 1     //!< 采集到报文
    };
    /*!
     * @brief CAN配置类
     */
    class Config
    {
    private:
        unsigned long _device_type;      //! \brief 设备类型
        unsigned long _device_index;     //! \brief 设备索引号
        unsigned long _device_channel;   //! \brief 第几路
        unsigned int _baud_rate;         //! \brief 波特率
        CanCfg _config;         //! \brief 其他配置参数
    public:
        Config();
        /*!
         * @brief 返回设备类型
         * @return 设备类型
         */
        inline unsigned long deviceType() const { return _device_type; }

        /*!
         * @brief 返回设备索引号
         * @return 设备索引号
         */
        inline unsigned long deviceIndex() const { return _device_index; }

        /*!
         * @brief 返回设备通道号
         * @return 设备通道号
         */
        inline unsigned long deviceChannel() const { return _device_channel; }

        /*!
         * @brief 返回设备保留字
         * @return 设备保留字
         */
        inline unsigned long reserved() const { return _config.Reserved; }

        /*!
         * @brief 返回其他参数常引用
         * @return 其他参数常引用
         */
        inline const CanCfg &config() const { return _config; }

    private:
        friend class Can;
        /*!
         * @brief 返回其他参数引用
         * @return 其他参数引用
         * @details 不允许Can以外的类调用该函数直接修改变量
         */
        inline CanCfg &config() { return _config; }

    public:

        /*!
         * @brief 设置设备类型
         * @param type 设备类型
         */
        inline void
        setDeviceType(unsigned long type) { _device_channel = type; }

        /*!
         * @brief 设置设备索引号
         * @param index 设备索引号
         */
        inline void
        setDeviceIndex(unsigned long index) { _device_index = index; }

        /*!
         * @brief 设置设备通道
         * @param channel 设备通道
         */
        inline void
        setDeviceChannel(unsigned long channel) { _device_channel = channel; }

        void setBaudRate(Cd::BaudRate rate);

        /*!
         * @brief 设置接收掩码
         * @param acc_code 接收掩码
         * @details 设置报文滤波时起作用
         */
        inline void
        setAccCode(unsigned long acc_code) { _config.AccCode = acc_code; }

        /*!
         * @brief 设置拒收掩码
         * @param acc_mask 拒收掩码
         * @details 设置报文滤波时起作用
         */
        inline void
        setAccMask(unsigned long acc_mask) { _config.AccMask = acc_mask; }

        /*!
         * @brief 设置保留字
         * @param reserved 保留字
         */
        inline void
        setReserved(unsigned long reserved) { _config.Reserved = reserved; }

        /*!
         * @brief 设置滤波类型
         * @param filter 滤波类型
         */
        inline void
        setFilter(unsigned char filter) { _config.Filter = filter; }

        /*!
         * @brief 设置工作模式
         * @param mode 工作模式
         */
        inline void setMode(unsigned char mode) { _config.Mode = mode; }
    };

    /*!
     * @brief 错误信息
     */
    class ErrorInfo
    {
    private:
        VCI_ERR_INFO _error;    //! \brief 错误信息结构体
    public:
        //! \brief 构造函数
        ErrorInfo() = default;

        /*!
         * @brief 返回错误结构体引用
         * @return 错误结构体引用
         */
        inline VCI_ERR_INFO &error() { return _error; }

        /*!
         * @brief 返回错误结构体常引用
         * @return 错误结构体常引用
         */
        inline const VCI_ERR_INFO &error() const { return _error; }

        /*!
         * @brief 返回错误码
         * @return 错误码
         */
        inline int errorCode() const { return _error.ErrCode; }
    };

private:
    int _status;            //! \brief CAN状态
    Config _config;         //! \brief 配置
    ErrorInfo _error_info;  //! \brief 错误

public:
    explicit Can(Message *message = nullptr);

    /*!
     * @brief 返回配置引用
     * @return 配置引用
     */
    inline Config &config() { return _config; }

    /*!
     * @brief 返回配置常引用
     * @return 配置常引用
     */
    inline const Config &config() const { return _config; }

    bool open();
    bool init();
    bool start();
    bool connect();
    bool close();
    bool reset();
    bool reconnect();
    int collect(RecvBuffer &buffer, int delay = 0);
    bool deliver(SendBuffer &buffer, unsigned long num);
//    bool command(unsigned int id, const QByteArray &cmd);

    /*!
     * @deprecated
     * @brief 发送字符串
     * @param id CAN总线ID
     * @param cmd 字符串右值
     * @return 是否成功
     */
    /*inline bool command(unsigned int id, QByteArray &&cmd)
    {
        return command(id, cmd);
    }*/

    bool isConnected();
    void clear() const;
    int status() const;
    int getError();
    void reportError();

};


#endif //REFINE_CAN_HPP
