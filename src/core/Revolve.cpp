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
#include "CurveEditor.hpp"

Revolve::Revolve(Initializer *init) :
        Message(),
        _init(init),
        _manage(init, this),
        _can(this),
        _curve(this),
        _buffer(100),
        _collect(this),
        _tribe(this),
        _transform(this),
        _record(this),
        _sketch(nullptr),
        _softcan(this),
        _timer_stop(),
        _msec(10),
        _time(10),
        _config(),
        _status(Stop)
{
    _config = 0x00;
    connect(&_timer_stop, &QTimer::timeout, this,
            static_cast<bool (Revolve::*)(void)>(&Revolve::stop),
            Qt::DirectConnection);
    connect(&_collect, &Collect::info, this, &Revolve::collectError,
            Qt::AutoConnection);
}

Revolve::~Revolve() {}

bool Revolve::begin(unsigned long msec, int config, int time)
{
    if (!_can.isConnected()) {
        emitMessage(Warning, tr("开始失败，确保CAN连接好再采集"));
        return false;
    }
    if (_status != Stop) {
        emitMessage(Warning, tr("开始失败，已经在采集了"));
        return false;
    }
    _can.clear();
    _msec = msec;
    _config = config;
    _time = time;
    _collect.setParams(&_can, &_buffer, Collect::FromCan, msec);
    _manage.generate();
    //! @deprecated genName();
    if ((unsigned) _config & (unsigned) Config::WithTransform) {
        if (!_curve.isInitialized()) {
            emitMessage(Warning, tr("开始失败，没有加载曲线配置"));
            return false;
        }
        _tribe.genFromCurve(_curve);
        _tribe_model->genData(&_tribe);
        //! @deprecated genCurveDataFile();
        _transform.setParams(&_curve, &_buffer, &_tribe, &_combine, _msec);
        connect(&_collect, &Collect::baudRate, this, &Revolve::baudRate);
        _collect.begin();
        _transform.begin();
    } else {
        _collect.begin();
    }
    if (_sketch) {
        _sketch->start();
    }
    if ((unsigned) _config & (unsigned) WithRecord) {
        //! @deprecated genFramesDataFile();
        _record.setParams(_manage.frameData().absoluteFilePath(), &_buffer);
        _record.begin();
    }
    if ((unsigned) _config & (unsigned) WithTiming) {
        _timer_stop.start();
    }
    if ((unsigned) _config & (unsigned) WithTrigger) {}
    _status = Running;
    emitMessage(Info, tr("开始采集成功"));
    return true;
}

bool Revolve::stop(bool error)
{
    if (_status == Stop) {
        emitMessage(Warning, tr("结束失败，采集还没开始"));
        return false;
    }
    if (!error) {
        _collect.stop();
        disconnect(&_collect, &Collect::baudRate, this, &Revolve::baudRate);
    } else {
        _menu_init_can->setChecked(false);
        _can.close();
        disconnect(&_collect, &Collect::baudRate, this, &Revolve::baudRate);
    }
    if ((unsigned) _config & (unsigned) WithTiming) {
        _timer_stop.stop();
    }
    QStringList files;
    QStringList paths;
    if ((unsigned) _config & (unsigned) WithTransform) {
        _transform.stop(_manage.curveData().absoluteFilePath());
    }
    if ((unsigned) _config & (unsigned) WithRecord) {
        _record.stop();
    }
    if (_sketch) {
        _sketch->stop();
    }
    _file.dumpCurveConfig(_manage.curveConfig().absoluteFilePath(), _curve);
    _manage.compress();
    _manage.copyToCache();
    _status = Stop;
    emitMessage(Info, tr("停止采集成功"));
    return true;
}

bool Revolve::exit()
{
    if (_sketch) {
        _sketch->stop();
    }
    if ((unsigned) _config & (unsigned) WithTiming) {
        _timer_stop.stop();
    }
    if ((unsigned) _config & (unsigned) WithTransform) {
        _transform.stop();
    }
    if ((unsigned) _config & (unsigned) WithRecord) {
        _record.stop(true);
    }
    _status = Stop;
    emitMessage(Info, tr("采集异常，安全停止"));
    return true;
}

