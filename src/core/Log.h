//
// Created by jiang.wenqiang on 2018/8/7.
//

#ifndef REFINE_LOG_H
#define REFINE_LOG_H

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QDateTime>

class Log {
private:
    class Cell;

private:
    QVector<Cell> _log;
    QString _path;
    QFile _file;
public:
    explicit Log(const QString &path);

    ~Log();

    void handler(QtMsgType type, const QMessageLogContext &context,
                 const QString &msg);
};

class Log::Cell {
public:
    Cell();

    Cell(QtMsgType type, const QString &&msg);

    Cell(QtMsgType type, const QString &msg);

    Cell(const Cell &cell) = default;

    Cell &operator=(const Cell &cell) = default;

    QString str() const;

private:
    QtMsgType _type;
    QString _msg;
    QDateTime _time;
};


#endif //REFINE_LOG_H
