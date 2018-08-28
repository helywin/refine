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
    Config *_config;

public:
    Can() = delete;

    explicit Can(Config *config);

    void setConfig(Config *config);

    bool open();

    bool init();

    bool start();

    bool connect();

    bool close();

    bool reset();

    bool reconnect();

    bool collect(Buffer &buffer, int delay = 0);

    bool deliver(Buffer &buffer);

    bool command(unsigned int id, QString &&cmd);

    bool isConnected();

    void clear();

private:
    void getError(VCI_ERR_INFO *error);
};

class Can::Config {
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

    ~Config();

    unsigned long deviceType() const;
    unsigned long deviceIndex() const;
    unsigned long deviceChannel() const;
    unsigned long reserved() const;
    const VCI_INIT_CONFIG *initConfig() const;
    VCI_INIT_CONFIG *initConfig();

};

class Can::BoardInfo {
};

class Can::ErrorInfo {
};

class Can::RegStatus {
};




#endif //REFINE_CAN_H
