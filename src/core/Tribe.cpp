//
// Created by jiang.wenqiang on 2018/7/10.
//

#include "Tribe.h"


Tribe::Tribe(const QStringList &keys) {
    for (const auto &key : keys) {
        if (!key.isEmpty()) {
            clan.insert(key, TribeCell());
        }
    }
}

void Tribe::add(const QString &key) {
    clan.insert(key, TribeCell());
}

void Tribe::add(const QString &str, unsigned int size) {
    clan.insert(str, TribeCell(size));
}

TribeCell &Tribe::operator[](const QString &key) {
    return clan[key];
}

QString Tribe::str(const QString &key) {
    QStringList list;
    for (const auto &iter : clan[key]) {
        list.append(QString::number(iter));
    }
    return list.join(QChar(','));
}
