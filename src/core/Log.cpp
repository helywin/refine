//
// Created by jiang.wenqiang on 2018/6/29.
//

#include "Log.h"
#include <QtCore/QFile>


LogCell::LogCell() {
    type = QtDebugMsg;
}

//LogCell::LogCell(const LogCell &cell) {
//    *this = cell;
//}

//LogCell& LogCell::operator=(const LogCell &cell){
//    this->type = cell.type;
//    this->msg = cell.msg;
//    this->time = cell.time;
//    return *this;
//}

LogCell::LogCell(QtMsgType type, const QString &msg) {
    this->type = type;
    this->msg = msg;
    time = QDateTime::currentDateTime();
}

QString LogCell::str() {
    QString s;
    s += "[";
    s += time.toString(QString("yyyy/MM/dd HH:mm:ss"));
    s += "]";
    s += "\t";
    switch (type) {
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
    s += msg;
    return s;
}

QVector<LogCell> Log::log = QVector<LogCell>();
QString Log::path = QString("");

void Log::handler(QtMsgType type, const QMessageLogContext &ctxt,
                  const QString &msg) {
    LogCell cell(type, msg);
    log.append(cell);
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    if (!file.isOpen()) {
        qCritical("日志文件没打开");
        return;
    }
    QTextStream steam(&file);
    steam.setCodec("UTF-8");
    steam << cell.str() << "\n";
    file.close();
}

void Log::setPath(const QString &path) {
    Log::path = path;
}
/*todo 后面可以调用线程进行日志的写入，
 * 日志文件名称也要按照日期来分开日志的
 * 日志可以选择不写入，也要定期清除日志
 */