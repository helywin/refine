//
// Created by jiang.wenqiang on 2018/10/30.
//

#include <QtCore/QEvent>
#include "LostFocus.hpp"

LostFocus::LostFocus(QWidget *parent) :
    QDialog(parent)
{
//    setWindowFlag(Qt::SplashScreen);
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    QWidget::installEventFilter(this);
}

bool LostFocus::eventFilter(QObject *object, QEvent *event) {
    if (object == this) {
        if (event->type() == QEvent::WindowDeactivate) {
            hide();
            return true;
        }
    }
    return false;
}