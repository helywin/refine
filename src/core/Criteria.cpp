//
// Created by jiang.wenqiang on 2018/7/11.
// 数据预处理，打算用OpenMP并行处理
//

#include "Criteria.h"
#include "Constant.h"
#include "cmath"

Criteria::Criteria(Criteria::ModeType type, Tribe &tribe, Rate &rate) :
        mode_type(type), tribe(tribe), rate(rate) {}

bool Criteria::checkData(Tribe &tribe) {
    return tribe.exist("发动机转速_SMO") &&
           tribe.exist("当前档位") &&
           tribe.exist("车速");
}

void Criteria::gearRatio(Tribe &tribe) {
    tribe.add("传动比", tribe.length());
#pragma omp parallel for
    for (int i = 0; i < tribe.length(); ++i) {
        tribe["传动比"][i] =
                tribe["发动机转速_SMO"][i] / tribe["车速_SMO"][i];
    }
}

void Criteria::converterSlip(Tribe &tribe) {
    tribe.add("转换损失", tribe.length());
#pragma omp parallel for
    for (int i = 0; i < tribe.length(); ++i) {
        double current_ratio = 0;
        switch ((int) tribe["当前档位"][i]) {
            case Const::Shift::one :
                current_ratio = Const::Ratio::one;
                break;
            default:
                break;
        }
        tribe["转换损失"][i] =
                (tribe["传动比"][i] - current_ratio) / current_ratio;
    }
}

void Criteria::synchronSpeed(Tribe &tribe) {
    tribe.add("发动机同步速度", tribe.length());
#pragma omp parallel for
    for (int i = 0; i < tribe.length(); ++i) {
        tribe["同步速度"][i] =
                tribe["传动比"][i] * tribe["车速"][i];
    }
}

void Criteria::roadGradient(Tribe &tribe) {
    tribe.add("道路坡度", tribe.length());
#pragma omp parallel for
    for (int i = 0; i < tribe.length(); ++i) {
        tribe["道路坡度"][i] =
                asin(tribe["加速度"][i] / Const::gravity);
    }
}

void Criteria::wheelSlip(Tribe &tribe) {
    tribe.add("打滑", tribe.length());
#pragma omp parallel for
    //todo
}

void Criteria::torque(Tribe &tribe) {
    //todo
}

void Criteria::power(Tribe &tribe) {
    //todo
}

void Criteria::bemp(Tribe &tribe) {
    //todo 貌似可以采集到
}

void Criteria::torqueExpected(Tribe &tribe) {
    //todo 驾驶员请求扭矩吗
}

void Criteria::accelerationExpected(Tribe &tribe) {
    //todo 根据期望扭矩求
}