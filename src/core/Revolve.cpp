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
        _store_curvedata(),
        _msec(10),
        _time(10),
        _config(),
        _status(Stop),
        _packer(init)
{
    _config = 0x00;
    connect(&_timer_stop, &QTimer::timeout, this, &Revolve::stop,
            Qt::DirectConnection);
    connect(&_collect, &Collect::info, this, &Revolve::collectError,
            Qt::DirectConnection);
}

Revolve::~Revolve() {}

void Revolve::begin(unsigned long msec, int config, int time)
{
    if (!_can.isConnected()) {
        message(Messager::Warning, tr("开始失败，确保CAN连接好再采集"));
        return;
    }
    if (_status != Stop) {
        message(Messager::Warning, tr("开始失败，已经在采集了"));
        return;
    }
    _can.clear();
    _msec = msec;
    _config = config;
    _time = time;
    _collect.setParams(&_can, &_buffer, Collect::FromCan, msec);
    _collect.begin();
    genName();
    if ((unsigned) _config & (unsigned) Config::WithTransform) {
        if (!_curve.isInitialized()) {
            message(Messager::Warning, tr("开始失败，没有加载曲线配置"));
            return;
        }
        genCurveDataFile();
        _transform.setParams(&_curve, &_buffer, &_tribe, msec);
        _transform.begin();
    }
    if ((unsigned) _config & (unsigned) WithRecord) {
        genFramesDataFile();
        _record.setParams(&_store_frames, &_buffer);
        _record.begin();
    }
    if ((unsigned) _config & (unsigned) WithTiming) {
        _timer_stop.start();
    }
    if (_sketch) {
        _sketch->start();
    }
    if ((unsigned) _config & (unsigned) WithTrigger) {}
    _status = Running;
    message(Messager::Info, tr("开始采集成功"));
}

void Revolve::stop()
{
    if (_status == Stop) {
        message(Messager::Warning, tr("结束失败，采集还没开始"));
        return;
    }
    if (_sketch) {
        _sketch->stop();
    }
    if ((unsigned) _config & (unsigned) WithTiming) {
        _timer_stop.stop();
    }
    QStringList files;
    QStringList paths;
    if ((unsigned) _config & (unsigned) WithTransform) {
        _transform.stop(&_store_curvedata);
        QString name = _store_curvedata.fileName();
        files << _store_curvedata_name;
        paths << name;
        name = QDir::currentPath() + "/" + name;
        message(Messager::Info, tr("曲线: ") + name);
    }
    if ((unsigned) _config & (unsigned) WithRecord) {
        _record.stop();
        QString name = _store_frames.fileName();
        files << _store_frames_name;
        paths << name;
        name = QDir::currentPath() + "/" + name;
        message(Messager::Info, tr("报文: ") + name);
    }
    genCurveConfigFile();
    paths << _store_curveconfig.fileName();
    files << _store_curveconfig_name;
    _file.dumpCurveConfig(_store_curveconfig, _curve);
    genArchiveFileName();
    if (_packer.compress(files, _store_archive_name)) {
        for (const auto &path : paths) {
            QFile::remove(path);
        }
    } else {
        message(Messager::Critical, tr("包文件生成失败"));
    }
    _collect.stop();
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
    _collect.pause();
    if ((unsigned) _config & (unsigned) WithTransform) {
        _transform.pause();
    }
    if ((unsigned) _config & (unsigned) WithRecord) {
        _record.pause();
    }
    if ((unsigned) _config & (unsigned) WithTiming) {
        _time = _timer_stop.remainingTime();
        _timer_stop.stop();
    }
    if (_sketch) {
        _sketch->pause();
    }
    _status = Pause;
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
    _collect.resume();
    if ((unsigned) _config & (unsigned) WithTransform) {
        _transform.resume();
    }
    if ((unsigned) _config & (unsigned) WithRecord) {
        _record.reusme();
    }
    if ((unsigned) _config & (unsigned) WithTiming) {
        _time = _timer_stop.remainingTime();
        _timer_stop.stop();
    }
    if (_sketch) {
        _sketch->resume();
    }
    _status = Running;
    message(Messager::Info, tr("继续采集成功"));
}

void Revolve::setCollectManner(Collect::Manner manner, QString &collect_frame)
{
    _collect_frames.setFileName(collect_frame);
    _collect.setParams(&_can, &_buffer, manner, _msec, &_collect_frames);
}

void Revolve::genName()
{
    _name = QDateTime::currentDateTime()
            .toString(_init->get(Initializer::Core,
                                 Initializer::NameFormat).toString());
}

void Revolve::genFramesDataFile()
{
    _store_frames.close();
    _store_frames_name =
            _name + "." + FilePicker::extendName(FilePicker::FrameData);
    _store_frames.setFileName(
            _init->get(Initializer::Core, Initializer::DataDir).toString() +
            _store_frames_name);
    qDebug() << "生成报文数据文件名: " << _store_frames.fileName();
}

void Revolve::genCurveDataFile()
{
    _store_curvedata.close();
    _store_curvedata_name =
            _name + "." + FilePicker::extendName(FilePicker::CurveData);
    _store_curvedata.setFileName(
            _init->get(Initializer::Core, Initializer::DataDir).toString() +
            _store_curvedata_name);
    qDebug() << "生成曲线数据文件名: " << _store_curvedata.fileName();
}

void Revolve::genCurveConfigFile()
{
    _store_curveconfig.close();
    _store_curveconfig_name =
            _name + "." + FilePicker::extendName(FilePicker::CurveConfig);
    _store_curveconfig.setFileName(
            _init->get(Initializer::Core, Initializer::DataDir).toString() +
            _store_curveconfig_name);
    qDebug() << "生成曲线配置文件名: " << _store_curveconfig.fileName();
}

void Revolve::genArchiveFileName()
{
    _store_archive_name =
            _name + "." + FilePicker::extendName(FilePicker::Archive);
    qDebug() << "生成曲线配置文件名: " << _store_archive_name;
}

bool Revolve::inputCurveConfig(const QString &name)
{
    File file;
    QFile f(name);
    if (file.loadCurveConfig(f, _curve)) {
        _tribe.genFromCurve(_curve);
        curveLoaded();
        return true;
    } else {
        return false;
    }
}

bool Revolve::importCsvCurveConfig(const QString &name)
{
    if (_curve.loadFromCsv(name)) {
        _tribe.genFromCurve(_curve);
        curveLoaded();
        return true;
    } else {
        return false;
    }
}

bool Revolve::importSoftcanCurveConfig(const QString &name)
{
    if (_softcan.load(name)) {
        _softcan.toCurve(_curve);
        _tribe.genFromCurve(_curve);
        curveLoaded();
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

void Revolve::collectError(int code)
{
    if (code == Collect::ErrorConnection) {
        message(Messager::Critical, tr("检测到连接已经断开，停止采集"));
        stop();
    } else if (code == Collect::WarnNoFrame) {
        message(Messager::Warning, tr("采集不到报文"));
    }
}

void Revolve::transformError(int code)
{

}

void Revolve::recordError(int code)
{

}


