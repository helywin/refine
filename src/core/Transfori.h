//
// Created by jiang.wenqiang on 2018/8/8.
//

#ifndef REFINE_TRANSFORI_H
#define REFINE_TRANSFORI_H

#include <QtCore/QThread>
#include "Curve.h"
#include "Buffer.h"
#include "Kebab.h"

class Transfori : public QThread {
Q_OBJECT
private:
    Curve *_curve;
    Buffer *_buffer;
    Kebab *_kebab;

public:
    Transfori() = delete;

    Transfori(Curve *curve, Buffer *buffer, Kebab *kebab);

    void setCurve(Curve *curve);

    void setBuffer(Buffer *buffer);

    void setKebab(Kebab *kebab);

protected:
    void run() override;
};


#endif //REFINE_TRANSFORI_H
