//
// Created by jiang.wenqiang on 2018/9/28.
//

#include <QtCore/QProcess>
#include <QtWidgets/QMessageBox>
#include <QtCore/QStandardPaths>
#include "Scope.hpp"

Scope::Scope(QWidget *parent) : QMainWindow(parent)
{
    setupUi();
    setupCore();
    qInfo() << Version::str();
    qInfo() << "初始化完毕";
}

void Scope::setupUi()
{
    this->setWindowTitle(QString("示波器"));
    this->resize(500, 500);
    this->move(400, 100);
    _menubar = new QMenuBar(this);
    _menu_init = new QMenu(QString("初始(&I)"), _menubar);
    _menu_init_channel = new QMenu(QString("通道选择(&C)"), _menu_init);
    _menu_init_channel_ch0 = new QAction(QString("通道 0"), _menu_init_channel);
    _menu_init_channel_ch1 = new QAction(QString("通道 1"), _menu_init_channel);
    _menu_init_channel_ch0->setCheckable(true);
    _menu_init_channel_ch0->setChecked(true);
    _menu_init_channel_ch1->setCheckable(true);
    _menu_init_channel_ch1->setChecked(false);
    _menu_init_channel->addAction(_menu_init_channel_ch0);
    _menu_init_channel->addAction(_menu_init_channel_ch1);
    _menu_init_connection = new QAction(QString("连接(&C)"), _menu_init);
    _menu_init_connection->setCheckable(true);
    _menu_init_curve = new QAction(QString("加载曲线配置(&V)"), _menu_init);
    _menu_init_softcan = new QAction(QString("加载SoftCAN配置(&S)"), _menu_init);
    _menu_init->addMenu(_menu_init_channel);
    _menu_init->addAction(_menu_init_channel->menuAction());
    _menu_init->addAction(_menu_init_connection);
    _menu_init->addAction(_menu_init_curve);
    _menu_init->addAction(_menu_init_softcan);
    _menu_collect = new QMenu(QString("采集(&C)"), _menubar);
    _menu_collect_fromframe = new QAction(QString("从帧文件采集(&F)"), _menu_collect);
    _menu_collect_fromframe->setCheckable(true);
    _menu_collect_saveframe = new QAction(QString("保存帧文件(&S)"), _menu_collect);
    _menu_collect_saveframe->setCheckable(true);
    _menu_collect_start = new QAction(QString("开始(&B)"), _menu_collect);
    _menu_collect_pause = new QAction(QString("暂停(&P)"), _menu_collect);
    _menu_collect_resume = new QAction(QString("继续(&R)"), _menu_collect);
    _menu_collect_stop = new QAction(QString("结束(&E)"), _menu_collect);
    _menu_collect->addAction(_menu_collect_fromframe);
    _menu_collect->addAction(_menu_collect_saveframe);
    _menu_collect->addAction(_menu_collect_start);
    _menu_collect->addAction(_menu_collect_pause);
    _menu_collect->addAction(_menu_collect_resume);
    _menu_collect->addAction(_menu_collect_stop);
    _menu_view = new QMenu(QString("视图(&V)"), _menubar);
    _menu_view_fixed = new QAction(QString("固定(&F)"), _menu_view);
    _menu_view_fixed->setCheckable(true);
    _menu_view_fixed->setChecked(false);
    _menu_view_msec = new QMenu(QString("刷新时间(&T)"), _menu_view);
    _menu_view_msec_10 = new QAction(QString("10 ms"), _menu_view_msec);
    _menu_view_msec_20 = new QAction(QString("20 ms"), _menu_view_msec);
    _menu_view_msec_30 = new QAction(QString("30 ms"), _menu_view_msec);
    _menu_view_msec_50 = new QAction(QString("50 ms"), _menu_view_msec);
    _menu_view_msec_100 = new QAction(QString("100 ms"), _menu_view_msec);
    _menu_view_msec_10->setCheckable(true);
    _menu_view_msec_10->setChecked(true);
    _menu_view_msec_20->setCheckable(true);
    _menu_view_msec_30->setCheckable(true);
    _menu_view_msec_50->setCheckable(true);
    _menu_view_msec_100->setCheckable(true);
    _menu_view_msec->addAction(_menu_view_msec_10);
    _menu_view_msec->addAction(_menu_view_msec_20);
    _menu_view_msec->addAction(_menu_view_msec_30);
    _menu_view_msec->addAction(_menu_view_msec_50);
    _menu_view_msec->addAction(_menu_view_msec_100);
    _menu_view_smooth = new QAction(QString("抗锯齿(&S)"), _menu_view);
    _menu_view_smooth->setCheckable(true);
    _menu_view_smooth->setChecked(true);
    _menu_view->addAction(_menu_view_fixed);
    _menu_view->addMenu(_menu_view_msec);
    _menu_view->addAction(_menu_view_msec->menuAction());
    _menu_view->addAction(_menu_view_smooth);
    _menu_data = new QMenu(QString("数据(&D)"), _menubar);
    _menu_data_frame = new QAction(QString("报文(&F)"), _menu_data);
    _menu_data_curve = new QAction(QString("曲线(&C)"), _menu_data);
    _menu_data->addAction(_menu_data_frame);
    _menu_data->addAction(_menu_data_curve);
    _menu_help = new QMenu(QString("帮助(&H)"), _menubar);
    _menu_help_about = new QAction(QString("关于(&A)"), _menu_help);
    _menu_help->addAction(_menu_help_about);

    _menubar->addMenu(_menu_init);
    _menubar->addMenu(_menu_collect);
    _menubar->addMenu(_menu_view);
    _menubar->addMenu(_menu_data);
    _menubar->addMenu(_menu_help);
    _menubar->addAction(_menu_init->menuAction());
    _menubar->addAction(_menu_collect->menuAction());
    _menubar->addAction(_menu_view->menuAction());
    _menubar->addAction(_menu_data->menuAction());
    _menubar->addAction(_menu_help->menuAction());
    this->setMenuBar(_menubar);

    _central_splitter = new QSplitter(this);
    this->setCentralWidget(_central_splitter);

    _display = new Display(_central_splitter);
    _central_splitter->addWidget(_display);

    _statusbar = new StatusBar(this);
    this->setStatusBar(_statusbar);


    _timer = new QTimer(this);
    _file_curve = new QFileDialog(this, "打开配置文件", ".", "csv配置文件(*.csv)");
    static QList<QUrl> urls;
    urls.append(QUrl::fromLocalFile(QStandardPaths::standardLocations(
            QStandardPaths::DesktopLocation).first()));
    _file_curve->setSidebarUrls(urls);

    _file_softcan = new QFileDialog(this, "导入SoftCAN配置文件", ".",
                                    "SoftCAN配置文件(*.cfg)");
//    _file_softcan->setSidebarUrls(urls);
    _core_initialized = false;
    _curve_initialized = false;
    _refresh_msec = 10;
    previous_connection_status = false;
    _process_open_curve = new QProcess;
    connect(_menu_init_connection, &QAction::triggered, this,
            &Scope::connectCan, Qt::DirectConnection);
    connect(_menu_init_channel_ch0, &QAction::triggered, this,
            &Scope::changeToChannel0, Qt::DirectConnection);
    connect(_menu_init_channel_ch1, &QAction::triggered, this,
            &Scope::changeToChannel1, Qt::DirectConnection);
    connect(_menu_collect_fromframe, &QAction::triggered, this,
            &Scope::setCollectFrame, Qt::DirectConnection);
    connect(_menu_collect_saveframe, &QAction::triggered, this,
            &Scope::setSaveFrame, Qt::DirectConnection);
    connect(_menu_view_fixed, &QAction::triggered, this,
            &Scope::setViewFixed, Qt::DirectConnection);
    connect(_menu_init_curve, &QAction::triggered, this,
            &Scope::openCurveConfig, Qt::DirectConnection);
    connect(_menu_init_softcan, &QAction::triggered, _file_softcan,
            &QFileDialog::show, Qt::DirectConnection);
    connect(_file_curve, &QFileDialog::fileSelected, this,
            &Scope::loadCurveConfig, Qt::DirectConnection);
    connect(_file_softcan, &QFileDialog::fileSelected, this,
            &Scope::importSoftcan, Qt::DirectConnection);
    connect(_menu_view_smooth, &QAction::triggered, this,
            &Scope::setupSmooth, Qt::DirectConnection);
}

