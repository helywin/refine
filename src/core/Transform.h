//
// Created by jiang.wenqiang on 2018/7/30.
//

#ifndef REFINE_TRANSFORM_H
#define REFINE_TRANSFORM_H

#include "CanBuffer.h"
#include "CurveConfig.h"
#include "Kebab.h"
#include "Tribe.h"

class Transform {
private:
    CurveConfig &config;
public:
    explicit Transform(CurveConfig &config);

    Kebab::Group &&canToData(CanBuffer &buffer) const;

    CanBuffer::Cell &&dataToCan(Tribe &tribe) const;
};


#endif //REFINE_TRANSFORM_H
