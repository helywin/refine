//
// Created by jiang.wenqiang on 2018/6/29.
//

#ifndef CORE_LOG_H
#define CORE_LOG_H

#include <QtCore/QDebug>
#include <QtCore/QDateTime>

class LogCell{
public:
    LogCell(QtMsgType type, const QString &msg);
    LogCell(const LogCell& cell) = default;
    LogCell &operator=(const LogCell &cell) = default;
    LogCell();

    QtMsgType type;
    QString msg;
    QDateTime time;
    QString str();
};

class Log {
public:

    static QVector<LogCell> log;
    static QString path;

    static void handler(QtMsgType type, const QMessageLogContext & ctxt,
                        const QString &msg);

    static void set_path(const QString &path);
};


#endif //CORE_LOG_H
