//
// Created by jiang.wenqiang on 2018/7/5.
//

#include "Keep.h"
#include <QtCore/QTextStream>
#include "Log.h"

Keep::Keep(Kebab &kebab, QFile &file) :
        kebab(kebab), file(file) {
    if (file.isOpen()) {
        file.close();
    }
    this->file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!file.isOpen()) {
        qCritical("数据文件打不开");
    }
}

void Keep::run() {
    qDebug("线程keep");
    QTextStream stream(&file);
    QStringList list;
    auto array = new double[kebab.size()];
    qDebug() << "数据数 " << kebab.size();
    while (kebab.out(array)) {
        for (int i = 0; i < kebab.size(); ++i) {
//            qDebug() << "keep: " << array[i];
            stream << array[i];
            if (i != kebab.size() - 1) {
                stream << ",";
            } else {
                stream << "\n";
            }
        }
    }
    delete[]array;
    this->file.flush();
    qDebug("线程keep结束");
}

Keep::~Keep() {
    this->file.close();
}