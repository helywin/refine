//
// Created by jiang.wenqiang on 2018/7/20.
//

#include "About.hpp"
#include "Version.hpp"
#include <QtCore/QEvent>
#include <QtCore/QDebug>
#include <QtGui/QPainter>

About::About(QWidget *parent) : QDialog(parent) {
    setup();
}

void About::setup() {
    setWindowFlag(Qt::SplashScreen);
    setWindowTitle(tr("关于"));
    setFixedSize(500, 240);
    QWidget::installEventFilter(this);
    logo = new QLabel(this);
    logo->move(20, 20);
    logo->setFixedSize(200, 200);
    QPixmap pic;
    pic.load(":/res/ui/logo.png");
    logo->setPixmap(pic);
    QPalette palette_background = this->palette();
    palette_background.setColor(QPalette::Active, QPalette::Window,
                                QColor(44, 145, 190));
    this->setPalette(palette_background);
    this->setAutoFillBackground(true);
    name = new QLabel(this);
    name->setText("Refine");
    QPalette palette_text = name->palette();
    palette_text.setColor(QPalette::Active, QPalette::WindowText,
                          Qt::white);
    QFont font_name("微软雅黑", 35, 5);
    name->setFont(font_name);
    name->move(240, 35);
    name->resize(240, 45);
    name->setAlignment(Qt::AlignCenter);
    name->setPalette(palette_text);
    name->setAutoFillBackground(true);

    version = new QLabel(this);
    QFont font("微软雅黑", 10, 5);
    version->setText(Version::str());
    version->setFont(font);
    version->move(240, 100);
    version->resize(240, 20);
    version->setAlignment(Qt::AlignCenter);
    version->setPalette(palette_text);
    version->setAutoFillBackground(true);

    intro = new QLabel(this);
    intro->setText(tr("数据采集 & 工况匹配软件"));
    intro->setFont(font);
    intro->move(240, 140);
    intro->resize(240, 20);
    intro->setAlignment(Qt::AlignCenter);
    intro->setPalette(palette_text);
    intro->setAutoFillBackground(true);

    copyright = new QLabel(this);
    copyright->setText(tr("No Copyright"));
    copyright->setFont(font);
    copyright->move(240, 180);
    copyright->resize(240, 20);
    copyright->setAlignment(Qt::AlignCenter);
    copyright->setPalette(palette_text);
    copyright->setAutoFillBackground(true);
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
