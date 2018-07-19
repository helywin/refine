//
// Created by jiang.wenqiang on 2018/7/17.
//

#include "../src/Log.h"
#include "yaml-cpp/yaml.h"
#include <sstream>
#include <QtCore/QFile>

int main() {
    QFile f("D:/jiang.wenqiang/code/autophi/core/data/test.yaml");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!f.isOpen()) {
        return 1;
    }
    QTextStream stream(&f);
    stream.setCodec("GBK");
    QString ss = stream.readAll();
    YAML::Node config = YAML::Load(ss.toStdString());
    if (config["lastLogin"]) {
        QString s = QString::fromStdString(config["lastLogin"].as<std::string>());
        qDebug() << s;
    }
    config["lastLogin"] = QDate::currentDate().toString(QString("yyyy-MM-dd")).toStdString();
    QFile fw("D:/jiang.wenqiang/code/autophi/core/data/test_bak.yaml");
    fw.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream write(&fw);
    write.setCodec("GBK");
    write << QString("#注释") << endl;
    std::stringstream stringstream;
    stringstream << config;
    std::string temp = stringstream.str();
    write << QString::fromStdString(temp);

    return 0;
}