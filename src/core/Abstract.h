//
// Created by jiang.wenqiang on 2018/7/10.
//

#ifndef CORE_ABSTRACT_H
#define CORE_ABSTRACT_H

#include "Tribe.h"
#include <QtCore/QFile>

class Abstract {
private:
    Tribe &tribe;

public:
    Abstract() = delete;

    explicit Abstract(Tribe &tribe);

    bool read_csv(QFile &file);

    bool save_csv(QFile &file);
};


#endif //CORE_ABSTRACT_H
