//
// Created by jiang.wenqiang on 2018/8/3.
//

#ifndef REFINE_CAN_H
#define REFINE_CAN_H

#include "ControlCan.h"
#include "Buffer.h"

class Can {
public:
    class Config;

    class BoardInfo;

    class ErrorInfo;

    class RegStatus;

    enum Status {
        Disconnected = 0,
        Connected = 1,
    };

private:
    Status _status;
    const Config *_config;

public:
    Can() = delete;

    explicit Can(const Config *config);

    void setConfig(const Config *config);

    bool connect();

    bool disconnect();

    bool reset();

    bool reconnect();

    bool collect(Buffer &buffer, int delay = -1);

    bool deliver(Buffer &buffer);

    bool command(unsigned int id, QString &&cmd);

    bool isConnected();

private:
    void getError(PVCI_ERR_INFO error);
};

class Can::Config {
private:
    unsigned long _device_type;      //! \brief 设备类型
    unsigned long _device_index;     //! \brief 设备索引号
    unsigned long _device_channel;   //! \brief 第几路
    unsigned int _baud_rate;         //! \brief 波特率
    PVCI_INIT_CONFIG _config;
public:
    Config() = delete;

    explicit Config(unsigned long channel);

    Config(const Config &config) = delete;

    Config &operator=(const Config &config) = delete;

    ~Config();

    inline unsigned long deviceType() const { return _device_type; }

    inline unsigned long deviceIndex() const { return _device_index; }

    inline unsigned long deviceChannel() const { return _device_channel; }

    inline unsigned long reserved() const { return _config->Reserved; }

    PVCI_INIT_CONFIG initConfig() const { return _config; }

};

class Can::BoardInfo {
};

class Can::ErrorInfo {
};

class Can::RegStatus {
};


#endif //REFINE_CAN_H
