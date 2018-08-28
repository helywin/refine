//
// Created by jiang.wenqiang on 2018/8/20.
//

#include <QtCore/QDebug>
#include "Load.h"

Load::Load() : _tribe(nullptr), _file(nullptr), _file_type(FileType::Csv),
               _csv(nullptr), _ref(nullptr) {}

void Load::setTribe(Tribe* tribe) {
    Q_ASSERT(tribe != nullptr);
    _tribe = tribe;
}

void Load::setFile(QFile *f, Load::FileType type) {
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
            _csv->startRead("gbk");
            break;
        case FileType::Ref:
            break;
    }
}

void Load::run() {
    Q_ASSERT(_tribe != nullptr);
    Q_ASSERT(_file != nullptr);
    if (_file_type == FileType::Csv) {
        QStringList list;
        QStringList name;
        while (!_csv->finishRead()) {
            if(name.isEmpty()) {
                _csv->readLine(name);
                _tribe->setIndex(name, Tribe::DataType::Raw);
            }
            _csv->readLine(list);
            _tribe->addSequence(list, Tribe::DataType::Raw);
        }
    }
}
