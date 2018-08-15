//
// Created by jiang.wenqiang on 2018/7/10.
//


#include <QtCore/QTextStream>
#include "Abstract.h"
#include "Log.h"

Abstract::Abstract(Tribe &tribe) :
        tribe(tribe) {}

bool Abstract::readCsv(QFile &file) {
    if (file.isOpen()) {
        file.close();
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.isOpen()) {
        qCritical("打开数据文件失败");
        return false;
    }
    QTextStream stream(&file);
    QString s;
    QStringList header;
    QStringList list;
    bool flag;
    stream.setCodec("GBK");
    s = stream.readLine(2000);
    s.remove(QRegExp("\\s"));
    qDebug() << "读取头: " << s;
    header = s.split(QChar(','), QString::KeepEmptyParts);
    for (const auto &str : header) {
        tribe.add(str);
    }
    while (!stream.atEnd()) {
        s = stream.readLine(2000);
        list = s.split(QChar(','));
        for (int i = 0; i < list.size(); ++i) {
            flag = false;
            double val = list[i].toDouble(&flag);
            if (flag) {
                tribe[header[i]].append(val);
            } else {
                tribe[header[i]].append(qSNaN());
            }
//            qDebug() << val;
        }
    }
    file.close();
    if (header[0] == "时间") {
        qDebug("读取编码没问题");
    }
    qDebug("读取完成");
}

bool Abstract::saveCsv(QFile &file) {
    if (file.isOpen()) {
        file.close();
    }
    file.setPermissions(QFileDevice::WriteUser);
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    if (!file.isOpen()) {
        qCritical("打开数据文件失败");
        return false;
    }
    QTextStream stream(&file);
    stream.setCodec("GBK");
    QStringList list(tribe.keys());
    stream << list.join(QChar(',')) << "\n";
    for (int i = 0; i < tribe.length(); ++i) {
        list.clear();
        for (const auto &keys : tribe.keys()) {
            list.append(QString::number(tribe[keys][i]));
        }
        stream << list.join(QChar(',')) << "\n";
    }
    file.setPermissions(QFileDevice::ReadUser);
    file.close();

}