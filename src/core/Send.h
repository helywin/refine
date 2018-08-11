//
// Created by jiang.wenqiang on 2018/7/27.
//

#ifndef REFINE_SEND_H
#define REFINE_SEND_H

#include <QtCore/QThread>
#include "Can.h"
#include "Abstract.h"
#include "Curve.h"

class Send : QThread{
Q_OBJECT
private:
    Tribe &tribe;
    Can &can;
    Curve &config;

public:
    Send() = delete;
    explicit Send(Tribe &Tribe, Can &can, Curve &config);

private:
    void run() override;

};


#endif //REFINE_SEND_H
