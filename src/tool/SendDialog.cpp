//
// Created by jiang.wenqiang on 2018/7/30.
//

#include "SendDialog.h"

SendDialog::SendDialog(QWidget *parent) : QMainWindow(parent){
    setupUi();
}

void SendDialog::setupUi() {
    resize(500,500);
}

void SendDialog::resizeEvent(QResizeEvent *event) {
}
