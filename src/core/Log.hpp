//
// Created by jiang.wenqiang on 2018/9/11.
//

#ifndef CORE_LOG_HPP
#define CORE_LOG_HPP

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <QtCore/QDataStream>

class Log
{
private:
    class Cell
    {
    public:
        Cell();
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
    //! \brief 输出的函数指针，对象无关，方便后面变更
    typedef void (Output)(QtMsgType type, const QString &info);
private:
    QList<Cell> _log;
    QFile *_file;
    Output *_output;
    bool _save_enable;
    bool _output_enable;
    QTextStream _stream;
public:
    Log();

    void handler(QtMsgType type, const QMessageLogContext &context,
                 const QString &msg);

    inline void setupHandler() const { qInstallMessageHandler(Log::handler); }

    bool saveEnable(QFile *file);

    inline void saveDisable() { _save_enable = false; }

    void outputEnable(Output *output);

    inline void outputDisable() { _output_enable = false; }

    QStringList str() const;
};


#endif //CORE_LOG_HPP
