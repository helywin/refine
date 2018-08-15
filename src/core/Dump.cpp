//
// Created by jiang.wenqiang on 2018/8/8.
//

#include "Dump.h"

Dump::Dump() : _kebab(nullptr), _file(nullptr), _file_type(FileType::Csv){}

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
}

void Dump::run() {
    Q_ASSERT(_kebab != nullptr);
    Q_ASSERT(_file != nullptr);

}

