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
        message(MessagerPanel::Warning, tr("开始失败，确保CAN连接好再采集"));
        return false;
    }
    if (_status != Stop) {
        message(MessagerPanel::Warning, tr("开始失败，已经在采集了"));
        return false;
    }
    _can.clear();
    _msec = msec;
    _config = config;
    _time = time;
    _collect.setParams(&_can, &_buffer, Collect::FromCan, msec);
    genName();
    if ((unsigned) _config & (unsigned) Config::WithTransform) {
        if (!_curve.isInitialized()) {
            message(MessagerPanel::Warning, tr("开始失败，没有加载曲线配置"));
            return false;
        }
        _tribe.genFromCurve(_curve);
        _tribe_model->genData(&_tribe);
        genCurveDataFile();
        _transform.setParams(&_curve, &_buffer, &_tribe, msec);
        _collect.begin();
        _transform.begin();
    } else {
        _collect.begin();
    }
    if (_sketch) {
        _sketch->start();
    }
    if ((unsigned) _config & (unsigned) WithRecord) {
        genFramesDataFile();
        _record.setParams(&_store_frames, &_buffer);
        _record.begin();
    }
    if ((unsigned) _config & (unsigned) WithTiming) {
        _timer_stop.start();
    }
    if ((unsigned) _config & (unsigned) WithTrigger) {}
    _status = Running;
    message(MessagerPanel::Info, tr("开始采集成功"));
    return true;
}

bool Revolve::stop(bool error)
{
    if (_status == Stop) {
        message(MessagerPanel::Warning, tr("结束失败，采集还没开始"));
        return false;
    }
    if (!error) {
        _collect.stop();
    } else {
        _menu_init_can->setChecked(false);
        _can.close();
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
    }
    if ((unsigned) _config & (unsigned) WithRecord) {
        _record.stop();
        QString name = _store_frames.fileName();
        files << _store_frames_name;
        paths << name;
    }
    if (_sketch) {
        _sketch->stop();
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
        message(MessagerPanel::Info, tr("生成数据 ") + _store_archive_name);
    } else {
        message(MessagerPanel::Critical, tr("包文件生成失败"));
    }
    _status = Stop;
    message(MessagerPanel::Info, tr("停止采集成功"));
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
    message(MessagerPanel::Info, tr("采集异常，安全停止"));
    return true;
}

