/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 底层调度
 * @details 底层功能模块全部由Revolve实现调度，GUI不参与任何变量初始化和传递
 ******************************************************************************/

#include "Revolve.hpp"
#include "CurveViewer.hpp"
#include "FilePicker.hpp"
#include "CurveEditor.hpp"

Revolve::Revolve(Initializer *init) :
        Message(),
        _init(init),
        _manage(init, this),
        _can(this),
        _curve(this),
        _recv_buf(),
        _send_buf(),
        _collect(this),
        _transmit(this),
        _tribe(this),
        _communicate(this),
        _transform(this),
        _record(this),
        _softcan(this),
        _file(this),
        _timer_stop(),
        _msec(10),
        _time(10),
        _flags(nullptr),
        _status(Re::Stop),
        _viewer(nullptr)
{
    connect(&_timer_stop, &QTimer::timeout, this,
            static_cast<bool (Revolve::*)(void)>(&Revolve::stopCollect), Qt::DirectConnection);
    connect(&_collect, &Collect::info, this, &Revolve::collectError, Qt::AutoConnection);
    connect(&_collect, &Collect::baudRate, this, &Revolve::baudRate);
    connect(&_transform, &Transform::getTransformedTcuMessage,
            this, &Revolve::getTransformedTcuMessage);
}

bool Revolve::beginCollect(unsigned long msec, Re::RevolveFlags flags, int time)
{
    if (!_can.isConnected()) {
        emitMessage(Re::Warning, tr("开始失败，确保CAN连接好再采集"));
        return false;
    }
    if (_status != Re::Stop) {
        emitMessage(Re::Warning, tr("开始失败，已经在采集了"));
        return false;
    }
    _can.clear();
    _msec = msec;
    _flags = flags;
    _time = time;
    _manage.generate();
    if (_viewer) {
        _viewer->start();
    }
    if (_flags & Re::Communicate) {
        _communicate.begin();
        _transmit.setParams(&_can, &_send_buf, 50, 100);
        _transmit.begin();
        emitMessage(Re::Debug, "Revolve::beginCollect::Communicate");
    }
    if (_flags & Re::Collect) {
        _collect.setParams(&_can, &_recv_buf, Collect::FromCan, msec);
        _collect.begin();
        emitMessage(Re::Debug, "Revolve::beginCollect::Collect");
    }
    if (_flags & Re::TransformData) {
        if (!_curve.isInitialized()) {
            emitMessage(Re::Warning, tr("没有加载曲线配，你将看不到任何东西"));
        }
        _tribe.genFromCurve(_curve);
        _tribe_model->genData(&_tribe);
        _transform.setParams(&_curve, &_recv_buf, &_tribe, _msec);
        _transform.begin();
        emitMessage(Re::Debug, "Revolve::beginCollect::TransformData");
    }
    if (_flags & Re::RecordFrame) {
        //! @deprecated genFramesDataFile();
        _record.setParams(_manage.frameData().absoluteFilePath(), &_recv_buf);
        _record.begin();
        emitMessage(Re::Debug, "Revolve::beginCollect::TransformData");
    }
    if (_flags & Re::TimingStop) {
        _timer_stop.start();
        emitMessage(Re::Debug, "Revolve::beginCollect::TimingStop");
    }
    _status = Re::Running;
    emitMessage(Re::Info, tr("开始采集成功"));
    return true;
}

bool Revolve::stopCollect(bool error)
{
    if (_status == Re::Stop) {
        emitMessage(Re::Warning, tr("结束失败，采集还没开始"));
        return false;
    }
    if (_flags & Re::Communicate) {
        _communicate.stop();
        _transmit.stop();
    }
    if (!error) {
        _collect.stop();
    } else {
        _menu_init_can->setChecked(false);
        _can.close();
    }
    if (_flags & Re::TimingStop) {
        _timer_stop.stop();
    }
    if (_flags & Re::TransformData) {
        _transform.stop(_manage.curveData().absoluteFilePath());
    }
    if (_flags & Re::RecordFrame) {
        _record.stop();
    }
    if (_viewer) {
        _viewer->stop();
    }
    _file.dumpCurveConfig(_manage.curveConfig().absoluteFilePath(), _curve);
    _manage.compress();
    _manage.copyToCache();
    _status = Re::Stop;
    emitMessage(Re::Info, tr("停止采集成功"));
    return true;
}

bool Revolve::exitCollect()
{
    if (_viewer) {
        _viewer->resume();
    }
    if (_flags & Re::TimingStop) {
        _timer_stop.stop();
    }
    if (_flags & Re::TransformData) {
        _transform.stop();
    }
    if (_flags & Re::RecordFrame) {
        _record.stop(true);
    }
    if (_flags & Re::Communicate) {
        _communicate.stop();
        _transmit.stop();
    }
    _status = Re::Stop;
    emitMessage(Re::Info, tr("采集异常，安全停止"));
    return true;
}

