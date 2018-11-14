//
// Created by jiang.wenqiang on 2018/10/22.
//

#include <QtWidgets/QApplication>
#include "Refine.hpp"
#include "Output.hpp"
#include "ChangeLog.hpp"
#include "Sketch.hpp"

Refine::Refine() :
        Message(nullptr), _init(), _revolve(&_init), _translator()
{
    setup();
}

void Refine::setup()
{
    _init.loadSettings();
    setLanguage();
    setWindowTitle(tr("Refine - 数据采集 & 工况匹配"));
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
    _menu_file_open->setIcon(QIcon(":res/icons/open.png"));
    initMenu(_menu_file_save, tr("保存(&S)..."), _menu_file,
             tr("保存数据包文件"), QKeySequence("Ctrl+S"));
    _menu_file_save->setIcon(QIcon(":res/icons/save.png"));
    _menu_file->addSeparator();
    initMenu(_menu_file_import, tr("导入(&I)"), _menu_file);
    _menu_file_import->setIcon(QIcon(":res/icons/import.png"));
    initMenu(_menu_file_import_config, tr("曲线配置(&C)..."),
             _menu_file_import, tr("导入曲线配置"));
    initMenu(_menu_file_import_data, tr("曲线数据(&D)..."),
             _menu_file_import, tr("导入曲线数据"));
    initMenu(_menu_file_import_frame, tr("报文数据(&F)..."),
             _menu_file_import, tr("导入报文数据"));
    initMenu(_menu_file_import_mode, tr("工况配置(&M)..."),
             _menu_file_import, tr("导入工况配置"));
    initMenu(_menu_file_export, tr("导出(&E)"), _menu_file);
    _menu_file_export->setIcon(QIcon(":res/icons/export.png"));
    initMenu(_menu_file_export_config, tr("曲线配置(&C)..."),
             _menu_file_export, tr("导出曲线配置"));
    initMenu(_menu_file_export_data, tr("曲线数据(&D)..."),
             _menu_file_export, tr("导出曲线数据"));
    initMenu(_menu_file_export_frame, tr("报文数据(&F)..."),
             _menu_file_export, tr("导出报文数据"));
    initMenu(_menu_file_export_mode, tr("工况配置(&M)..."),
             _menu_file_export, tr("导出工况配置"));
    _menu_file->addSeparator();
    initMenu(_menu_file_settings, tr("设置(&S)..."), _menu_file,
             tr("配置软件设置"), QKeySequence("Ctrl+`"));
    _menu_file_settings->setIcon(QIcon(":res/icons/settings.png"));
    _menu_file->addSeparator();
    initMenu(_menu_file_exit, tr("退出(&E)"), _menu_file,
             tr("退出该软件"), QKeySequence("Alt+F4"));
    _menu_file_exit->setIcon(QIcon(":res/icons/exit.png"));
    initMenu(_menu_view, tr("视图(&I)"), _menubar);
    initMenu(_menu_view_display, tr("显示(&D)"), _menu_view);
    _menu_view_display->setIcon(QIcon(":res/icons/display.png"));
    _menu_view_display_group = new QActionGroup(_menu_view_display);
    _menu_view_display_group->setExclusive(false);
    initMenu(_menu_view_display_file, tr("文件工具栏(&F)"),
             _menu_view_display_group, _menu_view_display,
             tr("工具栏显示/隐藏"), true, true);
    _menu_view_display->addSeparator();
    initMenu(_menu_view_display_tools, tr("工具箱窗口(&T)"),
             _menu_view_display_group, _menu_view_display,
             tr("工具箱窗口显示/隐藏"), true, true);
    initMenu(_menu_view_display_output, tr("输出窗口(&O)"),
             _menu_view_display_group, _menu_view_display,
             tr("输出窗口显示/隐藏"), true, true);
    initMenu(_menu_view_display_curve, tr("曲线窗口(&C)"),
             _menu_view_display_group, _menu_view_display,
             tr("曲线窗口显示/隐藏"), true, true);
    initMenu(_menu_view_display_mark, tr("标注窗口(&M)"),
             _menu_view_display_group, _menu_view_display,
             tr("标注窗口显示/隐藏"), true, true);
    initMenu(_menu_view_full, tr("全屏(&F)"), _menu_view,
             tr("全屏/取消全屏"), QKeySequence("F11"), true);
    initMenu(_menu_view_presentation, tr("演示(&P)"), _menu_view,
             tr("演示/取消演示"), QKeySequence("F12"), true);
    initMenu(_menu_view_sketchmsec, tr("重绘间隔(&R)"), _menu_view);
    _menu_view_sketchmsec_group = new QActionGroup(_menu_view_sketchmsec);
    initMenu(_menu_view_sketchmsec_10, tr("10 ms"), _menu_view_sketchmsec_group,
             _menu_view_sketchmsec, tr("10ms刷新间隔"), true, true);
    initMenu(_menu_view_sketchmsec_20, tr("20 ms"), _menu_view_sketchmsec_group,
             _menu_view_sketchmsec, tr("20ms刷新间隔"), true);
    initMenu(_menu_view_sketchmsec_30, tr("30 ms"), _menu_view_sketchmsec_group,
             _menu_view_sketchmsec, tr("30ms刷新间隔"), true);
    initMenu(_menu_view_sketchmsec_50, tr("50 ms"), _menu_view_sketchmsec_group,
             _menu_view_sketchmsec, tr("50ms刷新间隔"), true);
    initMenu(_menu_view_sketchmsec_100, tr("100 ms"),
             _menu_view_sketchmsec_group,
             _menu_view_sketchmsec, tr("100ms刷新间隔"), true);
    _menu_view_sketchmsec_group->setExclusive(true);
    initMenu(_menu_view_smooth, tr("反走样(&A)"), _menu_view,
             tr("开启/关闭反走样"), true, true);

    initMenu(_menu_init, tr("初始化(&I)"), _menubar);
    initMenu(_menu_init_option, tr("采集选项(&S)..."), _menu_init,
             tr("设置CAN、曲线配置和工况"));
    _menu_init_option->setIcon(QIcon(":res/icons/config.png"));
    _menu_init->addSeparator();
    initMenu(_menu_init_can, tr("连接CAN(&C)"), _menu_init,
             tr("连接/断开CAN"), true);
    _menu_init->addSeparator();
    initMenu(_menu_init_curve, tr("加载曲线配置(&L)..."), _menu_init,
             tr("加载曲线配置"));
    initMenu(_menu_init_editcurve, tr("编辑曲线配置(&E)..."), _menu_init,
             tr("编辑曲线配置"));
    _menu_init->addSeparator();
    initMenu(_menu_init_mode, tr("加载工况配置(&L)..."), _menu_init,
             tr("加载工况配置"));
    initMenu(_menu_control, tr("控制(&C)"), _menubar);
    initMenu(_menu_control_start, tr("开始(&S)"), _menu_control,
             tr("开始采集曲线"));
    _menu_control_start->setIcon(QIcon(":res/icons/start.png"));
    initMenu(_menu_control_pause, tr("暂停(&P)"), _menu_control,
             tr("暂停采集曲线(曲线段结束)"));
    _menu_control_pause->setIcon(QIcon(":res/icons/pause.png"));
    initMenu(_menu_control_resume, tr("继续(&R)"), _menu_control,
             tr("继续采集曲线(曲线段开始)"));
    _menu_control_resume->setIcon(QIcon(":res/icons/resume.png"));
    initMenu(_menu_control_finish, tr("结束(&F)"), _menu_control,
             tr("结束采集曲线"));
    _menu_control_finish->setIcon(QIcon(":res/icons/stop.png"));
    initMenu(_menu_tools, tr("工具(&T)"), _menubar);
    initMenu(_menu_tools_timer, tr("计时器(&T)"), _menu_tools);
    _menu_tools_timer->setIcon(QIcon(":res/icons/timer.png"));
    _menu_tools_timer_group = new QActionGroup(_menu_tools_timer);
    _menu_tools_timer_group->setExclusive(false);
    initMenu(_menu_tools_timers[0], tr("计时器1"),
             _menu_tools_timer_group, _menu_tools_timer, tr("计时器1开始计时"),
             QKeySequence("Ctrl+1"), true);
    initMenu(_menu_tools_timers[1], tr("计时器2"),
             _menu_tools_timer_group, _menu_tools_timer, tr("计时器2开始计时"),
             QKeySequence("Ctrl+2"), true);
    initMenu(_menu_tools_timers[2], tr("计时器3"),
             _menu_tools_timer_group, _menu_tools_timer, tr("计时器3开始计时"),
             QKeySequence("Ctrl+3"), true);
    initMenu(_menu_tools_wakeup, tr("屏幕常亮(&S)"), _menu_tools,
             tr("保持屏幕常亮不黑屏开/关"), true);
    initMenu(_menu_help, tr("帮助(&H)"), _menubar);
    initMenu(_menu_help_tutorial, tr("手册(&T)..."), _menu_help,
             tr("软件手册和工况"));
    initMenu(_menu_help_changelog, tr("变更(&C)..."), _menu_help,
             tr("软件版本变更信息"));
    initMenu(_menu_help_license, tr("声明(&D)..."), _menu_help,
             tr("开源声明"));
    initMenu(_menu_help_feedback, tr("反馈(&F)..."), _menu_help,
             tr("反馈bug或意见"));
    initMenu(_menu_help_about, tr("关于(&A)..."), _menu_help,
             tr("关于本软件"));

    _toolbar_file = new QToolBar(tr("文件(&F)"), this);
    this->addToolBar(_toolbar_file);
    _toolbar_file->addAction(_menu_file_open);
    _toolbar_file->addAction(_menu_file_save);
//    _toolbar_file->addAction(_menu_file_import_config);
//    _toolbar_file->addAction(_menu_file_import_data);
//    _toolbar_file->addAction(_menu_file_import_frame);
//    _toolbar_file->addAction(_menu_file_import_mode);
//    _toolbar_file->addAction(_menu_file_export_config);
//    _toolbar_file->addAction(_menu_file_export_data);
//    _toolbar_file->addAction(_menu_file_export_frame);
//    _toolbar_file->addAction(_menu_file_export_mode);
    _toolbar_file->addAction(_menu_file_settings);
    _toolbar_file->addAction(_menu_file_exit);

    _toolbox = new Toolbox(this);
    _toolbox->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, _toolbox);

    _output = new Output(this);
    _output->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, _output);
    _output->connectToMessager(this);
    _output->connectToMessager(&_revolve);
