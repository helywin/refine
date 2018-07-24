//
// Created by jiang.wenqiang on 2018/7/5.
//

#ifndef CORE_SUBLIME_H
#define CORE_SUBLIME_H

#include <QtCore/QThread>
#include <omp.h>
#include "CurveConfig.h"
#include "CanBuffer.h"
#include "Kebab.h"

class Sublime : public QThread {
Q_OBJECT
private:
    CurveConfig &cfg;

    CanBuffer &buffer;

    Kebab &kebab;

    omp_lock_t lock;

public:
    Sublime() = delete;

    Sublime(CurveConfig &cfg, CanBuffer &buffer, Kebab &kebab);

    ~Sublime() final;

protected:
    void run() Q_DECL_OVERRIDE;

signals:

    void result(unsigned int use, unsigned int total);

    void unload();
};


#endif //CORE_SUBLIME_H