void Revolve::pause()
{
    if (_status == Pause) {
        message(MessagerPanel::Warning, tr("暂停失败，采集已经暂停"));
        return;
    }
    if (_status == Stop) {
        message(MessagerPanel::Warning, tr("暂停失败，采集已经停止了"));
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
    message(MessagerPanel::Info, tr("暂停采集成功"));
}

void Revolve::resume()
{
    if (_status == Running) {
        message(MessagerPanel::Warning, tr("继续失败，采集没有暂停"));
        return;
    }
    if (_status == Stop) {
        message(MessagerPanel::Warning, tr("继续失败，采集已经停止了"));
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
    message(MessagerPanel::Info, tr("继续采集成功"));
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

void Revolve::getFile(int type, const QString &file, const QString &suffix)
{
    qDebug() << file;
    qDebug() << suffix;
    if (suffix.isEmpty()) {
        emit message(MessagerPanel::Fatal, tr("读取的文件不带扩展名"));
    }
    switch (type) {
        case FilePicker::ArchiveInFile:
            break;
        case FilePicker::ArchiveOutFile:
            break;
        case FilePicker::CurveConfigInFile:
            if (suffix == FilePicker::extendName(FilePicker::CurveConfigCsv)) {
                if (importCsvCurveConfig(file)) {
                    emit message(MessagerPanel::Info,
                                 tr("载入csv曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emit message(MessagerPanel::Warning,
                                 tr("载入csv曲线配置失败，检查配置格式"));
                }
            } else if (suffix ==
                       FilePicker::extendName(FilePicker::CurveConfig)) {
                if (inputCurveConfig(file)) {
                    emit message(MessagerPanel::Info,
                                 tr("载入曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emit message(MessagerPanel::Warning,
                                 tr("载入曲线配置失败，检查配置格式"));
                }
            } else if (suffix == FilePicker::extendName(
                    FilePicker::CurveConfigSoftcan)) {
                if (importSoftcanCurveConfig(file)) {
                    emit message(MessagerPanel::Info,
                                 tr("载入SoftCAN曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emit message(MessagerPanel::Warning,
                                 tr("载入SoftCAN曲线配置失败，检查配置格式"));
                }
            } else {
                emit message(MessagerPanel::Fatal,
                             tr("载入的曲线配置扩展名超出预料"));
            }
            break;
        case FilePicker::CurveConfigOutFile:
            if (suffix == FilePicker::extendName(FilePicker::CurveConfigCsv)) {
                if (exportCsvCurveConfig(file)) {
                    emit message(MessagerPanel::Info,
                                 tr("导出csv曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emit message(MessagerPanel::Warning,
                                 tr("导出csv曲线配置失败"));
                }
            } else if (suffix == FilePicker::extendName(FilePicker::CurveConfig)) {
                if (outputCurveConfig(file)) {
                    emit message(MessagerPanel::Info,
                                 tr("导出曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emit message(MessagerPanel::Warning,
                                 tr("导出曲线配置失败"));
                }
            } else {
                emit message(MessagerPanel::Fatal,
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
                    emit message(MessagerPanel::Info,
                                 tr("载入曲线数据成功"));
                } else {
                    emit message(MessagerPanel::Warning,
                                 tr("载入曲线数据失败，检查配置格式"));
                }
            } else {
                emit message(MessagerPanel::Fatal,
                             tr("载入曲线数据扩展名超出预料"));
            }
            break;
        case FilePicker::CurveDataOutFile:
            if (suffix == FilePicker::extendName(
                    FilePicker::CurveData)) {
                if (outputCurveData(file)) {
                    emit message(MessagerPanel::Info,
                                 tr("保存曲线数据成功"));
                } else {
                    emit message(MessagerPanel::Warning,
                                 tr("保存曲线数据失败"));
                }
            } else if (suffix == FilePicker::extendName(
                        FilePicker::CurveDataCsv)) {
                if (exportCsvCurveData(file)) {
                    emit message(MessagerPanel::Info,
                                 tr("保存CSV曲线数据成功"));
                } else {
                    emit message(MessagerPanel::Warning,
                                 tr("保存CSV曲线数据失败"));
                }
            }
            else {
                emit message(MessagerPanel::Fatal,
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
    return file.loadCurveConfig(f, _curve);
}

bool Revolve::importCsvCurveConfig(const QString &name)
{
    return _curve.loadFromCsv(name);
}

bool Revolve::importSoftcanCurveConfig(const QString &name)
{
    bool flag = _softcan.load(name);
    _softcan.toCurve(_curve);
    return flag;
}

bool Revolve::outputCurveConfig(const QString &name)
{
    File file;
    QFile f(name);
    return file.dumpCurveConfig(f, _curve);
}

bool Revolve::exportCsvCurveConfig(const QString &name)
{
    return _curve.dumpToCsv(name);
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
    _sketch->initData();
    _tribe_model->genData(&_tribe);
    return true;
}

bool Revolve::outputCurveData(const QString &name)
{
    File file;
    QFile f(name);
    return file.dumpCurveRecord(f, _tribe);
}

bool Revolve::exportCsvCurveData(const QString &name)
{
    return _tribe.dumpToCsv(name);
}

void Revolve::collectError(int code)
{
    if (code == Collect::ErrorConnection) {
        message(MessagerPanel::Critical, tr("检测到连接已经断开，停止采集"));
//        canLostConnection();
        if (stop(true)) {
            collectMenuEnable(false);
        }
    } else if (code == Collect::WarnNoFrame) {
        message(MessagerPanel::Warning, tr("采集不到报文"));
    }
}

void Revolve::transformError(int code)
{

}

void Revolve::recordError(int code)
{

}


