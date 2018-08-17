//
// Created by jiang.wenqiang on 2018/8/8.
//

#include "Dump.h"
#include "Kebab.h"

Dump::Dump() : _kebab(nullptr), _file(nullptr), _file_type(FileType::Csv),
               _csv(nullptr), _rf(nullptr) {}

void Dump::setKebab(Kebab *kebab) {
    Q_ASSERT(kebab != nullptr);
    _kebab = kebab;
}

void Dump::setFile(QFile *f, const FileType type) {
    Q_ASSERT(f != nullptr);
    if (_file != nullptr) {
        _file->close();
    }
    _file = f;
    _file_type = type;
    switch (_file_type) {
        case FileType::Csv:
            _csv = new ::Csv();
            _csv->setFile(f);
            _csv->startWrite("gbk", true);
            break;
        case FileType::Rf:
            break;
    }
}

void Dump::run() {
    Q_ASSERT(_kebab != nullptr);
    Q_ASSERT(_file != nullptr);
    if (_file_type == FileType::Csv) {
        QStringList list;
        while (!_kebab->isEmpty()) {
            if (_kebab->popLine(list)) {
                _csv->writeLine(list);
            }
        }
    }
}

