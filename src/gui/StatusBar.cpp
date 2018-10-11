//
// Created by jiang.wenqiang on 2018/10/11.
//

#include "StatusBar.hpp"

StatusLabel::StatusLabel(QWidget *parent, const QString &prefix) :
        _prefix(prefix), QLabel(parent)
{
    setText(prefix);
}


StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent)
{
    setupUi();
}

void StatusBar::setupUi()
{
    _label_info = new StatusLabel(this, QString("信息: "));
    _label_warning = new StatusLabel(this, QString("警告: "));
    _label_can = new StatusLabel(this, QString("CAN: "));
    _label_curve = new StatusLabel(this, QString("配置: "));
    addWidget(_label_info, 1);
    addWidget(_label_warning, 1);
    addWidget(_label_can);
    addWidget(_label_curve);
}

void StatusBar::displayMessage(QtMsgType type, const QString &info)
{
    switch (type) {
        case QtMsgType::QtInfoMsg:
            _label_info->setTextWithPrefix(info);
            break;
        case QtMsgType::QtWarningMsg:
        case QtMsgType::QtFatalMsg:
        case QtMsgType::QtCriticalMsg:
            _label_warning->setTextWithPrefix(info);
            break;
        default:
            break;
    }
}