//    tabifyDockWidget(_toolbox, _output);
//    _toolbox->raise();

    _curvebox = new CurveBox(&_revolve.tribe(), this, this);
    _curvebox->setAllowedAreas(
            Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, _curvebox);
    _revolve.setTribeModel(&_curvebox->tribeModel());

    _markbox = new MarkBox(this);
    _markbox->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, _markbox);

    tabifyDockWidget(_curvebox, _markbox);
    _curvebox->raise();

    _display = new Display(this, &_revolve, this);
    _revolve.setSketch(&_display->sketch());
    _curvebox->connectModelToSketch(&_display->sketch());
    setCentralWidget(_display);

    _statusbar = new StatusBar(this);
    this->setStatusBar(_statusbar);
    _file_picker = new FilePicker(this);
    _output->connectToMessager(_file_picker);

    _changelog = new ChangeLog(this);
    _about = new About(this);

    _settings = new Settings(this);

    _editor = new CurveEditor(&_revolve.curve(), this);
    _revolve.setCurveEditor(_editor);
    _revolve.setActionCan(_menu_init_can);

    _timer_start[0] = false;
    _timer_start[1] = false;
    _timer_start[2] = false;

    connect(_menu_file_open, &QAction::triggered,
            _file_picker, &FilePicker::loadArchive, Qt::DirectConnection);
    connect(_menu_file_save, &QAction::triggered,
            _file_picker, &FilePicker::saveArchive, Qt::DirectConnection);
    connect(_menu_file_import_config, &QAction::triggered,
            _file_picker, &FilePicker::loadCurveConfig, Qt::DirectConnection);
    connect(_menu_file_import_data, &QAction::triggered,
            _file_picker, &FilePicker::loadCurveData, Qt::DirectConnection);
    connect(_menu_file_import_frame, &QAction::triggered,
            _file_picker, &FilePicker::loadFrameData, Qt::DirectConnection);
    connect(_menu_file_import_mode, &QAction::triggered,
            _file_picker, &FilePicker::loadModeConfig, Qt::DirectConnection);
    connect(_menu_file_export_config, &QAction::triggered,
            _file_picker, &FilePicker::saveCurveConfig, Qt::DirectConnection);
    connect(_menu_file_export_data, &QAction::triggered,
            _file_picker, &FilePicker::saveCurveData, Qt::DirectConnection);
    connect(_menu_file_export_frame, &QAction::triggered,
            _file_picker, &FilePicker::saveFrameData, Qt::DirectConnection);
    connect(_menu_file_export_mode, &QAction::triggered,
            _file_picker, &FilePicker::saveModeConfig, Qt::DirectConnection);
    connect(_menu_init_curve, &QAction::triggered,
            _file_picker, &FilePicker::loadCurveConfig, Qt::DirectConnection);
    connect(_menu_view_full, &QAction::triggered,
            this, &Refine::fullScreen, Qt::DirectConnection);
    connect(_menu_file_exit, &QAction::triggered,
            this, &Refine::close, Qt::DirectConnection);
    connect(_menu_init_can, &QAction::triggered,
            this, &Refine::connectCan, Qt::DirectConnection);
    connect(_menu_init_editcurve, &QAction::triggered,
            _editor, &CurveEditor::show, Qt::DirectConnection);
    connect(_menu_control_start, &QAction::triggered,
            this, &Refine::startRevolve, Qt::DirectConnection);
    connect(_menu_control_pause, &QAction::triggered,
            &_revolve, &Revolve::pause, Qt::DirectConnection);
    connect(_menu_control_resume, &QAction::triggered,
            &_revolve, &Revolve::resume, Qt::DirectConnection);
    connect(_menu_control_finish, &QAction::triggered,
            this, &Refine::stopRevolve, Qt::DirectConnection);
    connect(_file_picker, &FilePicker::pickFile,
            &_revolve, &Revolve::getFile, Qt::DirectConnection);
    connect(_menu_tools_timer_group, &QActionGroup::triggered,
            this, &Refine::startTimers, Qt::DirectConnection);
    connect(_menu_help_changelog, &QAction::triggered,
            _changelog, &ChangeLog::show, Qt::DirectConnection);
    connect(_menu_help_about, &QAction::triggered,
            _about, &About::show, Qt::DirectConnection);
    connect(_menu_view_sketchmsec_group, &QActionGroup::triggered,
            this, &Refine::changeUpdateMsec, Qt::DirectConnection);
    connect(_menu_view_display_group, &QActionGroup::triggered,
            this, &Refine::displayAndHide, Qt::DirectConnection);
    connect(_menu_file_settings, &QAction::triggered,
            _settings, &Settings::show, Qt::DirectConnection);
    connect(_menu_view_smooth, &QAction::triggered,
            this, &Refine::setSmooth, Qt::DirectConnection);
    connect(_toolbar_file, &QToolBar::visibilityChanged,
            this, &Refine::widgetsVisibilityChanged);
    connect(_toolbox, &QDockWidget::visibilityChanged,
            this, &Refine::widgetsVisibilityChanged);
    connect(_output, &QDockWidget::visibilityChanged,
            this, &Refine::widgetsVisibilityChanged);
    connect(_curvebox, &QDockWidget::visibilityChanged,
            this, &Refine::widgetsVisibilityChanged);
    connect(_markbox, &QDockWidget::visibilityChanged,
            this, &Refine::widgetsVisibilityChanged);
    connect(&_revolve, &Revolve::collectMenuEnable,
            this, &Refine::setCollectMenuEnable);
}

