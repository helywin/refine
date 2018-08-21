//
// Created by jiang.wenqiang on 2018/7/3.
//

#include "my_head.h"

int main() {
//    qInstallMessageHandler(Log::handler);
    Sender send;
    Receiver receiver;
    receiver.set_send(&send);
//    send.start();
    receiver.test();
}