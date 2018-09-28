//
// Created by jiang.wenqiang on 2018/9/28.
//

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
    _menu_init = new QMenu(QString("初始化(&I)"), _menubar);
    _menu_init_can = new QAction(QString("CAN(&C)"), _menu_init);
    _menu_init_curve = new QAction(QString("曲线配置(&V)"), _menu_init);
    _menu_init->addAction(_menu_init_can);
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
    _menu_help = new QMenu(QString("帮助(&H)"), _menubar);
    _menu_help_about = new QAction(QString("关于(&A)"), _menu_help);
    _menu_help->addAction(_menu_help_about);

    _menubar->addMenu(_menu_init);
    _menubar->addMenu(_menu_collect);
    _menubar->addMenu(_menu_help);
    _menubar->addAction(_menu_init->menuAction());
    _menubar->addAction(_menu_collect->menuAction());
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
    _transform->disableFramesStored();
    _revolve = new Revolve(_collect, _transform, nullptr);
    _core_initialized = true;
    if (!_can->connect()) {
        qDebug("can没连接！");
        return false;
    }
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
