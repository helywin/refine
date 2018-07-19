//
// Created by jiang.wenqiang on 2018/6/28.
//

#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QtCore/QDebug>

int main() {
    QString a;
    QTextStream stream(&a);
    QString str;
    stream << hex << 34;
    str = stream.readAll();
    qDebug() << str;
    qDebug() << a;
    return 0;
}