bool Scope::setupCore()
{
    _config = new Can::Config(0);
    _can = new Can(_config);
    _menu_init_connection->setChecked(_can->isConnected());
    _curve = new Curve;
    _file_frames = new QFile("data.fmd");
    _collect_frames = new QFile("frame.fmd");
    _file_log = new QFile("log.txt");
    _buffer = new Buffer(100, 100);
    _collect = new Collect(_can, _buffer);
    _tribe = new Tribe;
    _transform = new Transform(_buffer, _curve, _tribe, _file_frames, true);
    _revolve = new Revolve(_collect, _transform, nullptr, _tribe);
    _softcan = new Softcan;
    _core_initialized = true;
    _display->setTribe(_tribe);
    _display->setCurve(_curve);
    _frame_viewer = new FrameViewer(this);
    Log::enableOutput(_statusbar);
    Log::enableSave(_file_log);
    Log::setupHandler();

    connect(_timer, &QTimer::timeout, _display, &Display::updateGL);
    connect(_menu_collect_start, &QAction::triggered, this,
            &Scope::start);
    connect(_menu_collect_pause, &QAction::triggered, this,
            &Scope::pause, Qt::DirectConnection);
    connect(_menu_collect_resume, &QAction::triggered, this,
            &Scope::resume, Qt::DirectConnection);
    connect(_menu_collect_stop, &QAction::triggered, this,
            &Scope::stop, Qt::DirectConnection);
    connect(_menu_data_frame, &QAction::triggered, this,
            &Scope::openFrameData, Qt::DirectConnection);
    connect(_menu_data_curve, &QAction::triggered, this,
            &Scope::openCurveData, Qt::DirectConnection);
    connect(_menu_view_msec_10, &QAction::triggered, this,
            &Scope::setViewRefreshMsec10, Qt::DirectConnection);
    connect(_menu_view_msec_20, &QAction::triggered, this,
            &Scope::setViewRefreshMsec20, Qt::DirectConnection);
    connect(_menu_view_msec_30, &QAction::triggered, this,
            &Scope::setViewRefreshMsec30, Qt::DirectConnection);
    connect(_menu_view_msec_50, &QAction::triggered, this,
            &Scope::setViewRefreshMsec50, Qt::DirectConnection);
    connect(_menu_view_msec_100, &QAction::triggered, this,
            &Scope::setViewRefreshMsec100, Qt::DirectConnection);
    connect(_menu_help_about, &QAction::triggered, this,
            &Scope::showAbout, Qt::DirectConnection);
    return true;
}

