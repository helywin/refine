//
// Created by jiang.wenqiang on 2018/7/5.
//

#include "my_head.h"
#include <QtCore/QWaitCondition>
#include <iostream>
#include <windows.h>

using std::cout;
using std::endl;

void Sender::run() {
    for (int i = 0; i < 20; ++i) {
        while (!lock.try_lock()) {}
        cout << "send:" << i << endl;
        emit string_send();
        lock.unlock();
        msleep(1000);
    }
    emit finish_send();
}

void Sender::start_send() {
    cout << "start send" << endl;
    run();
}

void Sender::pause_send() {
    cout << "pause send" << endl;
    while (!lock.try_lock()) {}
    cout << "pause send finish" << endl;
}

void Sender::resume_send() {
    cout << "resume send" << endl;
    lock.unlock();
}

void Sender::stop_send() {
    cout << "stop send" << endl;
    exit();
}

void Receiver::send_finish() {
    cout << "send finish" << endl;
}

void Receiver::send_string() {
    cout << "send string" << endl;
}

void Receiver::set_send(Sender *send) {
//    QObject::connect(this, SIGNAL(start_send()), send, SLOT(start_send()));
//    QObject::connect(this, SIGNAL(pause_send()), send, SLOT(pause_send()));
//    QObject::connect(this, SIGNAL(resume_send()), send, SLOT(resume_send()));
//    QObject::connect(this, SIGNAL(stop_send()), send, SLOT(stop_send()));
    this->send = send;
    QObject::connect(send, SIGNAL(finish_send()), this, SLOT(send_finish()),
                     Qt::DirectConnection);
    QObject::connect(send, SIGNAL(string_send()), this, SLOT(send_string()),
                     Qt::DirectConnection);
}

void Receiver::test() {
    send->start();
    cout << "send start signal" << endl;
    Sleep(3000);
    cout << "send pause signal" << endl;
    send->pause_send();
    Sleep(3000);
    cout << "send resume signal" << endl;
    send->resume_send();
    Sleep(50000);
    cout << "send stop signal" << endl;
    send->stop_send();
}