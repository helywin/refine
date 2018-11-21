//
// Created by jiang.wenqiang on 2018/10/22.
//

#ifndef REFINE_FILEPICKER_HPP
#define REFINE_FILEPICKER_HPP

#include <QtWidgets/QFileDialog>
#include <QtGui/QIcon>
#include "MessagerPanel.hpp"

class FilePicker : public QFileDialog
{
Q_OBJECT
public:
    /*!
     * @brief 文件对话框接收的类型
     */
    enum FileType
    {
        None = 0x0000,
        Archive = 0x0001,
        CurveConfig = 0x0002,
        CurveConfigCsv = 0x0004,
        CurveConfigSoftcan = 0x0008,
        ModeConfig = 0x0010,
        ModeConfigCsv = 0x0020,
        FrameData = 0x0040,
        FrameDataCsv = 0x0080,
        CurveData = 0x0100,
        CurveDataCsv = 0x0200,
        CurveDataSoftcan = 0x400,
        ResultData = 0x0800,
        ResultDataCsv = 0x1000,
        In = 0x10000000,
        Out = 0x20000000,
        ArchiveInFile = Archive
                        + In,
        ArchiveOutFile = Archive
                         + Out,
        CurveConfigInFile = CurveConfig
                            //+ CurveConfigCsv
                            + CurveConfigSoftcan
                            + In,
        CurveConfigOutFile = CurveConfig
                             + CurveConfigCsv
                             + Out,
        ModeConfigInFile = ModeConfig
                           //+ ModeConfigCsv
                           + In,
        ModeConfigOutFile = ModeConfig
                            + ModeConfigCsv
                            + Out,
        FrameDataInFile = FrameData
                          + In,
        FrameDataOutFile = FrameData
                           + FrameDataCsv
                           + Out,
        CurveDataInFile = CurveData
                          + CurveDataSoftcan
                          + In,
        CurveDataOutFile = CurveData
                           + CurveDataCsv
                           + Out,
        ResultDataInFile = ResultData
                           + In,
        ResultDataOutFile = ResultData
                            + Out,
    };
private:
    FileType _type;
    QString _last_filter;
public:
    explicit FilePicker(QWidget *parent = nullptr);
    static QStringList extendNames(unsigned int type);

    static QString extendName(unsigned int type);

    static QStringList extendNameWithStr(unsigned int type);

    void loadArchive();
    void loadCurveConfig();
    void loadFrameData();
    void loadCurveData();
    void loadModeConfig();
    void saveArchive();
    void saveCurveConfig();
    void saveFrameData();
    void saveCurveData();
    void saveModeConfig();
public slots:
    void showDialog();
private:
    void setup();
private slots:

    void fileSelectedSlot(const QString &file);
signals:
    void message(MessagerPanel::MessageType type, const QString &msg);
    void pickFile(int type, const QString &file, const QString &suffix);
};


#endif //REFINE_FILEPICKER_HPP