void Scope::releseCore()
{
    _can->close();
    delete _softcan;
    delete _revolve;
    delete _transform;
    delete _tribe;
    delete _collect;
    delete _buffer;
    delete _file_frames;
    delete _collect_frames;
    delete _file_log;
    delete _curve;
    delete _can;
    delete _config;
}

Scope::~Scope()
{
    if (_core_initialized) {
        releseCore();
    }
}

void Scope::connectCan()
{
    previous_connection_status = !_menu_init_connection->isChecked();
    if (_can->isConnected()) {
        if (_can->close()) {
            _menu_init_connection->setChecked(false);
            _menu_init_channel->setDisabled(false);
            qInfo() << "CAN断开成功";
        } else {
//            QMessageBox::warning(this, QString("警告"), QString("CAN断开失败"));
            _menu_init_connection->setChecked(previous_connection_status);
            qWarning() << "CAN断开失败";
        }
    } else {
        if (_can->connect()) {
            _menu_init_connection->setChecked(true);
            _menu_init_channel->setDisabled(true);
            qInfo() << "CAN连接成功";
        } else {
            if (previous_connection_status) {
//                QMessageBox::warning(this, QString("警告"),
//                                     QString("CAN断开失败"));
                qWarning() << "CAN断开失败";
            } else {
//                QMessageBox::warning(this, QString("警告"),
//                                     QString("CAN没连接或被占用"));
                qWarning() << "CAN没连接或被占用";
            }
            _menu_init_connection->setChecked(previous_connection_status);
        }
    }
//    qDebug() << "连接状态:" << _can->isConnected();
}

void Scope::setViewFixed()
{
    if (_menu_view_fixed->isChecked()) {
        _timer->stop();
        qInfo() << "固定视图";
    } else {
        _timer->setInterval(_refresh_msec);
        _timer->start();
        qInfo() << "取消固定视图";
    }
}

void Scope::openFrameData()
{
    if (_revolve->isRunning()) {
        return;
    }
    _frame_viewer->readFrameData("data.fmd");
    _frame_viewer->move(200, 50);
    _frame_viewer->show();
}

void Scope::openCurveData()
{
    if (_revolve->isRunning()) {
        return;
    }
    _tribe->dumpToCsv("data.csv");
    _process_open_curve->start(
            "notepad data.csv");
//    QProcess::execute("notepad D:/jiang.wenqiang/code/refine/data/data.csv");
}

void Scope::setViewRefreshMsec10()
{
    _refresh_msec = 10;
    _menu_view_msec_20->setChecked(false);
    _menu_view_msec_30->setChecked(false);
    _menu_view_msec_50->setChecked(false);
    _menu_view_msec_100->setChecked(false);
    if (_revolve->isRunning()) {
        _timer->stop();
        _timer->setInterval(_refresh_msec);
        _timer->start();
    }
}

