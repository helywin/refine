//
// Created by jiang.wenqiang on 2018/8/8.
//

#ifndef REFINE_REVOLVI_H
#define REFINE_REVOLVI_H

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include "Collect.h"
#include "Transfori.h"

class Revolvi : public QObject {
Q_OBJECT
private:
    Collect *_collect;
    Transfori *_transfori;
    int _time_limit;
    int _interval;

public:
    Revolvi() = delete;

    Revolvi(Collect *collect, Transfori *transfori,
            int limit = 300, int interval = 10);

    void setTimeLimit(int limit);

    void setCollect(Collect *collect);

    void setTransfori(Transfori *transfori);

    void setInterval(int interval);

    void marvel();

    void pulse();

    void pulse(int msec);

    void resume();

public slots:

    void collectResult(Collect::Result type);

};


#endif //REFINE_REVOLVI_H
