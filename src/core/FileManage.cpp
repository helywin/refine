//
// Created by jiang.wenqiang on 2018/11/22.
//

#include <QtCore/QDateTime>
#include "FileManage.hpp"
#include "FilePicker.hpp"

FileManage::FileManage(Initializer *init, Message *message) :
        Message(message)
{
//    _cache_dir.setPath(init->get(Initializer::GroupIndex::Core,
//                                 Initializer::KeyIndex::TempDir).toString());
//    _work_dir.setPath(init->get(Initializer::GroupIndex::Core,
//                                Initializer::KeyIndex::DataDir).toString());
//    _format = init->get(Initializer::GroupIndex::Core,
//                        Initializer::KeyIndex::NameFormat).toString();
    _work_dir.setPath("data");
    _cache_dir.setPath("temp");
    _format = "yyyy-MM-dd@hh-mm-ss";
    if (!_work_dir.exists()) {
        qDebug() << "创建工作目录";
        _work_dir.mkpath(_work_dir.absolutePath());
    }
    if (!_cache_dir.exists()) {
        qDebug() << "创建缓存目录";
        _cache_dir.mkpath(_cache_dir.absolutePath());
    }
    _cache_dir.setFilter(QDir::Dirs | QDir::Files);
    if (!QFile::exists(Z7ZIP)) {
        qDebug() << "找不到bin目录压缩软件";
    } else {
        _process.setProgram(Z7ZIP);     //弄个32位的放在目录下面
    }
    connect(&_process, static_cast<void (QProcess::*)
                    (int exitCode, QProcess::ExitStatus exitStatus)>
            (&QProcess::finished),
            this, &FileManage::processExit);
}

void FileManage::generate()
{
    QString name = QDateTime::currentDateTime().toString(_format);
    name += ".";
    qDebug() << _format;
    _curve_config.setFile(_work_dir, name + FilePicker::extendName(FilePicker::CurveConfig));
    _curve_data.setFile(_work_dir, name + FilePicker::extendName(FilePicker::CurveData));
    _frame_data.setFile(_work_dir, name + FilePicker::extendName(FilePicker::FrameData));
    _mode_config.setFile(_work_dir, name + FilePicker::extendName(FilePicker::ModeConfig));
    _archive.setFile(_work_dir, name + FilePicker::extendName(FilePicker::Archive));

    _cache_curve_config.setFile(_cache_dir, name + FilePicker::extendName(FilePicker::CurveConfig));
    _cache_curve_data.setFile(_cache_dir, name + FilePicker::extendName(FilePicker::CurveData));
    _cache_frame_data.setFile(_cache_dir, name + FilePicker::extendName(FilePicker::FrameData));
    _cache_mode_config.setFile(_cache_dir, name + FilePicker::extendName(FilePicker::ModeConfig));
    _cache_archive.setFile(_cache_dir, name + FilePicker::extendName(FilePicker::Archive));
    qDebug() << _curve_config.absoluteFilePath();
    qDebug() << _curve_data.absoluteFilePath();
    qDebug() << _frame_data.absoluteFilePath();
    qDebug() << _mode_config.absoluteFilePath();
    qDebug() << _archive.absoluteFilePath();

}

void FileManage::copyToCache()
{
    int j = _cache_dir.count();
    for (int i = 0; i < j; ++i) {
        _cache_dir.remove(_cache_dir[i]);
    }

    copy(_curve_config, _cache_curve_config, tr("复制曲线配置失败"));
    copy(_curve_data, _cache_curve_data, tr("复制曲线数据失败"));
    copy(_frame_data, _cache_frame_data, tr("复制报文数据失败"));
    copy(_mode_config, _cache_mode_config, tr("复制工况配置失败"));
    copy(_archive, _cache_archive, tr("复制包文件失败"));

}

void FileManage::move(const QFileInfo &info, const QFileInfo &cache, const QString &msg)
{
    if (info.exists()) {
        if (QFile::copy(info.absoluteFilePath(),
                        cache.absoluteFilePath())) {
            QFile::remove(info.absoluteFilePath());
        } else {
            emitMessage(Re::Critical, QString("%1 %2")
                    .arg(msg)
                    .arg(info.fileName()));
        }
    }
}

void FileManage::copy(const QFileInfo &info, const QFileInfo &cache, const QString &msg)
{
    if (info.exists()) {
        if (!QFile::copy(info.absoluteFilePath(),
                         cache.absoluteFilePath())) {
            emitMessage(Re::Critical, QString("%1 %2")
                    .arg(msg)
                    .arg(info.fileName()));
        }
    }
}

void FileManage::compress()
{
    _process.setWorkingDirectory(_work_dir.absolutePath());
    QStringList file_list;
    file_list << _curve_config.absoluteFilePath()
              << _curve_data.absoluteFilePath()
              << _frame_data.absoluteFilePath();
    QStringList args;
    args << "a"
         << "-tzip"
         << "-sdel"
         << _archive.absoluteFilePath();
    args << file_list;
    args << QString("-p<%1>").arg(PASSWORD, 10, QChar('.'));
    qDebug() << args.join(' ');
    _process.setArguments(args);
    _process.start();
}

void FileManage::uncompress(const QString &file)
{
    _process.setWorkingDirectory(_cache_dir.absolutePath());
    QStringList args;
    args << "e"
         << "-tzip"
         << "-aoa"
         << file
         << "-o" + _cache_dir.absolutePath();
    args << QString("-p<%1>").arg(PASSWORD, 10, QChar('.'));
//    qDebug() << args.join(' ');
    _process.setArguments(args);
    _process.start();
}

void FileManage::processExit(int exitCode, QProcess::ExitStatus exitStatus)
{

}