void Scope::setViewRefreshMsec20()
{
    _refresh_msec = 20;
    _menu_view_msec_10->setChecked(false);
    _menu_view_msec_30->setChecked(false);
    _menu_view_msec_50->setChecked(false);
    _menu_view_msec_100->setChecked(false);
    if (_revolve->isRunning()) {
        _timer->stop();
        _timer->setInterval(_refresh_msec);
        _timer->start();
    }
}

void Scope::setViewRefreshMsec30()
{
    _refresh_msec = 30;
    _menu_view_msec_10->setChecked(false);
    _menu_view_msec_20->setChecked(false);
    _menu_view_msec_50->setChecked(false);
    _menu_view_msec_100->setChecked(false);
    if (_revolve->isRunning()) {
        _timer->stop();
        _timer->setInterval(_refresh_msec);
        _timer->start();
    }
}

void Scope::setViewRefreshMsec50()
{
    _refresh_msec = 50;
    _menu_view_msec_10->setChecked(false);
    _menu_view_msec_20->setChecked(false);
    _menu_view_msec_30->setChecked(false);
    _menu_view_msec_100->setChecked(false);
    if (_revolve->isRunning()) {
        _timer->stop();
        _timer->setInterval(_refresh_msec);
        _timer->start();
    }
}

void Scope::setViewRefreshMsec100()
{
    _refresh_msec = 100;
    _menu_view_msec_10->setChecked(false);
    _menu_view_msec_20->setChecked(false);
    _menu_view_msec_30->setChecked(false);
    _menu_view_msec_50->setChecked(false);
    if (_revolve->isRunning()) {
        _timer->stop();
        _timer->setInterval(_refresh_msec);
        _timer->start();
    }
}

void Scope::closeEvent(QCloseEvent *event)
{
    if (_revolve->isRunning()) {
        _revolve->stopRevolve();
    }
    while (_revolve->isRunning()) {}
    _can->close();
}

void Scope::openCurveConfig()
{
    if (_curve_initialized) {
        return;
    }
    QString file_name("config.csv");
    QFile f(file_name);
    if (f.exists()) {
        loadCurveConfig(file_name);
    } else {
        _file_curve->show();
    }
}

void Scope::loadCurveConfig(const QString &file_name)
{
    if (!_curve->loadFromCsv(file_name)) {
//        QMessageBox::warning(this, "警告", "加载曲线配置失败");
        qWarning("加载曲线配置失败");
        return;
    }
    for (const auto &iter : *_curve) {
        _tribe->append(iter.name());
    }
    qInfo() << "曲线配置加载成功";
//    qDebug() << _curve->subIdMap777Str();
    _curve_initialized = true;
}

void Scope::showAbout()
{
    QMessageBox::information(this, "关于",
                             "Refine软件底层采集转换功能测试\n" + Version::str());
}

void Scope::setupSmooth()
{
    if (_menu_view_smooth->isChecked()) {
        _display->enableSmooth();
        qInfo() << "开启抗锯齿";
    } else {
        _display->disableSmooth();
        qInfo() << "关闭抗锯齿";
    }
}

void Scope::importSoftcan(const QString &file_name)
{
    if (_softcan->load(file_name)) {
        _curve->clear();
        _softcan->toCurve(*_curve);
        for (const auto &iter : *_curve) {
            _tribe->append(iter.name());
        }
//        qDebug() << _curve->subIdMap777Str();
        _curve_initialized = true;
    }
}

void Scope::setCollectFrame()
{
    if (_menu_collect_fromframe->isChecked()) {
        if (_collect_frames->exists()) {
            _collect->setMode(Collect::CollectManner::FromFile, 10,
                              _collect_frames);
            qInfo() << "从帧文件采集";
        } else {
            _menu_collect_fromframe->setChecked(false);
        }
    } else {
        _collect->setMode(Collect::CollectManner::FromCan, 10);
        qInfo() << "从CAN采集";
    }
}

void Scope::setSaveFrame()
{
    if (_menu_collect_saveframe->isChecked()) {
        _transform->enableFramesStored(_file_frames);
        qInfo() << "保存采集的帧";
    } else {
        _transform->disableFramesStored();
        qInfo() << "不保存采集的帧";
    }
}

void Scope::start()
{
    if (!_menu_collect_fromframe->isChecked() && !_can->isConnected()) {
        qWarning() << "CAN没连接不能采数据";
        return;
    }
    if (!_curve_initialized) {
        qWarning() << "没有曲线配置不能采数据";
        return;
    }
    _revolve->startRevolve();
    _timer->setInterval(_refresh_msec);
    _timer->start();
    if (_transform->isFramesStored()) {
        _file_frames->setFileName(
                QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss") +
                QString(".fmd"));
    }
    qInfo() << "开始数据采集";
}

