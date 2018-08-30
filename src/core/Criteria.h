//
// Created by jiang.wenqiang on 2018/7/11.
// 工况评价的配置和处理函数的基类
//

#ifndef CORE_CRITERIA_H
#define CORE_CRITERIA_H

#include <QtCore/QFile>
#include "Tribe.h"
#include "Rate.h"

class Criteria {
public:
    enum class ModeType {
        Invalid,
        Acceleration,
        DriveAway,
        Gearshift,
        TipIn,
        TipOut,
        CruiseControl
    };

    enum class Fail {
        File
    };

    class ModePeriod {
        bool validity;
        unsigned int start;
        unsigned int length;
    };

public:
    ModeType mode_type;

    QVector<ModePeriod> mode_periods;

    Tribe &tribe;

    Rate &rate;

public:
    Criteria() = delete;

    Criteria(ModeType type, Tribe &tribe, Rate &rate);

    virtual Fail init(QFile &file) = 0;

    virtual void splitPeriods() = 0;

    virtual void verify() = 0;

    virtual Fail analyse(Tribe &tribe) = 0;

protected:
    bool checkData(Tribe &tribe);

    void gearRatio(Tribe &tribe);

    void converterSlip(Tribe &tribe);

    void synchronSpeed(Tribe &tribe);

    void roadGradient(Tribe &tribe);

    void wheelSlip(Tribe &tribe);

    void torque(Tribe &tribe);

    void power(Tribe &tribe);

    void bemp(Tribe &tribe);

    void torqueExpected(Tribe &tribe);

    void accelerationExpected(Tribe &tribe);
};

//const Criteria::Ratio Criteria::ratio = {};
#endif //CORE_CRITERIA_H