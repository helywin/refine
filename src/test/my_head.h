//
// Created by jiang.wenqiang on 2018/7/3.
//

#ifndef CORE_MY_HEAD_H
#define CORE_MY_HEAD_H

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QMutex>

class Sender : public QThread {
Q_OBJECT
public:
    Sender() = default;

protected:
    void run() Q_DECL_OVERRIDE;

private:
    QMutex lock;

public:

    void start_send();

    void pause_send();

    void resume_send();

    void stop_send();

signals:

    void finish_send();

    void string_send();
};

class Receiver : public QObject {
Q_OBJECT
public:
    Sender *send;

    void set_send(Sender *send);

    void test();
public slots:

    void send_finish();

    void send_string();
//signals:
//    void start_send();
//    void pause_send();
//    void resume_send();
//    void stop_send();
};


#endif //CORE_MY_HEAD_H
