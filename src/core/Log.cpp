/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/11
 * @brief 日志记录
 * @details 利用Qt接口将日志按照格式存储在文件，同时调用日志函数指针
 ******************************************************************************/

#include <QtCore/QFile>
#include "Log.hpp"

Log::Cell::Cell(QtMsgType type, QString &&msg) :
        _type(type), _msg(msg), _time(QDateTime::currentDateTime()) {}

Log::Cell::Cell(QtMsgType type, const QString &msg) :
        _type(type), _msg(msg), _time(QDateTime::currentDateTime()) {}

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

QList<Log::Cell> Log::_log;
QFile *Log::_file = nullptr;
Logger *Log::_output = nullptr;
bool Log::_save_enable = false;
bool Log::_output_enable = false;
QTextStream Log::_stream;

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
        _output->displayMessage(type, msg);
    }
}

bool Log::enableSave(QFile *file)
{
    Q_ASSERT(file != nullptr);
    _file = file;
    _save_enable = true;
    _file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    if (!_file->isOpen()) {
        return false;
    }
    _stream.setDevice(_file);
    _stream.setCodec("UTF-8");
    return true;
}

void Log::enableOutput(Logger *output)
{
    Q_ASSERT(output != nullptr);
    _output = output;
    _output_enable = true;
}

QStringList Log::str()
{
    QStringList list;
    for (const auto &iter : _log) {
        list.append(iter.str());
    }
    return list;
}
