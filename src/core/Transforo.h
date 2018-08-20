//
// Created by jiang.wenqiang on 2018/8/13.
//

#ifndef REFINE_TRANSFORO_H
#define REFINE_TRANSFORO_H

#include <QtCore/QThread>
#include "Curve.h"
#include "Buffer.h"
#include "Tribe.h"

class Transforo : public QThread {
Q_OBJECT
private:
    Curve *_curve;
    Buffer *_buffer;
    Tribe *_tribe;
    int _size;
    int _index;

public:
    Transforo() = delete;

    Transforo(Curve *curve, Buffer *buffer, Tribe *tribe);

    void setCurve(Curve *curve);

    void setBuffer(Buffer *buffer);

    void setTribe(Tribe *tribe);

    void resetIndex();

protected:
    void run() override;
};


#endif //REFINE_TRANSFORO_H
