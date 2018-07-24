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
    enum ModeType {
        Invalid,
        Acceleration,
        DriveAway,
        Gearshift,
        TipIn,
        TipOut,
        CruiseControl
    };

    enum Fail {
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

    virtual void split_periods() = 0;

    virtual void verify() = 0;

    virtual Fail analyse(Tribe &tribe) = 0;

protected:
    bool check_data(Tribe &tribe);

    void gear_ratio(Tribe &tribe);

    void converter_slip(Tribe &tribe);

    void synchron_speed(Tribe &tribe);

    void road_gradient(Tribe &tribe);

    void wheel_slip(Tribe &tribe);

    void torque(Tribe &tribe);

    void power(Tribe &tribe);

    void bemp(Tribe &tribe);

    void torque_expected(Tribe &tribe);

    void acceleration_expected(Tribe &tribe);
};

//const Criteria::Ratio Criteria::ratio = {};
#endif //CORE_CRITERIA_H