void Refine::setLanguage()
{
    _translator.load("./lang/zh.qm");
    QApplication::installTranslator(&_translator);
}

void Refine::startRevolve()
{
    if (_revolve.begin(10, 3, 0)) {
        setCollectMenuEnable(true);
    }
}

void Refine::stopRevolve()
{
    if (_revolve.stop()) {
        setCollectMenuEnable(false);
    }
}


void Refine::connectCan()
{
    if (_menu_init_can->isChecked()) {
        if (_revolve.can().connect()) {
            emitMessage(Info, tr("连接成功"));
        } else {
            emitMessage(Warning,
                         tr("连接失败，检查CAN占用或连接情况"));
        }
    } else {
        int flag = 0;
        bool unfinished = !_revolve.finished();
        if (unfinished) {
            flag = QMessageBox::warning(this, tr("警告"),
                                        tr("采集尚未结束，是否关闭CAN"),
                                        tr("取消"),
                                        tr("确认"));
            qDebug() << "flag: " << flag;
            if (flag) {
                _revolve.stop();
            }
        }
        bool closed = false;
        if (unfinished && flag || !unfinished) {
            closed = _revolve.can().close();
        }
        if (closed) {      //与运算后面的语句可能不执行，看条件而定
            emitMessage(Info, tr("关闭成功"));
        } else if (flag) {
            emitMessage(Warning, tr("关闭失败"));
        } else {
            emitMessage(Info, tr("关闭取消"));
        }
    }
    _menu_init_can->setChecked(_revolve.can().isConnected());
}

