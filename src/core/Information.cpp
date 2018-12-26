//
// Created by jiang.wenqiang on 2018/12/26.
//

#include "Information.hpp"

Information::Cell::Cell(Re::MessageTypes type, const QString &msg) :
        _type(type)
{
    _message = msg;
    _message = _message.replace("\n", "\\n");
    _time = QDateTime::currentDateTime();
}

QString Information::Cell::str() const
{
    return _time.toString("yy-MM-dd hh-mm-ss - ")
           + QString("[%1]").arg(_type, 2, 16, QChar(' '))
           + _message;
}

Information::Information()
{
    QString log_path = QString("./log/%1.log").arg(QDate::currentDate().toString("yyyy-MM-dd"));
    _stream = new QTextStream(&log_path, QIODevice::Append
                                         | QIODevice::WriteOnly
                                         | QIODevice::Text);
    _stream->setCodec("gbk");
    (*_stream) << QString("=================%1=================").
            arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")) << "\n";
}

void Information::clear(Re::MessageTypes type)
{
    while (isRunning()) {}  //不引起冲突
    _is_clear = true;
    if (type & Re::Info) { _info_lists.clear(); }
    if (type & Re::Warning) { _warning_lists.clear(); }
    if (type & Re::Critical) { _critical_lists.clear(); }
    if (type & Re::Fatal) { _fatal_lists.clear(); }
    if (type & Re::Debug) { _debug_lists.clear(); }
    if (type & Re::Can) { _can_lists.clear(); }

    for (int i = 0; i < _all_infos.size(); ++i) {
        if (_all_infos[i].type() & type) {
            _all_infos.removeAt(i);
            i -= 1;     //保持当前位置
        }
    }
    _index = _all_infos.size() - 1;     //没有写入文件的缓存不会写入文件
    _is_clear = false;
}

void Information::run()
{
    if (_is_clear) {
        return;     //避开竞争
    }
    int len = _all_infos.size() - _index;
    for (int i = 0; i < len; ++i) {
        (*_stream) << _all_infos[len].str();
    }
    _index = _all_infos.size() - 1;
}

void Information::getMessage(Re::MessageTypes type, const QString &msg)
{
    _all_infos.append(Cell(type, msg));
    int last = _all_infos.size() - 1;
    if (type & Re::Info) {
        _info_lists.append(last);
    }
    if (type & Re::Warning) {
        _warning_lists.append(last);
    }
    if (type & Re::Critical) {
        _critical_lists.append(last);
    }
    if (type & Re::Fatal) {
        _fatal_lists.append(last);
    }
    if (type & Re::Debug) {
        _debug_lists.append(last);
    }
    if (type & Re::Can) {
        _can_lists.append(last);
    }
    if (!isRunning()) {
        start();
    }
}

Information::~Information()
{
    delete _stream;
}

int Information::size(Re::MessageTypes type) const
{
    switch (type) {
        case Re::Info:
            return _info_lists.size();
        case Re::Warning:
            return _warning_lists.size();
        case Re::Critical:
            return _critical_lists.size();
        case Re::Fatal:
            return _fatal_lists.size();
        case Re::Debug:
            return _debug_lists.size();
        case Re::Can:
            return _can_lists.size();
        default:
            return 0;
    }
}

const Information::Cell &Information::data(int index, Re::MessageTypes type) const
{
    Q_ASSERT(index >= 0 && index < size(type));
    return _all_infos[indexList(type)[index]];
}

const QVector<int> &Information::indexList(Re::MessageTypes type) const
{
    switch (type) {
        case Re::Info:
            return _info_lists;
        case Re::Warning:
            return _warning_lists;
        case Re::Critical:
            return _critical_lists;
        case Re::Fatal:
            return _fatal_lists;
        case Re::Debug:
            return _debug_lists;
        case Re::Can:
            return _can_lists;
        default:
            Q_ASSERT(0);        //不允许跳到这里
    }
}
