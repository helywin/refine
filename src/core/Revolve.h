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
    Keep *keep;

    Collect *clt;

    Sublime *sub;

    bool collect_fail;

    unsigned int unconnected_times;

public:

    Revolve();

    void set(Keep *keep, Collect *clt, Sublime *sub);

    void start_sublime();

    void start_keep();

    void start_collect();

    bool marvel();

public slots:

    void collect_got();

    void collect_failed(Collect::Fail code);

    void sublime_result(unsigned int use, unsigned int total);

    void sublime_unloaded();

    void keep_failed(Keep::KeepFail code);

};


#endif //CORE_REVOLVE_H
