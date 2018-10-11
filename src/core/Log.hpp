//
// Created by jiang.wenqiang on 2018/9/11.
//

#ifndef REFINE_LOG_HPP
#define REFINE_LOG_HPP

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <QtCore/QDataStream>

class Logger;

class Log
{
private:
    class Cell
    {
    public:
        Cell() = delete;
        Cell(QtMsgType type, QString &&msg);
        Cell(QtMsgType type, const QString &msg);
        Cell(const Cell &cell) = default;
        Cell &operator=(const Cell &cell) = default;
        QString str() const;

    private:
        QtMsgType _type;
        QString _msg;
        QDateTime _time;
    };
public:

private:
    static QList<Cell> _log;
    static QFile *_file;
    static Logger *_output;
    static bool _save_enable;
    static bool _output_enable;
    static QTextStream _stream;
public:

    static void handler(QtMsgType type, const QMessageLogContext &context,
                        const QString &msg);

    static inline void setupHandler()
    {
        qInstallMessageHandler((QtMessageHandler) &handler);
    }

    static bool enableSave(QFile *file);

    static inline void disableSave() { _save_enable = false; }

    static void enableOutput(Logger *output);

    static inline void disableOutput() { _output_enable = false; }

    static QStringList str();
};

/*!
 * @brief 处理日志的接口类
 */
class Logger
{
public:
    virtual void displayMessage(QtMsgType type, const QString &info) = 0;
};

#endif //REFINE_LOG_HPP