void Refine::fullScreen()
{
    if (_menu_view_full->isChecked()) {
        emitMessage(Debug, tr("进入全屏模式"));
        this->setWindowState(Qt::WindowFullScreen);
    } else {
        emitMessage(Debug, tr("退出全屏模式"));
        this->setWindowState(Qt::WindowMaximized);
    }
}

void Refine::closeEvent(QCloseEvent *event)
{
    if (!_revolve.finished()) {
        int flag = QMessageBox::warning(this, tr("警告"),
                                        tr("采集尚未结束，是否退出"),
                                        tr("取消"),
                                        tr("确认"));
        if (flag) {
            _revolve.stop();
            _revolve.can().close();
        } else {
            event->setAccepted(false);
            return;
        }
    }
    if (_revolve.can().isConnected()) {
        _revolve.can().close();
    }
}

void Refine::startTimers(QAction *action)
{
    int i = 0;
    for (; i < 3; ++i) {
        if (action == _menu_tools_timers[i]) {
            break;
        }
    }
    if (action->isChecked()) {
        message(MessagerPanel::Info, action->text() + tr("开始"));
        _timer[i] = QTime::currentTime();
        action->setStatusTip(action->text() + tr("停止计时"));
    }
    if (!action->isChecked()) {
        int ms = _timer[i].msecsTo(QTime::currentTime());
        int h = ms / (60 * 60 * 1000);
        ms %= (60 * 60 * 1000);
        int m = ms / (60 * 1000);
        ms %= (60 * 1000);
        int s = ms / (1000);
        ms %= (1000);
        QTime time(h, m, s, ms);
        QString str;
        if (h) {
            str += QString("%1h").arg(h);
        }
        if (m) {
            str += QString("%1m").arg(m, (h != 0) * 2, 10, QChar('0'));
        }
        if (s) {
            str += QString("%1s").arg(s, (h || m) * 3, 10, QChar('0'));
        }
        if (ms) {
            str += QString("%1ms").arg(ms, (h || m || s) * 2,
                                       10, QChar('0'));
        }
        message(MessagerPanel::Info, action->text() + tr("结束 ") + str);
        action->setStatusTip(action->text() + tr("开始计时"));
    }
}

