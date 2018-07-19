//
// Created by jiang.wenqiang on 2018/7/5.
//

#include "Collect.h"
#include "Log.h"

Collect::Collect(Can &can, CanBuffer &buffer,
                 int minute) :
        can(can), buffer(buffer), minute(minute), pause_flag(false),
        stop_flag(false), reconnect(0), fail_connect(0) {}

void Collect::stop() {
    stop_flag = true;
}

void Collect::pause() {
    pause_flag = true;
}

void Collect::resume() {
    can.clear();
    pause_flag = false;
}

void Collect::run() {
    //connect
    can.close();
    can.open();
    can.init();
    QTime time_now;
    time_now.start();
    if (!can.start()) {
        emit fail(Collect::Start);
        qDebug("打不开CAN盒采集数据");
        return;
    }
    can.clear();
    CanBoardInfo info;
    CanErrorInfo error;
    unsigned int check_cnt = 0;
    while (!stop_flag) {
        msleep(10);
        if (minute*60*1000 <= time_now.elapsed()){
            emit fail(Collect::Time);
            return;
        }
        check_cnt += 1;
        if(check_cnt == 100 && !can.board_info(info)){
            emit fail(Collect::Connect);
        } else if (check_cnt == 100) {
            check_cnt = 0;
        }
        if (pause_flag) {               //暂停
        } else {                        //采集
            unsigned long cache = can.cache();
            if (cache > 0) {
                if (!buffer.full()) {
                    bool flag = can.receive_frame(buffer.push(), error);
                    if (flag) {
                        QStringList list;
                        qDebug() << CanBufferCell::header();
//                        (buffer.head() - 1)->str(list);
//                        for (auto &iter : list) {
//                            qDebug() << iter;
//                        }
                        emit get();
                    } else {
                        qWarning("进入了不该进入的地方");
                    }
                } else {
                    qCritical("帧缓冲区满了");
                }
            }
        }
    }
}