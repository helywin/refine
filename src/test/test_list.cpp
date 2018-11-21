//
// Created by jiang.wenqiang on 2018/11/21.
//

#include <QtCore/QList>
#include <QtCore/QDebug>

int main()
{
    QList<double> list;
    qDebug() << list.size();
    list.insert(0, 10);
    qDebug() << list[0];
    return 0;
}