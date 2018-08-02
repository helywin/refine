//
// Created by jiang.wenqiang on 2018/7/10.
//

#ifndef CORE_TRIBE_H
#define CORE_TRIBE_H

#include <QtCore/QVector>
#include <QtCore/QMap>

<<<<<<< HEAD
//class Clan {
//public:
//    QVector<double> member;
//};

typedef QVector<double> TribeCell;

class Tribe {
=======
class Tribe {
public:
    typedef QVector<float> Cell;
>>>>>>> 568e96c... 准备重新开发结构
private:
    QMap<QString, TribeCell> clan;

public:

    Tribe() = default;

    explicit Tribe(const QStringList &keys);

    ~Tribe() = default;

    void add(const QString &key);

    void add(const QString &key, unsigned int size);

    TribeCell &operator[](const QString &key);

    QString str(const QString &key);

<<<<<<< HEAD
    inline QString str(const char *key) {str(QString(key)); };
=======
    inline QString str(const char *key) { return str(QString(key)); };
>>>>>>> 568e96c... 准备重新开发结构

    inline unsigned int size() { return (unsigned)clan.size(); }

    inline QList<QString> keys() { return clan.keys(); }

    inline unsigned int length() {
        return (unsigned) clan.begin().value().length();
    }

    inline bool exist(const QString &key) { return clan.contains(key); }

    inline bool empty(const QString &key) { return clan.empty(); };

};


#endif //CORE_TRIBE_H
