//
// Created by jiang.wenqiang on 2018/11/22.
//

#ifndef REFINE_FILEMANAGE_HPP
#define REFINE_FILEMANAGE_HPP

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include "Initializer.hpp"
#include "Message.hpp"

class FileManage : public QObject, public Message
{
Q_OBJECT
public:
    enum Error
    {
        CompressExitStatus,
        CompressExitCode,
        UncompressExitStatus,
        UncompressExitCode
    };
private:
    const QString PASSWORD = tr("by_Refine");
    const QString Z7ZIP = "bin/7zip/7z.exe";
    QFileInfo _curve_config;
    QFileInfo _curve_data;
    QFileInfo _frame_data;
    QFileInfo _mode_config;
    QFileInfo _archive;
    QFileInfo _cache_curve_config;
    QFileInfo _cache_curve_data;
    QFileInfo _cache_frame_data;
    QFileInfo _cache_mode_config;
    QFileInfo _cache_archive;
    QDir _work_dir;
    QDir _cache_dir;
    QString _format;
    QProcess _process;

public:
    explicit FileManage(Initializer *init, Message *message);

    void generate();

    inline const QFileInfo &curveConfig() const { return _curve_config; }

    inline const QFileInfo &curveData() const { return _curve_data; }

    inline const QFileInfo &frameData() const { return _frame_data; }

    inline const QFileInfo &modeConfig() const { return _mode_config; }

    inline const QFileInfo &archive() const { return _archive; }

    inline const QFileInfo &cacheCurveConfig() const { return _cache_curve_config; }

    inline const QFileInfo &cacheCurveData() const { return _cache_curve_data; }

    inline const QFileInfo &cacheFrameData() const { return _cache_frame_data; }

    inline const QFileInfo &cacheModeConfig() const { return _cache_mode_config; }

    inline const QFileInfo &cacheArchive() const { return _cache_archive; }

    inline const QDir &workDir() const { return _work_dir; }

    inline const QDir &cacheDir() const { return _cache_dir; }

    void copyToCache();

public slots:
    void compress();

    void uncompress(const QString &file);

private:
    void move(const QFileInfo &info, const QFileInfo &cache, const QString &msg);

    void copy(const QFileInfo &info, const QFileInfo &cache, const QString &msg);

private slots:
    void processExit(int exitCode, QProcess::ExitStatus exitStatus);

signals:
    void error(int error);
};


#endif //REFINE_FILEMANAGE_HPP
