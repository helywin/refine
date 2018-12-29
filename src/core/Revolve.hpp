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
#include "RecvBuffer.hpp"
#include "Collect.hpp"
#include "Csv.hpp"
#include "Record.hpp"
#include "Initializer.hpp"
#include "Transmit.hpp"
#include "Initializer.hpp"
#include "Information.hpp"
#include "CurvePanel.hpp"
#include "TribeModel.hpp"
#include "Message.hpp"
#include "FileManage.hpp"
#include "SendBuffer.hpp"
#include "Communicate.hpp"

/*!
 * @brief 底层调度类
 * 外部GUI只传文件名进来
 */
class CurveViewer;
class CurveEditor;

class Revolve : public QObject, public Message
{
Q_OBJECT
public:
    enum ResponseType
    {
        Send,
        Receive
    };
    Q_ENUMS(ResponseType)

private:
    Initializer *_init;
    FileManage _manage;
    Can _can;
    Curve _curve;
    RecvBuffer _recv_buf;
    SendBuffer _send_buf;
    Collect _collect;
    Transmit _transmit;
    Tribe _tribe;
    Communicate _communicate;
    Transform _transform;
    Record _record;
    Softcan _softcan;           //! \brief softcan配置转换工具
    File _file;
    QTimer _timer_stop;                     //! \brief 采样时钟
    unsigned long _msec;                    //! \brief 采样周期
    int _time;                              //! \brief 自动停止时间
    Re::RevolveFlags _flags;
    Re::RunningStatus _collect_status;      //互斥的任务
    Re::RunningStatus _burning_status;      //互斥的任务

    CurveViewer *_viewer;
    TribeModel *_tribe_model;
    CurveEditor *_curve_editor;
    QAction *_menu_init_can;

    QStringList _can_message;
    bool _communicate_finished;
    bool _transmit_finished;


public:
    explicit Revolve(Initializer *init);

//    ~Revolve() = default;

    //Can配置
    inline Can::Config &canConfig()
    { return _can.config(); }

public slots:
    //采集
    bool beginCollect(unsigned long msec, Re::RevolveFlags flags, int time);
    void pauseCollect();
    void resumeCollect();
    bool stopCollect(bool error);

    inline bool stopCollect()
    { return stopCollect(false); }

    bool exitCollect();

    bool beginBurning(const QString &file, unsigned long msec = 50, int frames = 100);
    void pauseBurning();
    void resumeBurning();
    bool abortBurning();

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

    inline Can &can()
    { return _can; }

    inline Tribe &tribe()
    { return _tribe; }

    inline Curve &curve()
    { return _curve; }

    inline void setCurveViewer(CurveViewer *viewer)
    { _viewer = viewer; }

    inline bool finished() const
    { return _collect_status == Re::Stop; }

    inline void setTribeModel(TribeModel *model)
    { _tribe_model = model; }

    inline void setCurveEditor(CurveEditor *editor)
    { _curve_editor = editor; }

    inline void setActionCan(QAction *action)
    { _menu_init_can = action; }

    void sendCommand(QByteArray &&bytes);
    bool burnProgram(const QString &file);

    inline const QStringList &canMessage() const
    { return _can_message; }

    inline void clearCanMessage()
    {}

    void setTransmitMsec(unsigned long msec);
    void setTransmitFrameNum(int frame_num);

protected:
    inline void emitMessage(MessageTypes type, const QString &msg) override
    { emit message(type, msg); }

public slots:

    void getFile(int type, const QString &file, const QString &suffix);

private slots:
    void collectError(int code);
    void transformError(int code);
    void recordError(int code);
    void getTransformedCanMessage(const QString &message);

signals:
    void message(Re::MessageTypes type, const QString &msg);
    void curveLoaded();
    void canLostConnection();
    void collectMenuEnable(bool isCollecting);
    void baudRate(double baud_rate);
    void response(ResponseType type, const QString &response);
    void burningProcess(double process);
    void finishBurning();
    void getCanMessage(const QString &msg);
};

#endif //REFINE_REVOLVE_HPP
