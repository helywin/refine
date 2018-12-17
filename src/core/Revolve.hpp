/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 底层调度
 * @details 底层功能模块全部由Revolve实现调度，GUI不参与任何变量初始化和传递
 ******************************************************************************/

#ifndef REFINE_REVOLVE_HPP
#define REFINE_REVOLVE_HPP

#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtCore/QThread>
#include <QtWidgets/QAction>
#include "Global.hpp"
#include "Can.hpp"
#include "Curve.hpp"
#include "File.hpp"
#include "Softcan.hpp"
#include "Transform.hpp"
#include "Tribe.hpp"
#include "Trigger.hpp"
#include "Buffer.hpp"
#include "Collect.hpp"
#include "Csv.hpp"
#include "Record.hpp"
#include "Initializer.hpp"
#include "Transmit.hpp"
#include "Initializer.hpp"
#include "MessagerPanel.hpp"
#include "CurvePanel.hpp"
#include "TribeModel.hpp"
#include "Message.hpp"
#include "FileManage.hpp"
/*!
 * @brief 底层调度类
 * 外部GUI只传文件名进来
 */
class Sketch;
class CurveEditor;

class Revolve : public QObject, public Message
{
Q_OBJECT
public:

    enum Config
    {
        WithTransform = 0x01,
        WithRecord = 0x02,
        WithTrigger = 0x4,
        WithTiming = 0x8
    };

private:
    Initializer *_init;
    FileManage _manage;
    Can _can;
    Curve _curve;
    Buffer _buffer;
    Collect _collect;
    Transmit _transmit;
    Tribe _tribe;
    Transform _transform;
    Record _record;
    Softcan _softcan;           //! \brief softcan配置转换工具
    File _file;
    QTimer _timer_stop;                     //! \brief 采样时钟
    unsigned long _msec;                    //! \brief 采样周期
    int _time;                              //! \brief 自动停止时间
    int _config;
    Re::RunningStatus _status;

    Sketch *_sketch;
    TribeModel *_tribe_model;
    CurveEditor *_curve_editor;
    QAction *_menu_init_can;


public:
    explicit Revolve(Initializer *init);

    ~Revolve() final;

    //Can配置
    inline Can::Config &canConfig() { return _can.config(); }

public slots:
    //采集
    bool begin(unsigned long msec, int config, int time);
    void pause();
    void resume();

    inline bool stop() { return stop(false); }

    bool stop(bool error);
    bool exit();
public:
    //采集配置
    void setCollectManner(Collect::Manner manner, QString &collect_frame);

    //文件导入导出
    bool inputCurveConfig(const QString &name);
    bool outputCurveConfig(const QString &name);

    bool importCsvCurveConfig(const QString &name);

    bool importSoftcanCurveConfig(const QString &name);

    bool exportCsvCurveConfig(const QString &name);

    bool outputFrameData(const QString &name);
    bool exportCsvFrameData(const QString &name);

    bool inputFrameData(const QString &name);
    bool importCsvFrameData(const QString &name);

    bool inputCurveData(const QString &name);
    bool importSoftcanCurveData(const QString &name);
    bool outputCurveData(const QString &name);
    bool exportCsvCurveData(const QString &name);

    inline Can &can() { return _can; }

    inline Tribe &tribe() { return _tribe; }

    inline Curve &curve() { return _curve; }

    inline void setSketch(Sketch *sketch) { _sketch = sketch; }

    inline bool finished() const { return _status == Re::Stop; }

    inline void setTribeModel(TribeModel *model) { _tribe_model = model; }

    inline void setCurveEditor(CurveEditor *editor) { _curve_editor = editor; }

    inline void setActionCan(QAction *action) { _menu_init_can = action; }

protected:
    inline void emitMessage(MessageType type, const QString &msg) override
    {
        emit message(type, msg);
    }

public slots:

    void getFile(int type, const QString &file, const QString &suffix);

    void collectError(int code);

    void transformError(int code);

    void recordError(int code);

signals:
    void message(int type, const QString &msg);

    void curveLoaded();

    void canLostConnection();

    void collectMenuEnable(bool isCollecting);

    void baudRate(double baud_rate);

    void resetHScroll(int size, bool reset);
};


#endif //REFINE_REVOLVE_HPP