void Revolve::pauseCollect()
{
    if (_status == Re::Pause) {
        emitMessage(Re::Warning, tr("暂停失败，采集已经暂停"));
        return;
    }
    if (_status == Re::Stop) {
        emitMessage(Re::Warning, tr("暂停失败，采集已经停止了"));
        return;
    }
    _collect.pause();
    if (_flags & Re::TransformData) {
        _transform.pause();
    }
    if (_flags & Re::RecordFrame) {
        _record.pause();
    }
    if (_flags & Re::TimingStop) {
        _time = _timer_stop.remainingTime();
        _timer_stop.stop();
    }
    if (_viewer) {
        _viewer->pause();
    }
    _status = Re::Pause;
    emitMessage(Re::Info, tr("暂停采集成功"));
}

void Revolve::resumeCollect()
{
    if (_status == Re::Running) {
        emitMessage(Re::Warning, tr("继续失败，采集没有暂停"));
        return;
    }
    if (_status == Re::Stop) {
        emitMessage(Re::Warning, tr("继续失败，采集已经停止了"));
        return;
    }
    _collect.resume();
    if (_flags & Re::TransformData) {
        _transform.resume();
    }
    if (_flags & Re::RecordFrame) {
        _record.resume();
    }
    if (_flags & Re::TimingStop) {
        _time = _timer_stop.remainingTime();
        _timer_stop.stop();
    }
    if (_viewer) {
        _viewer->resume();
    }
    _status = Re::Running;
    emitMessage(Re::Info, tr("继续采集成功"));
}

void Revolve::setCollectManner(Collect::Manner manner, QString &collect_frame)
{
    Q_UNUSED(collect_frame)
    _collect.setParams(&_can, &_recv_buf, manner, _msec, QString());  //不从文件采集
}

