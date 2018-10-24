//
// Created by jiang.wenqiang on 2018/10/22.
//

#ifndef REFINE_FILEPICKER_HPP
#define REFINE_FILEPICKER_HPP

#include <QtWidgets/QFileDialog>
#include <QtGui/QIcon>

class FilePicker : public QFileDialog
{
Q_OBJECT
public:
    /*!
     * @brief 文件对话框接收的类型
     */
    enum FileType
    {
        Archive = 0x0001,
        CurveConfig = 0x0002,
        CurveConfigCsv = 0x0004,
        ModeConfig = 0x0008,
        ModeConfigCsv = 0x0010,
        FrameData = 0x0020,
        FrameDataCsv = 0x0040,
        CurveData = 0x0080,
        CurveDataCsv = 0x0100,
        ResultData = 0x0200,
        ResultDataCsv = 0x0400
    };
private:
public:
    explicit FilePicker(QWidget *parent = nullptr);
    static QStringList extendName(unsigned int type);

    static QStringList extendNameWithStr(unsigned int type);
    QString getCurveConfig();
private:
    void setup();
};


#endif //REFINE_FILEPICKER_HPP
