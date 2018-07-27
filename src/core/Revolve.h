//
// Created by jiang.wenqiang on 2018/7/5.
//

#ifndef CORE_REVOLVE_H
#define CORE_REVOLVE_H

#include <QtCore/QObject>
#include "Keep.h"
#include "Collect.h"
#include "Sublime.h"
#include <QtCore/QMutex>

class Revolve : public QObject {
Q_OBJECT
private:
    Keep &keep;

    Collect &clt;

    Sublime &sub;

    bool collect_fail;

    unsigned int unconnected_times;

public:
    Revolve() = delete;

    Revolve(Keep &keep, Collect &clt, Sublime &sub);

    inline void startSublime();

    inline void startKeep();

    inline void startCollect();

    bool marvel();

public slots:

    void collectGot();

    void collectFailed(Collect::Fail code);

    void sublimeResult(unsigned int use, unsigned int total);

    void sublimeUnloaded();

    void keepFailed(Keep::KeepFail code);

};


#endif //CORE_REVOLVE_H
