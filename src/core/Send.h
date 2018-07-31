//
// Created by jiang.wenqiang on 2018/7/27.
//

#ifndef REFINE_SEND_H
#define REFINE_SEND_H

#include <QtCore/QThread>
#include "Can.h"
#include "Abstract.h"
#include "CurveConfig.h"

class Send : QThread{
Q_OBJECT
private:
    Abstract &abstract;
    Can &can;
    CurveConfig &config;

public:
    Send() = delete;
    explicit Send(Abstract &abstract, Can &can, CurveConfig &config);

private:
    void run() override;

};


#endif //REFINE_SEND_H
