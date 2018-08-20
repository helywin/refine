//
// Created by jiang.wenqiang on 2018/8/13.
//

#ifndef REFINE_REVOLVO_H
#define REFINE_REVOLVO_H


#include <QtCore/QObject>
#include <QtCore/QTimer>
#include "Transmit.h"
#include "Transforo.h"
#include "Collect.h"

class Revolvo : public QObject {
Q_OBJECT
private:
    QTimer _timer;
    Transmit *_transmit;
    Transforo *_transforo;
    int _time_limit;
    int _interval;

public:
    Revolvo() = delete;

    Revolvo(Transmit *transmit, Transforo *transforo,
            int limit = 300, int interval = 10);

    void setTimeLimit(int limit);

    void setTransmit(Transmit *transmit);

    void setTransforo(Transforo *transforo);

    void setInterval(int interval);

    void marvel();

    void pulse();

    void pulse(int msec);

    void resume();

public slots:

};


#endif //REFINE_REVOLVO_H
