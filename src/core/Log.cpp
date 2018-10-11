//
// Created by jiang.wenqiang on 2018/9/11.
//

#include <QtCore/QFile>
#include "Log.hpp"

Log::Cell::Cell() : _type(QtMsgType::QtDebugMsg) {}

Log::Cell::Cell(QtMsgType type, QString &&msg) : _type(type), _msg(msg) {}

Log::Cell::Cell(QtMsgType type, const QString &msg) : _type(type), _msg(msg) {}

QString Log::Cell::str() const
{
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
    return s;
}

Log::Log() :
        _log(), _file(nullptr), _output(nullptr), _save_enable(false),
        _output_enable(false) {}

void Log::handler(QtMsgType type, const QMessageLogContext &context,
                  const QString &msg)
{
    Cell cell(type, msg);
    _log.append(cell);
    if (_save_enable) {
        _stream << cell.str() << "\n";
        _file->flush();
    }
    if (_output_enable) {
        _output(type, msg);
    }
}

bool Log::saveEnable(QFile *file)
{
    Q_ASSERT(_file != nullptr);
    _file = file;
    _save_enable = true;
    _file->open(QIODevice::WriteOnly | QIODevice::Text);
    if (!_file->isOpen()) {
        return false;
    }
    _stream.setDevice(_file);
    _stream.setCodec("UTF-8");
}

void Log::outputEnable(Log::Output *output)
{
    Q_ASSERT(output != nullptr);
    _output = output;
    _output_enable = true;
}

QStringList Log::str() const
{
    QStringList list;
    for (const auto &iter : _log) {
        list.append(iter.str());
    }
    return list;
}
