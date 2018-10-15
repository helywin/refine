//
// Created by jiang.wenqiang on 2018/10/15.
//

#include <QtCore/QObject>
#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtCore/QDebug>

class Print : public QThread
{
Q_OBJECT

protected:
    void run() override
    {
        qDebug("begin");
        msleep(2000);
        qDebug("finish");
    }

public:
    void startPrint() {
        if (this->isRunning()) {
            qDebug("is running");
        }
        this->start();
    }
};
