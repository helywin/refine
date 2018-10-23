//
// Created by jiang.wenqiang on 2018/10/22.
//

#include "FileDialog.hpp"

FileDialog::FileDialog(QWidget *parent) : QFileDialog(parent)
{
    setup();
}

void FileDialog::setup()
{
    setLabelText(QFileDialog::DialogLabel::LookIn, tr("查找范围"));
    setLabelText(QFileDialog::DialogLabel::FileType, tr("文件类型"));
    setLabelText(QFileDialog::DialogLabel::Accept, tr("确定"));
    setLabelText(QFileDialog::DialogLabel::FileName, tr("文件名"));
    setLabelText(QFileDialog::DialogLabel::Reject, tr("取消"));
}

QStringList FileDialog::extendName(unsigned int type)
{
    QStringList exts;
    if (type & (unsigned int) Archive) { exts.append(".ref"); }
    if (type & (unsigned int) CurveConfig) { exts.append(".cvc"); }
    if (type & (unsigned int) CurveConfigCsv) { exts.append(".csv"); }
    if (type & (unsigned int) ModeConfig) { exts.append(".mdc"); }
    if (type & (unsigned int) ModeConfigCsv) { exts.append(".csv"); }
    if (type & (unsigned int) FrameData) { exts.append(".fmd"); }
    if (type & (unsigned int) FrameDataCsv) { exts.append(".csv"); }
    if (type & (unsigned int) CurveData) { exts.append(".cvd"); }
    if (type & (unsigned int) CurveDataCsv) { exts.append(".csv"); }
    if (type & (unsigned int) ResultData) { exts.append(".rsd"); }
    if (type & (unsigned int) ResultDataCsv) { exts.append(".csv"); }
}

QStringList FileDialog::extendNameWithStr(unsigned int type)
{
    QStringList exts;
    if (type & (unsigned int) Archive) {
        exts.append(tr("打包文件(*.ref)"));
    }
    if (type & (unsigned int) CurveConfig) {
        exts.append(tr("曲线配置(*.cvc)"));
    }
    if (type & (unsigned int) CurveConfigCsv) {
        exts.append(tr("曲线配置(*.csv)"));
    }
    if (type & (unsigned int) ModeConfig) {
        exts.append(tr("工况配置(*.mdc)"));
    }
    if (type & (unsigned int) ModeConfigCsv) {
        exts.append(tr("工况配置(*.csv)"));
    }
    if (type & (unsigned int) FrameData) {
        exts.append(tr("帧数据(*.fmd)"));
    }
    if (type & (unsigned int) FrameDataCsv) {
        exts.append(tr("帧数据(*.csv)"));
    }
    if (type & (unsigned int) CurveData) {
        exts.append(tr("曲线数据(*.cvd)"));
    }
    if (type & (unsigned int) CurveDataCsv) {
        exts.append(tr("曲线数据(*.csv)"));
    }
    if (type & (unsigned int) ResultData) {
        exts.append(tr("结果数据(*.rsd)"));
    }
    if (type & (unsigned int) ResultDataCsv) {
        exts.append(tr("结果数据(*.csv)"));
    }
    return exts;
}

QString FileDialog::getCurveConfig()
{
    this->setFilter(QDir::Filter::Files);
    this->setWindowTitle(tr("打开曲线配置"));
    this->show();
}
