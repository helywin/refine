//
// Created by jiang.wenqiang on 2018/9/11.
//

#include <QtCore/QFile>
#include "Log.h"

Log::Log(const QString &path) :
        _log(), _path(path), _file(path) {
    Q_ASSERT(_file.open(QIODevice::WriteOnly | QIODevice::Append));
}

Log::~Log() {
    _file.close();
}

void Log::handler(QtMsgType type, const QMessageLogContext &context,
                  const QString &msg) {
    Cell cell(type, msg);
    _log.append(cell);
    QTextStream steam(&_file);
    steam.setCodec("UTF-8");
    steam << cell.str() << "\n";
    _file.flush();
}

Log::Cell::Cell() : _type(QtMsgType::QtDebugMsg) {}

Log::Cell::Cell(QtMsgType type, const QString &&msg) : _type(type), _msg(msg) {}

Log::Cell::Cell(QtMsgType type, const QString &msg) : _type(type), _msg(msg) {}

QString Log::Cell::str() const {
    QString s;
    s += "[";
    s += _time.toString(QString("yyyy/MM/dd HH:mm:ss"));
    s += "]";
    s += "\t";
    switch (_type) {
        case QtDebugMsg:
            s += "DEBUG   ";
            break;
        case QtWarningMsg:
            s += "WARNING ";
            break;
        case QtCriticalMsg:
            s += "CRITICAL";
            break;
        case QtFatalMsg:
            s += "FATAL   ";
            break;
        case QtInfoMsg:
            s += "INFO    ";
            break;
        default:
            s += "INVALID ";
            break;
    }
    s += "\t";
    s += _msg;
    return qMove(s);
}


