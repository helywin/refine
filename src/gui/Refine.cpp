//
// Created by jiang.wenqiang on 2018/10/22.
//

#include <QtWidgets/QApplication>
#include "Refine.hpp"
#include "Output.hpp"
#include "ChangeLog.hpp"
#include "Sketch.hpp"

Refine::Refine() :
        _init(), _revolve(&_init), _translator()
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
    initMenu(_menu_file_save, tr("保存(&S)..."), _menu_file,
             tr("保存数据包文件"), QKeySequence("Ctrl+S"));
    _menu_file->addSeparator();
    initMenu(_menu_file_settings, tr("设置(&S)..."), _menu_file,
             tr("配置软件设置"), QKeySequence("Ctrl+`"));
    _menu_file->addSeparator();
    initMenu(_menu_file_exit, tr("退出(&E)"), _menu_file,
             tr("退出该软件"), QKeySequence("Alt+F4"));
    initMenu(_menu_view, tr("视图(&I)"), _menubar);
    initMenu(_menu_view_display, tr("显示(&D)"), _menu_view);
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

    initMenu(_menu_init, tr("初始化(&I)"), _menubar);
    initMenu(_menu_init_option, tr("采集选项(&S)..."), _menu_init,
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
             tr("暂停采集曲线(曲线段结束)"));
    initMenu(_menu_control_resume, tr("继续(&R)"), _menu_control,
             tr("继续采集曲线(曲线段开始)"));
    initMenu(_menu_control_finish, tr("结束(&F)"), _menu_control,
             tr("结束采集曲线"));
    initMenu(_menu_tools, tr("工具(&T)"), _menubar);
    initMenu(_menu_tools_timer, tr("计时器(&T)"), _menu_tools);
    initMenu(_menu_tools_timers[0], tr("计时器1"), _menu_tools_timer,
             tr("计时器1开始计时"), QKeySequence("Ctrl+1"), true);
    initMenu(_menu_tools_timers[1], tr("计时器2"), _menu_tools_timer,
             tr("计时器2开始计时"), QKeySequence("Ctrl+2"), true);
    initMenu(_menu_tools_timers[2], tr("计时器3"), _menu_tools_timer,
             tr("计时器2开始计时"), QKeySequence("Ctrl+3"), true);
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

    _toolbox = new Toolbox(this);
    _toolbox->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _toolbox->setAction(_menu_view_display_tools);
    addDockWidget(Qt::LeftDockWidgetArea, _toolbox);

    _output = new Output(this);
    _output->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, _output);
    _output->connectToMessager(this);
    _output->connectToMessager(&_revolve);
    _output->setAction(_menu_view_display_output);
//    tabifyDockWidget(_toolbox, _output);
//    _toolbox->raise();

    _curvebox = new CurveBox(this);
    _curvebox->setAllowedAreas(
            Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, _curvebox);
    _curvebox->curvePanel()->setCurve(&_revolve.curve());
    connect(&_revolve, &Revolve::curveLoaded,
            _curvebox->curvePanel(), &CurvePanel::updateCurve,
            Qt::DirectConnection);
    _curvebox->curvePanel()->updateCurve();
    _curvebox->setAction(_menu_view_display_curve);

    _markbox = new MarkBox(this);
    _markbox->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, _markbox);
    _markbox->setAction(_menu_view_display_mark);

    tabifyDockWidget(_curvebox, _markbox);
    _curvebox->raise();

    _display = new Display(this, &_revolve);
    _revolve.setSketch(&_display->sketch());
    setCentralWidget(_display);

    _statusbar = new StatusBar(this);
    this->setStatusBar(_statusbar);
    _file_picker = new FilePicker(this);
    _output->connectToMessager(_file_picker);

    _changelog = new ChangeLog(this);

    _settings = new Settings(this);

    _timer_start[0] = false;
    _timer_start[1] = false;
    _timer_start[2] = false;

    connect(_menu_file_open, &QAction::triggered,
            _file_picker, &FilePicker::loadArchive, Qt::DirectConnection);
    connect(_menu_file_save, &QAction::triggered,
            _file_picker, &FilePicker::saveArchive, Qt::DirectConnection);
    connect(_menu_init_curve, &QAction::triggered,
            _file_picker, &FilePicker::loadCurveConfig, Qt::DirectConnection);
    connect(_menu_view_full, &QAction::triggered,
            this, &Refine::fullScreen, Qt::DirectConnection);
    connect(_menu_file_exit, &QAction::triggered,
            this, &Refine::close, Qt::DirectConnection);
    connect(_menu_init_can, &QAction::triggered,
            this, &Refine::connectCan, Qt::DirectConnection);
    connect(_menu_control_start, &QAction::triggered,
            this, &Refine::startRevolve, Qt::DirectConnection);
    connect(_menu_control_pause, &QAction::triggered,
            &_revolve, &Revolve::pause, Qt::DirectConnection);
    connect(_menu_control_resume, &QAction::triggered,
            &_revolve, &Revolve::resume, Qt::DirectConnection);
    connect(_menu_control_finish, &QAction::triggered,
            &_revolve, &Revolve::stop, Qt::DirectConnection);
    connect(_file_picker, &FilePicker::pickFile,
            this, &Refine::getFile, Qt::DirectConnection);
    connect(_menu_tools_timers[0], &QAction::triggered,
            this, &Refine::startTimers, Qt::DirectConnection);
    connect(_menu_tools_timers[1], &QAction::triggered,
            this, &Refine::startTimers, Qt::DirectConnection);
    connect(_menu_tools_timers[2], &QAction::triggered,
            this, &Refine::startTimers, Qt::DirectConnection);
    connect(_menu_help_changelog, &QAction::triggered,
            _changelog, &ChangeLog::show, Qt::DirectConnection);
    connect(_menu_view_sketchmsec_group, &QActionGroup::triggered,
            this, &Refine::changeUpdateMsec, Qt::DirectConnection);
    connect(_menu_view_display_group, &QActionGroup::triggered,
            this, &Refine::displayAndHide, Qt::DirectConnection);
    connect(_menu_file_settings, &QAction::triggered,
            _settings, &Settings::show, Qt::DirectConnection);
}

