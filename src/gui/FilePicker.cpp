//
// Created by jiang.wenqiang on 2018/10/22.
//

#include <QtCore/QStandardPaths>
#include <QtCore/QDebug>
#include "FilePicker.hpp"

FilePicker::FilePicker(QWidget *parent) :
        QFileDialog(parent), _type(None)
{
    setup();
}

void FilePicker::setup()
{
    setWindowTitle(tr("文件对话框"));
    setWindowIcon(QIcon(":res/ui/logo.ico"));
    setModal(true);
    auto flag = windowFlags();
    flag = flag | Qt::WindowStaysOnTopHint;
    setWindowFlags(flag);
    _last_url = QUrl::fromLocalFile(QStandardPaths::standardLocations(
            QStandardPaths::DesktopLocation).first());
/*
    setLabelText(QFileDialog::DialogLabel::LookIn, tr("查找范围"));
    setLabelText(QFileDialog::DialogLabel::FileType, tr("文件类型"));
    setLabelText(QFileDialog::DialogLabel::Accept, tr("确定"));
    setLabelText(QFileDialog::DialogLabel::FileName, tr("文件名"));
*/
    setLabelText(QFileDialog::DialogLabel::Reject, tr("取消"));
    setOptions(QFileDialog::DontUseNativeDialog);
    connect(this, &FilePicker::fileSelected,
            this, &FilePicker::fileSelectedSlot, Qt::DirectConnection);
}

void FilePicker::showDialog()
{
//    QList<QUrl> urls;
//    for (int i = 0; i < 2; ++i) {
//        const auto &info = QDir::drives()[i];
//        urls << QUrl::fromLocalFile(info.filePath());
//    }
//    urls << QUrl::fromLocalFile(QStandardPaths::standardLocations(
//            QStandardPaths::HomeLocation).first())
//         << QUrl::fromLocalFile(QStandardPaths::standardLocations(
//                 QStandardPaths::DesktopLocation).first());
////         << QUrl::fromLocalFile("\\\\sjnas01\\div17\\div17\\部门文件夹\\"
////                                "电控研究部\\交换数据\\测试科\\公共文件夹"
////                                "\\共享数据");
//    setSidebarUrls(urls);
    setDirectoryUrl(_last_url);
//    selectUrl(QUrl());
    show();
}

QStringList FilePicker::extendNames(FileTypes type)
{
    QStringList exts;
    if (type & Archive) {
        exts.append(extendName(Archive));
    }
    if (type & CurveConfig) {
        exts.append(extendName(CurveConfig));
    }
    if (type & CurveConfigCsv) {
        exts.append(extendName(CurveConfigCsv));
    }
    if (type & CurveConfigSoftcan) {
        exts.append(extendName(CurveConfigSoftcan));
    }
    if (type & ModeConfig) {
        exts.append(extendName(ModeConfig));
    }
    if (type & ModeConfigCsv) {
        exts.append(extendName(ModeConfigCsv));
    }
    if (type & FrameData) {
        exts.append(extendName(FrameData));
    }
    if (type & FrameDataCsv) {
        exts.append(extendName(FrameDataCsv));
    }
    if (type & CurveData) {
        exts.append(extendName(CurveData));
    }
    if (type & CurveDataCsv) {
        exts.append(extendName(CurveDataCsv));
    }
    if (type & ResultData) {
        exts.append(extendName(ResultData));
    }
    if (type & ResultDataCsv) {
        exts.append(extendName(ResultDataCsv));
    }
    return exts;
}

QString FilePicker::extendName(FileType type)
{
    switch (type) {
        case Archive:
            return QString("ref");
        case CurveConfig:
            return QString("cvc");
        case CurveConfigSoftcan:
            return QString("cfg");
        case ModeConfig:
            return QString("mdc");
        case FrameData:
            return QString("fmd");
        case CurveData:
            return QString("cvd");
        case CurveDataSoftcan:
            return QString("can");
        case ResultData:
            return QString("rsd");
        case CurveConfigCsv:
        case ModeConfigCsv:
        case FrameDataCsv:
        case CurveDataCsv:
        case ResultDataCsv:
            return QString("csv");
        default:
            return QString();
    }
}

QStringList FilePicker::extendNameWithStr(FileTypes type)
{
    QStringList exts;
    if (type & Archive) {
        exts.append(tr("打包文件") +
                    QString("(*.%1)").arg(extendName(Archive)));
    }
    if (type & CurveConfig) {
        exts.append(tr("曲线配置") +
                    QString("(*.%1)").arg(extendName(CurveConfig)));
    }
    if (type & CurveConfigCsv) {
        exts.append(tr("曲线配置") +
                    QString("(*.%1)").arg(extendName(CurveConfigCsv)));
    }
    if (type & CurveConfigSoftcan) {
        exts.append(tr("SoftCAN曲线配置") +
                    QString("(*.%1)").arg(extendName(CurveConfigSoftcan)));
    }
    if (type & ModeConfig) {
        exts.append(tr("工况配置") +
                    QString("(*.%1)").arg(extendName(ModeConfig)));
    }
    if (type & ModeConfigCsv) {
        exts.append(tr("工况配置") +
                    QString("(*.%1)").arg(extendName(ModeConfigCsv)));
    }
    if (type & FrameData) {
        exts.append(tr("帧数据") +
                    QString("(*.%1)").arg(extendName(FrameData)));
    }
    if (type & FrameDataCsv) {
        exts.append(tr("帧数据") +
                    QString("(*.%1)").arg(extendName(FrameDataCsv)));
    }
    if (type & CurveData) {
        exts.append(tr("曲线数据") +
                    QString("(*.%1)").arg(extendName(CurveData)));
    }
    if (type & CurveDataCsv) {
        exts.append(tr("曲线数据") +
                    QString("(*.%1)").arg(extendName(CurveDataCsv)));
    }
    if (type & CurveDataSoftcan) {
        exts.append(tr("SoftCAN曲线数据") +
                    QString("(*.%1)").arg(extendName(CurveDataSoftcan)));
    }
    if (type & ResultData) {
        exts.append(tr("结果数据") +
                    QString("(*.%1)").arg(extendName(ResultData)));
    }
    if (type & ResultDataCsv) {
        exts.append(tr("结果数据") +
                    QString("(*.%1)").arg(extendName(ResultDataCsv)));
    }
    return exts;
}

