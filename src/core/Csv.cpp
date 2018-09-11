//
// Created by jiang.wenqiang on 2018/9/11.
//

#include "Csv.h"

Csv::Csv() : _file(nullptr), _stream(nullptr), _separator(QChar(',')) {}

Csv::~Csv() {
    if (_file != nullptr) {
        _file->close();
    }
}

void Csv::setSeparator(QChar sep) {
    _separator = sep;
}

void Csv::setFile(QFile *f) {
    Q_ASSERT(f != nullptr);
    _file = f;
}

bool Csv::startWrite(const char *codec, bool is_append) {
    if(is_append) {
        _file->open(QIODevice::WriteOnly |
                    QIODevice::Text |
                    QIODevice::Append);
    } else {
        _file->open(QIODevice::WriteOnly |
                    QIODevice::Text);
    }
    _stream = new QTextStream(_file);
    _stream->setCodec(codec);
    return _file->isOpen();
}

void Csv::writeLine(const QStringList &list) const {
    *_stream << list.join(_separator);
    *_stream << QString("\n");
    _stream->flush();
}

bool Csv::finishWrite() {
    _stream->flush();
    _file->close();
    delete _stream;
    return !_file->isOpen();
}

bool Csv::startRead(const char *codec) {
    _file->open(QIODevice::ReadOnly | QIODevice::Text);
    _stream = new QTextStream(_file);
    _stream->setCodec(codec);
    return _file->isOpen();
}

void Csv::readLine(QStringList &list) const {
    list = _stream->readLine(500).split(_separator, QString::KeepEmptyParts);
}

bool Csv::finishRead() {
    if (_stream->atEnd()) {
        _file->close();
        delete _stream;
    }
    return !_file->isOpen();
}
