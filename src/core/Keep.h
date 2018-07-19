//
// Created by jiang.wenqiang on 2018/7/5.
//

#ifndef CORE_KEEP_H
#define CORE_KEEP_H

#include <QtCore/QThread>
#include "Kebab.h"
#include <QtCore/QFile>

class Keep : public QThread {
Q_OBJECT
public:
    enum KeepFail {
        Fail
    };
private:
    Kebab &raft;

    QFile &file;

public:
    Keep() = delete;

    Keep(Kebab &raft, QFile &file);

protected:
    void run() Q_DECL_OVERRIDE;

public slots:
//    void start();

signals:
    void fail(KeepFail code);
};


#endif //CORE_KEEP_H
