//
// Created by jiang.wenqiang on 2018/9/28.
//

#include <QtWidgets/QMessageBox>
#include "Scope.hpp"

Scope::Scope(QWidget *parent) : QMainWindow(parent)
{
    setupUi();
    setupCore();
}

void Scope::setupUi()
{
    this->setWindowTitle(QString("示波器"));
    this->resize(500, 500);
    this->move(400, 100);
    _menubar = new QMenuBar(this);
    _menu_init = new QMenu(QString("初始(&I)"), _menubar);
    _menu_init_channel = new QMenu(QString("通道选择(&C)"), _menu_init);
    _menu_init_can_ch0 = new QAction(QString("通道 0"), _menu_init_channel);
    _menu_init_can_ch1 = new QAction(QString("通道 1"), _menu_init_channel);
    _menu_init_channel->addAction(_menu_init_can_ch0);
    _menu_init_channel->addAction(_menu_init_can_ch1);
    _menu_init_connection = new QAction(QString("连接(&C)"), _menu_init);
    _menu_init_connection->setCheckable(true);
    _menu_init_curve = new QAction(QString("曲线配置(&V)"), _menu_init);
    _menu_init->addMenu(_menu_init_channel);
    _menu_init->addAction(_menu_init_channel->menuAction());
    _menu_init->addAction(_menu_init_connection);
    _menu_init->addAction(_menu_init_curve);
    _menu_collect = new QMenu(QString("采集(&C)"), _menubar);
    _menu_collect_start = new QAction(QString("开始(&B)"), _menu_collect);
    _menu_collect_pause = new QAction(QString("暂停(&P)"), _menu_collect);
    _menu_collect_resume = new QAction(QString("继续(&R)"), _menu_collect);
    _menu_collect_stop = new QAction(QString("结束(&E)"), _menu_collect);
    _menu_collect->addAction(_menu_collect_start);
    _menu_collect->addAction(_menu_collect_pause);
    _menu_collect->addAction(_menu_collect_resume);
    _menu_collect->addAction(_menu_collect_stop);
    _menu_view = new QMenu(QString("视图(&V)"), _menubar);
    _menu_view_fixed = new QAction(QString("固定(&F)"), _menu_view);
    _menu_view_fps = new QMenu(QString("刷新时间(&T)"), _menu_view);
    _menu_view_fps_10 = new QAction(QString("10 ms"), _menu_view_fps);
    _menu_view_fps_20 = new QAction(QString("20 ms"), _menu_view_fps);
    _menu_view_fps_30 = new QAction(QString("30 ms"), _menu_view_fps);
    _menu_view_fps_50 = new QAction(QString("50 ms"), _menu_view_fps);
    _menu_view_fps_100 = new QAction(QString("100 ms"), _menu_view_fps);
    _menu_view_fps_10->setChecked(true);
    _menu_view_fps_20->setChecked(true);
    _menu_view_fps_30->setChecked(true);
    _menu_view_fps_50->setChecked(true);
    _menu_view_fps_100->setChecked(true);
    _menu_view_fps->addAction(_menu_view_fps_10);
    _menu_view_fps->addAction(_menu_view_fps_20);
    _menu_view_fps->addAction(_menu_view_fps_30);
    _menu_view_fps->addAction(_menu_view_fps_50);
    _menu_view_fps->addAction(_menu_view_fps_100);
    _menu_view->addAction(_menu_view_fixed);
    _menu_view->addMenu(_menu_view_fps);
    _menu_view->addAction(_menu_view_fps->menuAction());
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

    _statusbar = new QStatusBar(this);
    this->setStatusBar(_statusbar);
    _timer = new QTimer(this);
    _core_initialized = false;
    _curve_initialized = false;
    _refresh_msec = 10;
    connect(_menu_init_connection, &QAction::triggered, this,
            &Scope::connectCan, Qt::DirectConnection);
}

bool Scope::setupCore()
{
    _config = new Can::Config(0);
    _can = new Can(_config);
    _curve = new Curve;
    _curve->loadFromCsv("D:/jiang.wenqiang/code/refine/config/配置.csv");
    _file_frames = new QFile("D:/jiang.wenqiang/code/refine/data/data.fmd");
    _buffer = new Buffer(100, 100);
    _collect = new Collect(_can, _buffer);
    _tribe = new Tribe;
    for (const auto &iter : *_curve) {
        _tribe->append(iter.name());
    }
    _transform = new Transform(_buffer, _curve, _tribe, _file_frames, true);
    _revolve = new Revolve(_collect, _transform, nullptr, _tribe);
    _core_initialized = true;
    _display->setTribe(_tribe);
    connect(_timer, &QTimer::timeout, _display, &Display::updateGL);
    connect(_menu_collect_start, &QAction::triggered, this,
            &Scope::start);
    connect(_menu_collect_pause, &QAction::triggered, this,
            &Scope::pause, Qt::DirectConnection);
    connect(_menu_collect_resume, &QAction::triggered, this,
            &Scope::resume, Qt::DirectConnection);
    connect(_menu_collect_stop, &QAction::triggered, this,
            &Scope::stop, Qt::DirectConnection);
    return true;
}

void Scope::releseCore()
{
    _can->close();
    delete _revolve;
    delete _transform;
    delete _tribe;
    delete _collect;
    delete _buffer;
    delete _file_frames;
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
    bool previous_check_status = !_menu_init_connection->isChecked();
    if (_can->isConnected()) {
        if (_can->close()) {
            _menu_init_connection->setChecked(false);
        } else {
            QMessageBox::warning(this, QString("警告"), QString("CAN断开失败"));
            _menu_init_connection->setChecked(previous_check_status);

        }
    } else {
        if (_can->connect()) {
            _menu_init_connection->setChecked(true);
        } else {
            QMessageBox::warning(this, QString("警告"), QString("CAN断开失败"));
            _menu_init_connection->setChecked(previous_check_status);
        }
    }
    qDebug() << "连接状态:" << _can->isConnected();
}