void Revolve::getFile(int type, const QString &file, const QString &suffix)
{
    qDebug() << file;
    qDebug() << suffix;
    if (suffix.isEmpty()) {
        emitMessage(Re::Fatal, tr("读取的文件不带扩展名"));
    }
    switch (type) {
        case FilePicker::ArchiveInFile:
            break;
        case FilePicker::ArchiveOutFile:
            break;
        case FilePicker::CurveConfigInFile:
            if (suffix == FilePicker::extendName(FilePicker::CurveConfigCsv)) {
                if (importCsvCurveConfig(file)) {
                    emitMessage(Re::Info,
                                tr("载入csv曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emitMessage(Re::Warning,
                                tr("载入csv曲线配置失败，检查配置格式"));
                }
            } else if (suffix ==
                       FilePicker::extendName(FilePicker::CurveConfig)) {
                if (inputCurveConfig(file)) {
                    emitMessage(Re::Info,
                                tr("载入曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emitMessage(Re::Warning,
                                tr("载入曲线配置失败，检查配置格式"));
                }
            } else if (suffix == FilePicker::extendName(
                    FilePicker::CurveConfigSoftcan)) {
                if (importSoftcanCurveConfig(file)) {
                    emitMessage(Re::Info,
                                tr("载入SoftCAN曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emitMessage(Re::Warning,
                                tr("载入SoftCAN曲线配置失败，检查配置格式"));
                }
            } else {
                emitMessage(Re::Fatal,
                            tr("载入的曲线配置扩展名超出预料"));
            }
            break;
        case FilePicker::CurveConfigOutFile:
            if (suffix == FilePicker::extendName(FilePicker::CurveConfigCsv)) {
                if (exportCsvCurveConfig(file)) {
                    emitMessage(Re::Info,
                                tr("导出csv曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emitMessage(Re::Warning,
                                tr("导出csv曲线配置失败"));
                }
            } else if (suffix ==
                       FilePicker::extendName(FilePicker::CurveConfig)) {
                if (outputCurveConfig(file)) {
                    emitMessage(Re::Info,
                                tr("导出曲线配置成功"));
                    _curve_editor->updateData();
                } else {
                    emitMessage(Re::Warning,
                                tr("导出曲线配置失败"));
                }
            } else {
                emitMessage(Re::Fatal,
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
                    emitMessage(Re::Info,
                                tr("载入曲线数据成功"));
                } else {
                    emitMessage(Re::Warning,
                                tr("载入曲线数据失败，检查数据格式"));
                }
            } else if (suffix == FilePicker::extendName(
                    FilePicker::CurveDataSoftcan)) {
                if (importSoftcanCurveData(file)) {
                    emitMessage(Re::Info,
                                tr("载入SoftCAN曲线数据成功"));
                } else {
                    emitMessage(Re::Warning,
                                tr("载入SoftCAN曲线数据失败，检查配置格式"));
                }
            } else {
                emitMessage(Re::Fatal,
                            tr("载入曲线数据扩展名超出预料"));
            }
            break;
        case FilePicker::CurveDataOutFile:
            if (suffix == FilePicker::extendName(
                    FilePicker::CurveData)) {
                if (outputCurveData(file)) {
                    emitMessage(Re::Info,
                                tr("保存曲线数据成功"));
                } else {
                    emitMessage(Re::Warning,
                                tr("保存曲线数据失败"));
                }
            } else if (suffix == FilePicker::extendName(
                    FilePicker::CurveDataCsv)) {
                if (exportCsvCurveData(file)) {
                    emitMessage(Re::Info,
                                tr("保存CSV曲线数据成功"));
                } else {
                    emitMessage(Re::Warning,
                                tr("保存CSV曲线数据失败"));
                }
            } else {
                emitMessage(Re::Fatal,
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
    File file(this);
    QFile f(name);
    bool flag = file.loadCurveConfig(f, _curve);
    emitMessage(Re::Debug, tr("导入曲线配置 %1").arg(name));
    return flag;

}

bool Revolve::importCsvCurveConfig(const QString &name)
{
    bool flag = _curve.loadFromCsv(name);
    emitMessage(Re::Debug, tr("导入CSV曲线配置 %1").arg(name));
    return flag;
}

bool Revolve::importSoftcanCurveConfig(const QString &name)
{
    bool flag = _softcan.load(name);
    _softcan.toCurve(_curve);
    emitMessage(Re::Debug, tr("导入SoftCAN曲线配置 %1").arg(name));
    return flag;
}

bool Revolve::outputCurveConfig(const QString &name)
{
    File file(this);
    QFile f(name);
    bool flag = file.dumpCurveConfig(f, _curve);
    emitMessage(Re::Debug, tr("导出曲线配置 %1").arg(name));
    return flag;
}

bool Revolve::exportCsvCurveConfig(const QString &name)
{
    bool flag = _curve.dumpToCsv(name);
    emitMessage(Re::Debug, tr("导出CSV曲线配置 %1").arg(name));
    return flag;
}

bool Revolve::outputFrameData(const QString &name)
{
    Q_UNUSED(name);
    return false;
}

bool Revolve::exportCsvFrameData(const QString &name)
{
    Q_UNUSED(name);
    return false;
}

bool Revolve::inputFrameData(const QString &name)
{
    Q_UNUSED(name);
    return false;
}

bool Revolve::importCsvFrameData(const QString &name)
{
    Q_UNUSED(name);
    return false;
}

bool Revolve::inputCurveData(const QString &name)
{
    File file(this);
    QFile f(name);
    if (!_file.loadCurveRecord(f, _tribe)) {
        return false;
    }
    _viewer->regen();
    _tribe_model->genData(&_tribe);
    emitMessage(Re::Debug, tr("导入曲线数据 %1").arg(name));
    return true;
}

bool Revolve::importSoftcanCurveData(const QString &name)
{
    if (!_softcan.load(name)) {
        return false;
    }
    _softcan.toCurve(_curve);
    _softcan.toTribe(_tribe);
    _softcan.clearDataSpace();
    _viewer->regen();
    _tribe_model->genData(&_tribe);
    emitMessage(Re::Debug, tr("导入SoftCAN曲线数据 %1").arg(name));
    return true;
}

bool Revolve::outputCurveData(const QString &name)
{
    File file(this);
    QFile f(name);
    bool flag = file.dumpCurveRecord(f, _tribe);
    emitMessage(Re::Debug, tr("导出曲线数据 %1").arg(name));
    return flag;
}

bool Revolve::exportCsvCurveData(const QString &name)
{
    bool flag = _tribe.dumpToCsv(name);
    emitMessage(Re::Debug, tr("导入CSV曲线数据 %1").arg(name));
    return flag;
}

void Revolve::collectError(int code)
{
    if (code == Collect::ErrorConnection) {
        emitMessage(Re::Critical, tr("检测到连接已经断开，停止采集"));
//        canLostConnection();
        if (stopCollect(true)) {
            collectMenuEnable(false);
        }
    } else if (code == Collect::WarnNoFrame) {
        emitMessage(Re::Warning, tr("采集不到报文"));
    }
}

void Revolve::transformError(int code)
{
    Q_UNUSED(code);
}

void Revolve::recordError(int code)
{
    Q_UNUSED(code);
}

bool Revolve::sendCommand(QByteArray &&bytes)
{
    return false;
}

bool Revolve::burnProgram(QByteArray &&bytes)
{
    return false;
}

void Revolve::getTransformedTcuMessage(const QString &message)
{
    _tcu_message.append(message);
}

QStringList Revolve::readTcuMessage()
{
    QStringList list;
    qSwap(list, _tcu_message);
    return list;
}