void Refine::setLanguage()
{
    _translator.load("./lang/zh.qm");
    QApplication::installTranslator(&_translator);
}

void Refine::startRevolve()
{
    _revolve.begin(10, 3, 0);
}

void Refine::connectCan()
{
    if (_menu_init_can->isChecked()) {
        if (_revolve.can().connect()) {
            emit message(Messager::MessageType::Info, tr("连接成功"));
        } else {
            emit message(Messager::MessageType::Warning,
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
            emit message(Messager::MessageType::Info, tr("关闭成功"));
        } else if (flag) {
            emit message(Messager::MessageType::Warning, tr("关闭失败"));
        } else {
            emit message(Messager::MessageType::Info, tr("关闭取消"));
        }
    }
    _menu_init_can->setChecked(_revolve.can().isConnected());
}

void Refine::fullScreen()
{
    if (_menu_view_full->isChecked()) {
        emit message(Messager::MessageType::Debug, tr("进入全屏模式"));
        this->setWindowState(Qt::WindowFullScreen);
    } else {
        emit message(Messager::MessageType::Debug, tr("退出全屏模式"));
        this->setWindowState(Qt::WindowMaximized);
    }
}

void Refine::getFile(int type, const QString &file)
{
    qDebug() << file;
    QString ext = FilePicker::extName(file);
    qDebug() << ext;
    if (ext.isEmpty()) {
        emit message(Messager::Fatal, tr("读取的文件不带扩展名"));
    }
    switch (type) {
        case FilePicker::ArchiveInFile:
            break;
        case FilePicker::ArchiveOutFile:
            break;
        case FilePicker::CurveConfigInFile:
            if (ext == FilePicker::extendName(FilePicker::CurveConfigCsv)) {
                if (_revolve.importCsvCurveConfig(file)) {
                    emit message(Messager::Info, tr("读取csv曲线配置成功"));
                } else {
                    emit message(Messager::Warning,
                                 tr("读取csv曲线配置失败，检查配置格式"));
                }
            } else if (ext == FilePicker::extendName(FilePicker::CurveConfig)) {
                if (_revolve.inputCurveConfig(file)) {
                    emit message(Messager::Info, tr("读取曲线配置成功"));
                } else {
                    emit message(Messager::Warning,
                                 tr("读取曲线配置失败，检查配置格式"));
                }
            } else if (ext == FilePicker::extendName(
                    FilePicker::CurveConfigSoftcan)) {
                if (_revolve.importSoftcanCurveConfig(file)) {
                    emit message(Messager::Info, tr("读取SoftCAN曲线配置成功"));
                } else {
                    emit message(Messager::Warning,
                                 tr("读取SoftCAN曲线配置失败，检查配置格式"));
                }
            } else {
                emit message(Messager::Fatal, tr("读取的曲线配置扩展名超出预料"));
            }
            break;
        case FilePicker::CurveConfigOutFile:
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
            break;
        case FilePicker::CurveDataOutFile:
            break;
        case FilePicker::ResultDataInFile:
            break;
        case FilePicker::ResultDataOutFile:
            break;
        default:
            break;
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

void Refine::startTimers()
{
    for (int i = 0; i < 3; ++i) {
        if (_menu_tools_timers[i]->isChecked() && !_timer_start[i]) {
            message(Messager::Info,
                    tr("计时器") + QString::number(i + 1) + tr("开始"));
            _timer[i] = QTime::currentTime();
            _timer_start[i] = true;
        }
        if (!_menu_tools_timers[i]->isChecked() && _timer_start[i]) {
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
            message(Messager::Info,
                    tr("计时器") + QString::number(i + 1) + tr("结束 ") + str);
            _timer_start[i] = false;
        }
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

