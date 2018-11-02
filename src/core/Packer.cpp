//
// Created by jiang.wenqiang on 2018/11/1.
//

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include "Packer.hpp"

Packer::Packer(Initializer *init)
{
    _process.setProgram("bin/7zip/7z.exe");     //弄个32位的放在目录下面
    _init = init;
}

bool Packer::compress(const QStringList &file_list, const QString &dist_name)
{
    _process.setWorkingDirectory(
            _init->get(Initializer::Core, Initializer::DataDir).toString());
    QStringList args;
    args << "a"
         << "-tzip"
         //         << "-sdel"
         << dist_name;
    args << file_list;
    args << "-p" + QString(PASSWORD);
    qDebug() << args.join(' ');
    _process.setArguments(args);
    _process.start();
    _process.waitForFinished();     //阻塞式设计
    if (_process.exitStatus() != QProcess::NormalExit) {
        return false;
    } else if (_process.exitCode() != 0) {
        return false;
    }
    return true;
}

void Packer::testzip()
{
    qDebug() << "compress: " << compress({"test.pdf"}, "fra me.ref");
    qDebug() << "uncompress: " << uncompress("fra me.ref", "temp");
}

bool Packer::uncompress(const QString &file, const QString &dist_path)
{
    _process.setWorkingDirectory(
            _init->get(Initializer::Core, Initializer::TempDir).toString());
    QStringList args;
    args << "e"
         << "-tzip"
         << "-aoa"
         << file;
    if (dist_path != ".") {
        args << "-o" + dist_path;
    }
    args << "-p" + QString(PASSWORD);
    qDebug() << args.join(' ');
    _process.setArguments(args);
    _process.start();
    _process.waitForFinished();     //阻塞式设计肯定不行
    if (_process.exitStatus() != QProcess::NormalExit) {
        return false;
    } else if (_process.exitCode() != 0) {
        return false;
    }
    return true;
}
