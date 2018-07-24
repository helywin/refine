//
// Created by jiang.wenqiang on 2018/7/23.
//

#include "Middle.h"
#include <QtGui/QKeyEvent>

Middle::Middle(QWidget *parent) : QWidget(parent) {}

void Middle::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
        case Qt::Key_F11:
            qDebug("hello");
            emit cancel_full_screen();
        default:
            break;
    }
}