void Refine::changeUpdateMsec(QAction *action)
{
    if (action == _menu_view_sketchmsec_10) {
        _display->sketch().setMsec(10);
    } else if (action == _menu_view_sketchmsec_20) {
        _display->sketch().setMsec(20);
    } else if (action == _menu_view_sketchmsec_30) {
        _display->sketch().setMsec(30);
    } else if (action == _menu_view_sketchmsec_50) {
        _display->sketch().setMsec(50);
    } else if (action == _menu_view_sketchmsec_100) {
        _display->sketch().setMsec(100);
    }
}

void Refine::displayAndHide(QAction *action)
{
    if (action == _menu_view_display_file) {
        _toolbar_file->setVisible(_menu_view_display_file->isChecked());
    } else if (action == _menu_view_display_tools) {
        _toolbox->setVisible(_menu_view_display_tools->isChecked());
    } else if (action == _menu_view_display_output) {
        _output->setVisible(_menu_view_display_output->isChecked());
    } else if (action == _menu_view_display_curve) {
        _curvebox->setVisible(_menu_view_display_curve->isChecked());
    } else if (action == _menu_view_display_mark) {
        _markbox->setVisible(_menu_view_display_mark->isChecked());
    }
}

void Refine::setSmooth()
{
    _display->sketch().setSmooth(_menu_view_smooth->isChecked());
}

void Refine::widgetsVisibilityChanged(bool visible)
{
    _menu_view_display_file->setChecked(_toolbar_file->isVisible());
    _menu_view_display_tools->setChecked(_toolbox->isVisible());
    _menu_view_display_output->setChecked(_output->isVisible());
    _menu_view_display_curve->setChecked(_curvebox->isVisible());
    _menu_view_display_mark->setChecked(_markbox->isVisible());
}

void Refine::setCollectMenuEnable(bool isCollecting)
{
    _menu_init_curve->setDisabled(isCollecting);
    _menu_init_editcurve->setDisabled(isCollecting);
    _menu_file_import_config->setDisabled(isCollecting);
    _menu_file_export_data->setDisabled(isCollecting);
    _menu_file_export_frame->setDisabled(isCollecting);
}

