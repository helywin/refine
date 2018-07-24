//
// Created by jiang.wenqiang on 2018/7/17.
//

#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

int main() {
    QJsonObject json;
    json.insert("name", "jiang");
    json.insert("age", 22);
    QJsonArray array;
    array.append(1);
    array.append(2);
    json.insert("arrays", array);
    QFile file("D:/jiang.wenqiang/code/autophi/core/data/test.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QJsonDocument doc(json);
    file.write(doc.toJson());
    file.close();
    qDebug() << json;
    QFile f("D:/jiang.wenqiang/code/autophi/core/data/test_bak.json");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!f.isOpen()) {
        return 1;
    }

    QJsonDocument rdoc;
    QJsonObject rjson(rdoc.object());
    auto error = new QJsonParseError;
    rdoc = QJsonDocument::fromJson(f.readAll(), error);
    qDebug() << rjson;
    qDebug() << rdoc.object();
    return 0;
}