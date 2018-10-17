/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 底层调度
 * @details 底层功能模块全部由Revolve实现调度，GUI不参与任何变量初始化和传递
 ******************************************************************************/

#ifndef REFINE_REVOLVE_HPP
#define REFINE_REVOLVE_HPP

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QFile>
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
#include "Despatch.hpp"
#include "Initializer.hpp"
#include "Transmit.hpp"

/*!
 * @brief 底层调度类
 * 外部GUI只传文件名进来
 */

class Revolve : public QObject
{
Q_OBJECT
public:
    enum Status
    {
        Idle = 0x00,
        Collecting = 0x01,
        Timing = 0x02,
        //todo
    };
private:
    Can _can;
    Curve _curve;
    Buffer _buffer;
    Collect _collect;
    Transmit _transmit;
    Tribe _tribe;
    Transform _transform;
    Record _record;
    Softcan _softcan;
    QTimer _timer_collect;
    QTimer _timer_stop;
    QFile *_store_frames;
    QFile *_collect_frames;     //从GUI读取的量
    QFile *_store_curves;
    int _msec;
    int _time;
    bool _is_transform;
    bool _is_record;

public:
    Revolve();

    //Can配置
    inline Can::Config &canConfig() { return _can.config(); }

public slots:
    //采集
    void begin(int msec = 10, bool is_transform = true,
               bool is_record = true, int time = 0);
    void pause();
    void resume();
    void stop();
public:
    //采集配置
    bool setCollectManner(Collect::Manner manner, QString &collect_frame);

    inline void setCollectMsec(int msec) {}

    //文件导入导出
    bool inputCurveConfig(const QString &name);
    bool outputCurveConfig(const QString &name);

    bool importCsvCurveConfig(const QString &name);
    bool exportCsvCurveConfig(const QString &name);

    bool outputFrameData(const QString &name);
    bool exportCsvFrameData(const QString &name);

//    bool inputFrameData(const QString &name) = delete;
//    bool importCsvFrameData(const QString &name) = delete;

    bool inputCurveData(const QString &name);
    bool outputCurveData(const QString &name);

//    bool importCsvCurveData(const QString &name) = delete;
    bool exportCsvCurveData(const QString &name);


private slots:
    void tictoc();

    void CollectError(int code);

    void TransformError(int code);

    void RecordError(int code);

};


#endif //REFINE_REVOLVE_HPP
