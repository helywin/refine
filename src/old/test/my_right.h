//
// Created by jiang.wenqiang on 2018/7/17.
//

#ifndef CORE_MY_RIGHT_H
#define CORE_MY_RIGHT_H

#include <QtCore/QString>
#include <QtCore/QDebug>

class Right {
public:
    QString str;
public:
    explicit Right(QString &&str);

    explicit Right(Right &&other);

    ~Right();
};

class Left {
public:
    QString str;
public:
    explicit Left(const QString &str);

    Left(const Left &other);

    ~Left();
};


#endif //CORE_MY_RIGHT_H
