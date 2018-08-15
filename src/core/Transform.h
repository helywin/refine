//
// Created by jiang.wenqiang on 2018/7/30.
//

#ifndef REFINE_TRANSFORM_H
#define REFINE_TRANSFORM_H

#include "Buffer.h"
#include "Curve.h"
#include "Kebab.h"
#include "Tribe.h"

class Transform {
private:
    Curve &config;
public:
    explicit Transform(Curve &config);

    Kebab::Group &&canToData(Buffer &buffer) const;

    Buffer::Cell &&dataToCan(Tribe &tribe) const;
};


#endif //REFINE_TRANSFORM_H
