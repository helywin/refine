//
// Created by jiang.wenqiang on 2018/7/5.
//

#include "Keep.h"
#include <QtCore/QTextStream>
#include "Log.h"

Keep::Keep(Kebab &raft, QFile &file) :
        raft(raft), file(file) {
    if (file.isOpen()) {
        file.close();
    }
}

void Keep::run() {
    qDebug("线程keep");
    this->file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!file.isOpen()) {
        qCritical("数据文件打不开");
        return;
    }
    QTextStream stream(&file);
    auto array = new double[raft.size()];
    qDebug() << "数据数 " << raft.size();
    while (raft.out(array)) {
        for (int i = 0; i < raft.size(); ++i) {
            stream << array[i];
            if (i != raft.size() - 1) {
                stream << ",";
            } else {
                stream << "\n";
            }
        }
    }
    delete[]array;
    this->file.close();
    qDebug("线程keep结束");
}