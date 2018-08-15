//
// Created by jiang.wenqiang on 2018/7/5.
//

#include "Revolve.h"
#include "Log.h"

Revolve::Revolve(Keep &keep, Collect &clt, Sublime &sub) :
        unconnected_times(0), collect_fail(false),
        clt(clt), keep(keep), sub(sub){
    connect(&clt, &Collect::get, this, &Revolve::collectGot,
            Qt::DirectConnection);
    connect(&clt, &Collect::fail, this, &Revolve::collectFailed,
            Qt::DirectConnection);

    connect(&sub, &Sublime::result, this, &Revolve::sublimeResult,
            Qt::DirectConnection);
    connect(&sub, &Sublime::unload, this, &Revolve::sublimeUnloaded,
            Qt::DirectConnection);

    connect(&keep, &Keep::fail, this, &Revolve::keepFailed,
            Qt::DirectConnection);
}

void Revolve::startSublime() {
    if (!sub.isRunning()) {
        sub.start();
    }
}

void Revolve::sublimeResult(unsigned int use,
                            unsigned int total) {
    qDebug() << "处理结果" << use << "/" << total;
}

void Revolve::startKeep() {
    if (!keep.isRunning()) {
        keep.start(QThread::HighestPriority);
    }
}

void Revolve::keepFailed(Keep::KeepFail code) {

}

void Revolve::collectGot() {
    startSublime();
}

void Revolve::startCollect() {
    if (!clt.isRunning()) {
        clt.start();
    }
}


void Revolve::collectFailed(Collect::Fail code) {
    collect_fail = true;
    qDebug("发送停止信号");
    clt.stop();
}

void Revolve::sublimeUnloaded() {
    qDebug("启动存储数据线程");
    startKeep();
}

bool Revolve::marvel() {
    startCollect();
    while (true) {
        if (clt.isFinished() && sub.isFinished() &&
            keep.isFinished()) {
            startKeep();
            while (keep.isRunning()) {}
            return true;
        } else {
            if (collect_fail && clt.isFinished()) {
                startKeep();
                while (keep.isRunning()) {}
                return false;
            }
        }
    }
}
