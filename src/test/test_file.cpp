//
// Created by jiang.wenqiang on 2018/9/13.
//

#include <QtCore/QDebug>
#include <QtCore/QDataStream>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include "File.hpp"
#include "Curve.hpp"
#include "Can.hpp"
#include "Buffer.hpp"

int main()
{
    QDir dir0("sub0");
    QDir dir1("sub1");
    QFileInfo f(dir0, "a.txt");
    QFile ff("sub0/a.txt");
    if (f.exists()) {
        qDebug() << "f.fileName(): " << f.fileName();
        qDebug() << "f.absolutePath(): " << f.absolutePath();
        qDebug() << "f.absoluteFilePath(): " << f.absoluteFilePath();
        qDebug() << "dir1.absolutePath(): " << dir1.absolutePath();
        qDebug() << "dir1.path(): " << dir1.path();
        if (QFile::copy(f.absoluteFilePath(), dir1.absolutePath() + "/" + f.fileName())) {
            QFile::remove(f.absoluteFilePath());
        }
    } else {
        if (QFile::copy(dir1.absolutePath() + "/" + f.fileName(),
                        dir0.absolutePath() + "/" + f.fileName())) {
            QFile::remove(dir1.absolutePath() + "/" + f.fileName());
        }
    }
    return 0;
}