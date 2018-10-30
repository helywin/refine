/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 底层调度
 * @details 底层功能模块全部由Revolve实现调度，GUI不参与任何变量初始化和传递
 ******************************************************************************/

#include "Revolve.hpp"
#include "Sketch.hpp"
#include "FilePicker.hpp"

Revolve::Revolve(Initializer *init) :
        _init(init),
        _can(),
        _curve(),
        _buffer(100),
        _collect(),
        _tribe(),
        _transform(),
        _record(),
        _sketch(nullptr),
        _softcan(),
        _timer_stop(),
        _store_frames(),
        _collect_frames(),
        _store_curves(),
        _msec(10),
        _time(10),
        _config(),
        _status(Stop)
{
    _config = 0x00;
    connect(&_timer_stop, &QTimer::timeout, this, &Revolve::stop,
            Qt::DirectConnection);
    connect(&_collect, &Collect::error, this, &Revolve::collectError,
            Qt::DirectConnection);
}

Revolve::~Revolve() {}

void Revolve::begin(int msec, int config, int time)
{
    if (!_can.isConnected()) {
        message(Messager::Warning, tr("开始失败，确保CAN连接好再采集"));
        return;
    }
    if (_status != Stop) {
        message(Messager::Warning, tr("开始失败，已经在采集了"));
        return;
    }
    _msec = msec;
    _config = config;
    _time = time;
    _buffer.reset();
    _tribe.reset();
    _collect.begin(&_can, &_buffer, Collect::FromCan);
    if ((unsigned) _config & (unsigned) Config::WithTransform) {
        if (!_curve.isInitialized()) {
            message(Messager::Warning, tr("开始失败，没有加载曲线配置"));
            return;
        }
        _tribe.reset();
        genCurveDataFile();
        _transform.begin(&_curve, &_buffer, &_tribe);

    }
    if ((unsigned) _config & (unsigned) WithRecord) {
        genFramesDataFile();
        _record.begin(&_store_frames, &_buffer);
    }
    if ((unsigned) _config & (unsigned) WithTiming) {
        _timer_stop.start();
    }
    if (_sketch) {
        _sketch->start();
    }
    if ((unsigned) _config & (unsigned) WithTrigger) {}
    _can.clear();
    _cmd = None;
    start(QThread::HighestPriority);
    _status = Running;
    message(Messager::Info, tr("开始采集成功"));
}

void Revolve::stop()
{
    if (_status == Stop) {
        message(Messager::Warning, tr("结束失败，采集还没开始"));
        return;
    }
    _cmd = CommandStop;
    msleep(10);
    while (_collect.isRunning()) {}
    if ((unsigned) _config & (unsigned) WithTransform) {
        while (_transform.isRunning()) {}
        _transform.finish(&_store_curves);
    }
    _collect.finish();
    if ((unsigned) _config & (unsigned) WithRecord) {
        while (_record.isRunning()) {}
        _record.finish();
    }
    if ((unsigned) _config & (unsigned) WithTiming) {
        _timer_stop.stop();
    }
    if ((unsigned) _config & (unsigned) WithTrigger) {}
    if (_sketch) {
        _sketch->stop();
    }
    while (isRunning()) {}
    _status = Stop;
    message(Messager::Info, tr("停止采集成功"));
}

void Revolve::pause()
{
    if (_status == Pause) {
        message(Messager::Warning, tr("暂停失败，采集已经暂停"));
        return;
    }
    if (_status == Stop) {
        message(Messager::Warning, tr("暂停失败，采集已经停止了"));
        return;
    }
    _cmd = CommandPause;
    if ((unsigned) _config & (unsigned) WithTiming) {
        _time = _timer_stop.remainingTime();
        _timer_stop.stop();
    }
    if (_sketch) {
        _sketch->pause();
    }
    _cmd = CommandPause;
    message(Messager::Info, tr("暂停采集成功"));
}

void Revolve::resume()
{
    if (_status == Running) {
        message(Messager::Warning, tr("继续失败，采集没有暂停"));
        return;
    }
    if (_status == Stop) {
        message(Messager::Warning, tr("继续失败，采集已经停止了"));
        return;
    }
    _cmd = CommandResume;
    if ((unsigned) _config & (unsigned) WithTiming) {
        _time = _timer_stop.remainingTime();
        _timer_stop.stop();
    }
    if (_sketch) {
        _sketch->resume();
    }
    _cmd = CommandResume;
    message(Messager::Info, tr("继续采集成功"));
}

void Revolve::setCollectManner(Collect::Manner manner, QString &collect_frame)
{
    _collect_frames.setFileName(collect_frame);
    _collect.setParams(&_can, &_buffer, manner, &_collect_frames);
}

void Revolve::genFramesDataFile()
{_store_frames.close();
    _store_frames.setFileName(
            QDateTime::currentDateTime()
                    .toString(_init->get(Initializer::Core,
                                         Initializer::NameFormat).toString()) +
            QString(".") +
            FilePicker::extendName(FilePicker::FrameData));
    qDebug() << "生成报文数据文件名: "
             << _store_frames.fileName();
}

void Revolve::genCurveDataFile()
{
    _store_curves.close();
    _store_curves.setFileName(
            QDateTime::currentDateTime()
                    .toString(_init->get(Initializer::Core,
                                         Initializer::NameFormat).toString()) +
            QString(".") +
            FilePicker::extendName(FilePicker::CurveData));
    qDebug() << "生成曲线数据文件名: "
             << _store_curves.fileName();
}

bool Revolve::inputCurveConfig(const QString &name)
{
    File file;
    QFile f(name);
    if (file.loadCurveConfig(f, _curve)) {
        genTribe();
        return true;
    } else {
        return false;
    }
}

bool Revolve::outputCurveConfig(const QString &name)
{
    File file;
    QFile f(name);
    return file.dumpCurveConfig(f, _curve);
}

void Revolve::genTribe()
{
    _tribe.clear();
    for (const auto &cell : _curve) {
        _tribe.append(cell.name());
    }
}

void Revolve::run()
{
    while (true) {
        msleep(10);
        if (_cmd == CommandStop) {
            break;
        }
        if (_cmd == CommandPause) {
            _status = Pause;
            continue;
        }
        if (_cmd == CommandResume) {
            _status = Running;
        }
#if 0   //测量时钟周期
        static QTime t = QTime::currentTime();
        static int i = 0;
        if (i == 99) {
            emit message(Messager::Debug,
                         QString("时钟周期: %1").arg(
                                 t.msecsTo(QTime::currentTime()) / 100));
            t = QTime::currentTime();
        }
        ++i;
        i %= 100;
#endif
        _collect.start();
        if ((unsigned) _config & (unsigned) WithTransform) {
            if (_transform.isRunning()) {
                emit message(Messager::Warning, tr("曲线转换跟不上"));
            } else {
                _transform.start(QThread::HighestPriority);
            }
        }
        if ((unsigned) _config & (unsigned) WithRecord) {
            if (_record.isRunning()) {
                emit message(Messager::Warning, tr("报文存储跟不上"));
            } else {
                _record.start();
            }
        }
    }
}

void Revolve::collectError(int code)
{
    if (code == Collect::ConnectionLost) {
        message(Messager::Critical, tr("检测到连接已经断开，停止采集"));
        stop();
    } else if (code == Collect::NoFrame) {
        message(Messager::Warning, tr("采集不到报文"));
    }
}

void Revolve::transformError(int code)
{

}

void Revolve::recordError(int code)
{

}


