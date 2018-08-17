//
// Created by jiang.wenqiang on 2018/8/8.
//

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtCore/QDebug>

int main() {
    QSqlDatabase data = QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName(QString("D:/jiang.wenqiang/Desktop/data.db"));
    if (data.open()) {
        qDebug("成功！");
    } else {
        qDebug("失败！");
    }
    QSqlQuery query;
    query.exec("");
    query.value(0);
    return 0;
}