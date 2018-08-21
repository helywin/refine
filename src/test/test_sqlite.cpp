//
// Created by jiang.wenqiang on 2018/8/13.
//

#include <QtSql/QSqlDatabase>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QSqlDatabase db;
    QString file = QFileDialog::getOpenFileName(nullptr, "选择数据文件", "", "数据库(*.db *.db3)");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(file);
    return QApplication::exec();
}