void FilePicker::loadArchive()
{
    setFilter(QDir::Files | QDir::NoDotAndDotDot);
    setFileMode(ExistingFile);
    setWindowTitle(tr("读取包文件"));
    _type = ArchiveInFile;
    setNameFilters(extendNameWithStr(_type));
    setAcceptMode(QFileDialog::AcceptOpen);
    showDialog();
}

void FilePicker::saveArchive()
{
    setFilter(QDir::Files | QDir::NoDotAndDotDot);
    setFileMode(AnyFile);
    setWindowTitle(tr("保存包文件"));
    _type = ArchiveOutFile;
    setNameFilters(extendNameWithStr(_type));
    setAcceptMode(QFileDialog::AcceptSave);
    showDialog();
}

void FilePicker::loadCurveConfig()
{
    setFilter(QDir::Files | QDir::NoDotAndDotDot);
    setFileMode(ExistingFile);
    setWindowTitle(tr("导入曲线配置"));
    _type = CurveConfigInFile;
    setNameFilters(extendNameWithStr(_type));
    setAcceptMode(QFileDialog::AcceptOpen);
    showDialog();
}

void FilePicker::saveCurveConfig()
{
    setFilter(QDir::Files | QDir::NoDotAndDotDot);
    setFileMode(AnyFile);
    setWindowTitle(tr("导出曲线配置"));
    _type = CurveConfigOutFile;
    setNameFilters(extendNameWithStr(_type));
    setAcceptMode(QFileDialog::AcceptSave);
    showDialog();
}

void FilePicker::loadFrameData()
{
    setFilter(QDir::Files | QDir::NoDotAndDotDot);
    setFileMode(ExistingFile);
    setWindowTitle(tr("导入报文数据"));
    _type = CurveDataInFile;
    setNameFilters(extendNameWithStr(_type));
    setAcceptMode(QFileDialog::AcceptOpen);
    showDialog();
}

void FilePicker::saveFrameData()
{
    setFilter(QDir::Files | QDir::NoDotAndDotDot);
    setFileMode(AnyFile);
    setWindowTitle(tr("导出报文数据"));
    _type = CurveDataOutFile;
    setNameFilters(extendNameWithStr(_type));
    setAcceptMode(QFileDialog::AcceptSave);
    showDialog();
}

void FilePicker::loadModeConfig()
{
    setFilter(QDir::Files | QDir::NoDotAndDotDot);
    setFileMode(ExistingFile);
    setWindowTitle(tr("导入工况配置文件"));
    _type = ModeConfigInFile;
    setNameFilters(extendNameWithStr(_type));
    setAcceptMode(QFileDialog::AcceptOpen);
    showDialog();
}

void FilePicker::saveModeConfig()
{
    setFilter(QDir::Files | QDir::NoDotAndDotDot);
    setFileMode(AnyFile);
    setWindowTitle(tr("导出数据文件"));
    _type = ModeConfigOutFile;
    setNameFilters(extendNameWithStr(_type));
    setAcceptMode(QFileDialog::AcceptSave);
    showDialog();
}

void FilePicker::loadCurveData()
{
    setFilter(QDir::Files | QDir::NoDotAndDotDot);
    setFileMode(ExistingFile);
    setWindowTitle(tr("导入曲线数据"));
    _type = CurveDataInFile;
    setNameFilters(extendNameWithStr(_type));
    setAcceptMode(QFileDialog::AcceptOpen);
    showDialog();
}

void FilePicker::saveCurveData()
{
    setFilter(QDir::Files | QDir::NoDotAndDotDot);
    setFileMode(AnyFile);
    setWindowTitle(tr("导出曲线数据"));
    _type = CurveDataOutFile;
    setNameFilters(extendNameWithStr(_type));
    setAcceptMode(QFileDialog::AcceptSave);
    showDialog();
}

void FilePicker::fileSelectedSlot(const QString &file)
{
//    this->directoryUrl().toLocalFile();
    QString name = file;
    QString suffix = QFileInfo(file).suffix();
    if (_type & Out) {
        QRegExp reg_exp("\\(\\*\\.(.*)\\)");
        int pos = selectedNameFilter().indexOf(reg_exp);
        QString suffix_temp;
        if (pos > 0) {
            suffix_temp = reg_exp.cap(1);
        } else {
            qCritical("FilePicker::fileSelectedSlot 不可预知错误");
        }
        if (suffix != suffix_temp) {
            name += "." + suffix_temp;
            suffix = suffix_temp;
        }
    }
    _last_url = QUrl::fromLocalFile(name);
    emit pickFile(_type, name, suffix);
}

