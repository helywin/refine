//
// Created by jiang.wenqiang on 2018/9/10.
//

#ifndef REFINE_CAN_HPP
#define REFINE_CAN_HPP


#include <QtCore/QString>
#include "ControlCan.h"

class Buffer;

class Can
{
public:
    class Config
    {
    private:
        unsigned long _device_type;      //! \brief 设备类型
        unsigned long _device_index;     //! \brief 设备索引号
        unsigned long _device_channel;   //! \brief 第几路
        unsigned int _baud_rate;         //! \brief 波特率
        VCI_INIT_CONFIG *_config;
    public:
        Config() = delete;
        explicit Config(unsigned long channel);
        Config(const Config &config) = delete;
        Config &operator=(const Config &config) = delete;
        ~Config() { delete _config; }
        inline unsigned long deviceType() const { return _device_type; }
        inline unsigned long deviceIndex() const { return _device_index; }
        inline unsigned long deviceChannel() const { return _device_channel; }
        inline unsigned long reserved() const { return _config->Reserved; }
        inline const VCI_INIT_CONFIG *config() const { return _config; }
        inline VCI_INIT_CONFIG *config() { return _config; }
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

    enum ReceiveResult
    {
        ReceiveFailed = -1,
        ReceiveSucceededWithNoFrame = 0,
        ReceiveSucceededWithFrames = 1
    };

private:
    int _status;
    Config *_config;

public:
    Can() = delete;
    explicit Can(Config *config);
    void setConfig(Config *config);
    bool open();
    bool init();
    bool start();
    bool connect(bool close = false);
    bool close();
    bool reset();
    bool reconnect();
    int collect(Buffer &buffer, int delay = 0);
    bool deliver(Buffer &buffer);
    bool command(unsigned int id, QString &&cmd);
    bool isConnected();
    void clear() const;
    int status() const;

private:
    void getError(VCI_ERR_INFO *error);
};



#endif //REFINE_CAN_HPP
