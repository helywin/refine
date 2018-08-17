//
// Created by jiang.wenqiang on 2018/7/11.
// 评分结果基类
//

#ifndef REFINE_RATE_H
#define REFINE_RATE_H

#include <QtCore/QFile>

class Rate {
public:
    enum ModeType {
        Invalid,
        Acceleration,
        DriveAway,
        Gearshift,
        TipIn,
        TipOut,
        CruiseControl
    };
private:
    ModeType mode_type;

public:
    Rate() : mode_type(Invalid) {};
};


#endif //CORE_RATE_H
