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
#include "Initializer.hpp"
#include "Messager.hpp"

/*!
 * @brief 底层调度类
 * 外部GUI只传文件名进来
 */
class Sketch;

class Revolve : public QObject
{
Q_OBJECT
public:
    enum Status
    {
        Stop,
        Running,
        Pause
    };

    enum Config
    {
        WithTransform = 0x01,
        WithRecord = 0x02,
        WithTrigger = 0x4,
        WithTiming = 0x8
    };

private:
    Initializer *_init;
    Can _can;
    Curve _curve;
    Buffer _buffer;
    Collect _collect;
    Transmit _transmit;
    Tribe _tribe;
    Transform _transform;
    Record _record;
    Softcan _softcan;           //! \brief softcan配置转换工具
    Sketch *_sketch;
    QTimer _timer_collect;      //! \brief 采样时钟
    QTimer _timer_stop;         //! \brief 自动停止的计时器
    QFile *_store_frames;       //! \brief 自动存储的报文数据
    QFile *_collect_frames;     //! \brief 从GUI读取的量
    QFile *_store_curves;       //! \brief 自动存储的曲线数据
    int _msec;      //! \brief 采样周期
    int _time;      //! \brief 自动停止时间
    int _config;
    Status _status;

public:
    explicit Revolve(Initializer *init);

    ~Revolve();
    //Can配置
    inline Can::Config &canConfig() { return _can.config(); }

public slots:
    //采集
    void begin(int msec, int config, int time);
    void pause();
    void resume();
    void stop();
public:
    //采集配置
    void setCollectManner(Collect::Manner manner, QString &collect_frame);

    //文件导入导出
    bool inputCurveConfig(const QString &name);
    bool outputCurveConfig(const QString &name);

    inline bool importCsvCurveConfig(const QString &name)
    {
        if (_curve.loadFromCsv(name)) {
            genTribe();
            return true;
        } else {
            return false;
        }
    }

    inline bool importSoftcanCurveConfig(const QString &name)
    {
        if (_softcan.load(name)) {
            _softcan.toCurve(_curve);
            genTribe();
            return true;
        } else {
            return false;
        }
    }

    inline bool exportCsvCurveConfig(const QString &name)
    {
        return _curve.dumpToCsv(name);
    }

    bool outputFrameData(const QString &name);
    bool exportCsvFrameData(const QString &name);

//    bool inputFrameData(const QString &name) = delete;
//    bool importCsvFrameData(const QString &name) = delete;

    bool inputCurveData(const QString &name);
    bool outputCurveData(const QString &name);
    bool exportCsvCurveData(const QString &name);

    inline Can &can() { return _can; }

    inline Tribe &tribe() { return _tribe; }

    inline Curve &curve() { return _curve; }

    inline void setSketch(Sketch *sketch) { _sketch = sketch; }

private:
    //生成临时存储文件
    void genFramesDataFile();

    void genCurveDataFile();

    void genTribe();

public slots:
    void tictoc();

    void CollectError(int code) {}

    void TransformError(int code) {}

    void RecordError(int code) {}

signals:
    void message(Messager::MessageType type, const QString &msg);
};


#endif //REFINE_REVOLVE_HPP
