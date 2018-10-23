//
// Created by jiang.wenqiang on 2018/10/22.
//

#include <QtWidgets/QApplication>
#include "Refine.hpp"

Refine::Refine() :
        _init(), _revolve(&_init)
{
    setup();
}

void Refine::setup()
{
    setWindowTitle(tr("Refine-数据采集工况匹配软件"));
    resize(800, 600);
    _win_state = Qt::WindowMaximized;
    _presentation = false;
    setWindowState(_win_state);
    setWindowIcon(QIcon(":res/ui/logo.ico"));
    QApplication::setStyle("fusion");
    _menubar = new QMenuBar(this);
    this->setMenuBar(_menubar);
    initMenu(_menu_file, tr("文件(&F)"), _menubar);
    initMenu(_menu_file_open, tr("打开(&O)..."), _menu_file,
             tr("打开数据包文件"), QKeySequence("Ctrl+O"));
    initMenu(_menu_file_save, tr("保存(&S)..."), _menu_file,
             tr("保存数据包文件"), QKeySequence("Ctrl+S"));
    _menu_file->addSeparator();
    initMenu(_menu_file_config, tr("选项(&C)..."), _menu_file,
             tr("配置软件设置"), QKeySequence("Ctrl+`"));
    _menu_file->addSeparator();
    initMenu(_menu_file_exit, tr("退出(&E)"), _menu_file,
             tr("退出该软件"), QKeySequence("Alt+F4"));
    initMenu(_menu_view, tr("视图(&I)"), _menubar);
    initMenu(_menu_view_display, tr("显示(&D)"), _menu_view);
    initMenu(_menu_view_display_tools, tr("文件工具栏(&F)"), _menu_view_display,
             tr("工具栏显示/隐藏"), true, true);
    _menu_view_display->addSeparator();
    initMenu(_menu_view_display_docker, tr("工具箱窗口(&T)"), _menu_view_display,
             tr("工具箱窗口显示/隐藏"), true, true);
    initMenu(_menu_view_display_output, tr("输出窗口(&O)"), _menu_view_display,
             tr("输出窗口显示/隐藏"), true, true);
    initMenu(_menu_view_display_curve, tr("曲线窗口(&C)"), _menu_view_display,
             tr("曲线窗口显示/隐藏"), true, true);
    initMenu(_menu_view_full, tr("全屏(&F)"), _menu_view,
             tr("全屏/取消全屏"), QKeySequence("F11"), true);
    initMenu(_menu_view_presentation, tr("演示(&P)"), _menu_view,
             tr("演示/取消演示"), QKeySequence("F12"), true);
    initMenu(_menu_init, tr("初始化(&I)"), _menubar);
    initMenu(_menu_init_setting, tr("设置(&S)..."), _menu_init,
             tr("设置CAN、曲线配置和工况"));
    initMenu(_menu_init_can, tr("连接CAN(&C)"), _menu_init,
            tr("连接/断开CAN"), true);
    initMenu(_menu_init_curve, tr("曲线配置(&C)..."), _menu_init,
            tr("读取曲线配置"));
    initMenu(_menu_init_mode, tr("工况配置(&M)..."), _menu_init,
            tr("读取工况配置"));
    initMenu(_menu_control, tr("控制(&C)"), _menubar);
    initMenu(_menu_control_start, tr("开始(&S)"), _menu_control,
            tr("开始采集曲线"));
    initMenu(_menu_control_pause, tr("暂停(&P)"), _menu_control,
            tr("暂停采集曲线(生成曲线段)"));
    initMenu(_menu_control_resume, tr("继续(&R)"), _menu_control,
            tr("继续采集曲线(开始曲线段)"));
    initMenu(_menu_control_finish, tr("结束(&F)"), _menu_control,
            tr("结束采集曲线"));
    initMenu(_menu_help, tr("帮助(&H)"), _menubar);
    initMenu(_menu_help_tutorial, tr("手册(&T)..."), _menu_help,
            tr("软件手册"));
    initMenu(_menu_help_version, tr("版本(&V)..."), _menu_help,
            tr("软件版本信息"));
    initMenu(_menu_help_license, tr("声明(&D)..."), _menu_help,
            tr("开源声明"));
    initMenu(_menu_help_feedback, tr("反馈(&F)..."), _menu_help,
            tr("反馈bug或意见"));
    initMenu(_menu_help_about, tr("关于(&A)..."), _menu_help,
            tr("关于本软件"));

    _toolbar_menu = new QToolBar(tr("文件(&F)"), this);
    this->addToolBar(_toolbar_menu);
    _toolbar_menu->addAction(_menu_file_open);

    _docker = new Toolbox(this);
    _docker->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, _docker);

    _output = new Output(this);
    _output->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, _output);

    _curvebox = new CurveBox(this);
    _curvebox->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, _curvebox);

    _splitter = new Splitter(this);
    setCentralWidget(_splitter);

    _statusbar = new StatusBar(this);
    this->setStatusBar(_statusbar);

    connect(_menu_file_open, &QAction::triggered,
            &_file_dialog, &FilePicker::show, Qt::DirectConnection);
    connect(_menu_view_full, &QAction::triggered,
            this, &Refine::fullScreen, Qt::DirectConnection);
    connect(_menu_file_exit, &QAction::triggered,
            this, &Refine::close, Qt::DirectConnection);

}

