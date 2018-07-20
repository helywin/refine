//
// Created by jiang.wenqiang on 2018/7/20.
//

#include "About.h"
#include <QtCore/QEvent>
#include <QtGui/QPicture>
#include <QtCore/QDebug>

About::About(QWidget *parent) : QDialog(parent) {
    setup_ui();
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowTitle("关于");
    setFixedSize(500,500);
    move(420,100);
    QWidget::installEventFilter(this);
}

void About::setup_ui() {
    logo = new QLabel(this);
    logo->move(20,20);
    logo->setFixedSize(200,200);
    QPixmap pic;
    pic.load("D:/jiang.wenqiang/code/refine/assets/logo.png");
    if (pic.isNull()) {
        qDebug("null");
    }
    logo->setPixmap(pic);
    name = new QLabel(this);
    name->setText("Refine");
    QFont font_name("微软雅黑",35, 5);
    name->setFont(font_name);
    name->move(240,35);
    name->resize(240, 45);
    name->setAlignment(Qt::AlignCenter);

    version = new QLabel(this);
    QFont font("微软雅黑",10, 5);
    version->setText("Version: 0.0.1(build 0)");
    version->setFont(font);
    version->move(240, 100);
    version->resize(240, 20);
    version->setAlignment(Qt::AlignCenter);

    intro = new QLabel(this);
    intro->setText("匹配用的软件");
    intro->setFont(font);
    intro->move(240, 140);
    intro->resize(240, 20);
    intro->setAlignment(Qt::AlignCenter);

    copyright = new QLabel(this);
    copyright->setText("Copyright (C) 2018 BYD company LTd.");
    copyright->setFont(font);
    copyright->move(240, 180);
    copyright->resize(240, 20);
    copyright->setAlignment(Qt::AlignCenter);

    QFont font_soft("微软雅黑",20, 5);
    soft = new QLabel(this);
    soft->setText("开源软件协议");
    soft->setFont(font_soft);
    soft->move(20, 240);
    soft->resize(460, 40);
    soft->setAlignment(Qt::AlignCenter);


    license = new QTextBrowser(this);
    license->move(20,280);
    license->resize(460, 200);
}

bool About::eventFilter(QObject *object, QEvent *event) {
    if (object == this) {
        if (event->type() == QEvent::WindowDeactivate) {
            hide();
            return true;
        }
    }
    return false;
}
