//
// Created by jiang.wenqiang on 2018/7/5.
//

#include "Revolve.h"
#include "Log.h"

Revolve::Revolve(Keep &keep, Collect &clt, Sublime &sub) :
        unconnected_times(0), collect_fail(false),
        clt(clt), keep(keep), sub(sub){
    connect(&clt, &Collect::get, this, &Revolve::collect_got,
            Qt::DirectConnection);
    connect(&clt, &Collect::fail, this, &Revolve::collect_failed,
            Qt::DirectConnection);

    connect(&sub, &Sublime::result, this, &Revolve::sublime_result,
            Qt::DirectConnection);
    connect(&sub, &Sublime::unload, this, &Revolve::sublime_unloaded,
            Qt::DirectConnection);

    connect(&keep, &Keep::fail, this, &Revolve::keep_failed,
            Qt::DirectConnection);
}

void Revolve::start_sublime() {
    if (!sub.isRunning()) {
        sub.start();
    }
}

void Revolve::sublime_result(unsigned int use,
                             unsigned int total) {
    qDebug() << "处理结果" << use << "/" << total;
}

void Revolve::start_keep() {
    if (!keep.isRunning()) {
        keep.start(QThread::HighestPriority);
    }
}

void Revolve::keep_failed(Keep::KeepFail code) {

}

void Revolve::collect_got() {
    start_sublime();
}

void Revolve::start_collect() {
    if (!clt.isRunning()) {
        clt.start();
    }
}


void Revolve::collect_failed(Collect::Fail code) {
    collect_fail = true;
    qDebug("发送停止信号");
    clt.stop();
}

void Revolve::sublime_unloaded() {
    qDebug("启动存储数据线程");
    start_keep();
}

bool Revolve::marvel() {
    start_collect();
    while (true) {
        if (clt.isFinished() && sub.isFinished() &&
            keep.isFinished()) {
            start_keep();
            while (keep.isRunning()) {}
            return true;
        } else {
            if (collect_fail && clt.isFinished()) {
                start_keep();
                while (keep.isRunning()) {}
                return false;
            }
        }
    }
}
