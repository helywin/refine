//
// Created by jiang.wenqiang on 2018/7/10.
//

#ifndef CORE_TRIBE_H
#define CORE_TRIBE_H

#include <QtCore/QVector>
#include <QtCore/QMap>

class Tribe {
public:
    typedef QVector<float> Cell;
private:
    QMap<QString, Cell> clan;

public:

    Tribe() = default;

    explicit Tribe(const QStringList &keys);

    ~Tribe() = default;

    void add(const QString &key);

    void add(const QString &key, unsigned int size);

    Cell &operator[](const QString &key);

    QString str(const QString &key);

    inline QString str(const char *key) { return str(QString(key)); };

    inline unsigned int size() { return (unsigned) clan.size(); }

    inline QList<QString> keys() { return clan.keys(); }

    inline unsigned int length() {
        return (unsigned) clan.begin().value().length();
    }

    inline bool exist(const QString &key) { return clan.contains(key); }

    inline bool empty(const QString &key) { return clan.empty(); };

};


#endif //CORE_TRIBE_H