void Revolve::pause()
{
    if (_status == Pause) {
        emitMessage(Warning, tr("暂停失败，采集已经暂停"));
        return;
    }
    if (_status == Stop) {
        emitMessage(Warning, tr("暂停失败，采集已经停止了"));
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
    emitMessage(Info, tr("暂停采集成功"));
}

void Revolve::resume()
{
    if (_status == Running) {
        emitMessage(Warning, tr("继续失败，采集没有暂停"));
        return;
    }
    if (_status == Stop) {
        emitMessage(Warning, tr("继续失败，采集已经停止了"));
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
    emitMessage(Info, tr("继续采集成功"));
}

void Revolve::setCollectManner(Collect::Manner manner, QString &collect_frame)
{
    _collect.setParams(&_can, &_buffer, manner, _msec, QString());  //不从文件采集
}

void Revolve::getFile(int type, const QString &file, const QString &suffix)
{
    qDebug() << file;
    qDebug() << suffix;
    if (suffix.isEmpty()) {
        emitMessage(Fatal, tr("读取的文件不带扩展名"));
    }
    switch (type) {
        case FilePicker::ArchiveInFile:
            break;
        case FilePicker::ArchiveOutFile:
            break;
        case FilePicker::CurveConfigInFile:
            if (suffix == FilePicker::extendName(FilePicker::CurveConfigCsv)) {
                if (importCsvCurveConfig(file)) {
                    emitMessage(Info,
                                tr("载入csv曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emitMessage(Warning,
                                tr("载入csv曲线配置失败，检查配置格式"));
                }
            } else if (suffix ==
                       FilePicker::extendName(FilePicker::CurveConfig)) {
                if (inputCurveConfig(file)) {
                    emitMessage(Info,
                                tr("载入曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emitMessage(Warning,
                                tr("载入曲线配置失败，检查配置格式"));
                }
            } else if (suffix == FilePicker::extendName(
                    FilePicker::CurveConfigSoftcan)) {
                if (importSoftcanCurveConfig(file)) {
                    emitMessage(Info,
                                tr("载入SoftCAN曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emitMessage(Warning,
                                tr("载入SoftCAN曲线配置失败，检查配置格式"));
                }
            } else {
                emitMessage(Fatal,
                            tr("载入的曲线配置扩展名超出预料"));
            }
            break;
        case FilePicker::CurveConfigOutFile:
            if (suffix == FilePicker::extendName(FilePicker::CurveConfigCsv)) {
                if (exportCsvCurveConfig(file)) {
                    emitMessage(Info,
                                tr("导出csv曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emitMessage(Warning,
                                tr("导出csv曲线配置失败"));
                }
            } else if (suffix ==
                       FilePicker::extendName(FilePicker::CurveConfig)) {
                if (outputCurveConfig(file)) {
                    emitMessage(Info,
                                tr("导出曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emitMessage(Warning,
                                tr("导出曲线配置失败"));
                }
            } else {
                emitMessage(Fatal,
                            tr("载入曲线数据扩展名超出预料"));
            }
            break;
        case FilePicker::ModeConfigInFile:
            break;
        case FilePicker::ModeConfigOutFile:
            break;
        case FilePicker::FrameDataInFile:
            break;
        case FilePicker::FrameDataOutFile:
            break;
        case FilePicker::CurveDataInFile:
            if (suffix == FilePicker::extendName(
                    FilePicker::CurveData)) {
                if (inputCurveData(file)) {
                    emitMessage(Info,
                                tr("载入曲线数据成功"));
                } else {
                    emitMessage(Warning,
                                tr("载入曲线数据失败，检查配置格式"));
                }
            } else if (suffix == FilePicker::extendName(
                    FilePicker::CurveDataSoftcan)) {
                if (importSoftcanCurveData(file)) {
                    emitMessage(Info,
                                tr("载入SoftCAN曲线数据成功"));
                } else {
                    emitMessage(Warning,
                                tr("载入SoftCAN曲线数据失败，检查配置格式"));
                }
            } else {
                emitMessage(Fatal,
                            tr("载入曲线数据扩展名超出预料"));
            }
            break;
        case FilePicker::CurveDataOutFile:
            if (suffix == FilePicker::extendName(
                    FilePicker::CurveData)) {
                if (outputCurveData(file)) {
                    emitMessage(Info,
                                tr("保存曲线数据成功"));
                } else {
                    emitMessage(Warning,
                                tr("保存曲线数据失败"));
                }
            } else if (suffix == FilePicker::extendName(
                    FilePicker::CurveDataCsv)) {
                if (exportCsvCurveData(file)) {
                    emitMessage(Info,
                                tr("保存CSV曲线数据成功"));
                } else {
                    emitMessage(Warning,
                                tr("保存CSV曲线数据失败"));
                }
            } else {
                emitMessage(Fatal,
                            tr("保存曲线数据扩展名超出预料"));
            }
            break;
        case FilePicker::ResultDataInFile:
            break;
        case FilePicker::ResultDataOutFile:
            break;
        default:
            break;
    }
}

bool Revolve::inputCurveConfig(const QString &name)
{
    File file;
    QFile f(name);
    bool flag = file.loadCurveConfig(f, _curve);
    emitMessage(Debug, tr("导入曲线配置 %1").arg(name));
    return flag;

}

bool Revolve::importCsvCurveConfig(const QString &name)
{
    bool flag = _curve.loadFromCsv(name);
    emitMessage(Debug, tr("导入CSV曲线配置 %1").arg(name));
    return flag;
}

bool Revolve::importSoftcanCurveConfig(const QString &name)
{
    bool flag = _softcan.load(name);
    _softcan.toCurve(_curve);
    emitMessage(Debug, tr("导入SoftCAN曲线配置 %1").arg(name));
    return flag;
}

bool Revolve::outputCurveConfig(const QString &name)
{
    File file;
    QFile f(name);
    bool flag = file.dumpCurveConfig(f, _curve);
    emitMessage(Debug, tr("导出曲线配置 %1").arg(name));
    return flag;
}

bool Revolve::exportCsvCurveConfig(const QString &name)
{
    bool flag = _curve.dumpToCsv(name);
    emitMessage(Debug, tr("导出CSV曲线配置 %1").arg(name));
    return flag;
}

bool Revolve::outputFrameData(const QString &name)
{
    return false;
}

bool Revolve::exportCsvFrameData(const QString &name)
{
    return false;
}

bool Revolve::inputFrameData(const QString &name)
{
    return false;
}

bool Revolve::importCsvFrameData(const QString &name)
{
    return false;
}

bool Revolve::inputCurveData(const QString &name)
{
    File file;
    QFile f(name);
    if (!file.loadCurveRecord(f, _tribe)) {
        return false;
    }
    _combine.genFromTribe(_tribe);
    _sketch->init();
    _tribe_model->genData(&_tribe);
    emitMessage(Debug, tr("导入曲线数据 %1").arg(name));
    return true;
}

bool Revolve::importSoftcanCurveData(const QString &name)
{

    if (!_softcan.load(name)) {
        return false;
    }
    _softcan.toTribe(_tribe);
    _sketch->init();
    _tribe_model->genData(&_tribe);
    emitMessage(Debug, tr("导入SoftCAN曲线数据 %1").arg(name));
    return true;
}

bool Revolve::outputCurveData(const QString &name)
{
    File file;
    QFile f(name);
    bool flag = file.dumpCurveRecord(f, _tribe);
    emitMessage(Debug, tr("导出曲线数据 %1").arg(name));
    return flag;
}

bool Revolve::exportCsvCurveData(const QString &name)
{
    bool flag = _tribe.dumpToCsv(name);
    emitMessage(Debug, tr("导入CSV曲线数据 %1").arg(name));
    return flag;
}

void Revolve::collectError(int code)
{
    if (code == Collect::ErrorConnection) {
        emitMessage(Critical, tr("检测到连接已经断开，停止采集"));
//        canLostConnection();
        if (stop(true)) {
            collectMenuEnable(false);
        }
    } else if (code == Collect::WarnNoFrame) {
        emitMessage(Warning, tr("采集不到报文"));
    }
}

void Revolve::transformError(int code)
{

}

void Revolve::recordError(int code)
{

}




