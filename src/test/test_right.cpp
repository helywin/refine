//
// Created by jiang.wenqiang on 2018/7/17.
//

#include "my_right.h"

int main() {
    QString R("right");
    Right right(qMove(R));
    Right right1(qMove(right));
//    qDebug() << right.str;

    return 0;
}
