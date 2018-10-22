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
    setWindowTitle("Refine");
    resize(800, 600);
    setWindowState(Qt::WindowMaximized);
    setWindowIcon(QIcon(":res/ui/logo.ico"));
    QApplication::setStyle("fusion");
    _menubar = new QMenuBar(this);
    this->setMenuBar(_menubar);
    initMenu(_menu_file, tr("文件(&F)"), _menubar);
    initMenu(_menu_file_open, tr("打开(&O)\tCtrl+O"), _menu_file);
    initMenu(_menu_file_save, tr("保存(&S)\tCtrl+S"), _menu_file);
    initMenu(_menu_file_exit, tr("退出(&E)\tAlt+F4"), _menu_file);
    initMenu(_menu_view, tr("视图(&I)"), _menubar);
    initMenu(_menu_view_display, tr("显示(&D)"), _menu_view);
    initMenu(_menu_view_display_west, tr("左侧(&W)"), _menu_view_display);
    initMenu(_menu_view_display_east, tr("右侧(&E)"), _menu_view_display);
    initMenu(_menu_view_display_south, tr("下侧(&S)"), _menu_view_display);
    initMenu(_menu_view_display_tools, tr("工具栏(&T)"), _menu_view_display);
    _menu_view_display_west->setCheckable(true);
    _menu_view_display_east->setCheckable(true);
    _menu_view_display_south->setCheckable(true);
    _menu_view_display_tools->setCheckable(true);
    _menu_view_display_west->setChecked(true);
    _menu_view_display_east->setChecked(true);
    _menu_view_display_south->setChecked(true);
    _menu_view_display_tools->setChecked(true);
    initMenu(_menu_view_full, tr("全屏(&F)\tF11"), _menu_view);
    initMenu(_menu_init, tr("初始化(&I)"), _menubar);
    initMenu(_menu_init_setting, tr("设置(&S)"), _menu_init);
    initMenu(_menu_init_can, tr("CAN(&C)"), _menu_init);
    initMenu(_menu_init_curve, tr("曲线配置(&C)"), _menu_init);
    initMenu(_menu_init_mode, tr("工况配置(&M)"), _menu_init);
    initMenu(_menu_control, tr("控制(&C)"), _menubar);
    initMenu(_menu_control_start, tr("开始(&S)"), _menu_control);
    initMenu(_menu_control_pause, tr("暂停(&P)"), _menu_control);
    initMenu(_menu_control_resume, tr("继续(&R)"), _menu_control);
    initMenu(_menu_control_finish, tr("结束(&F)"), _menu_control);
    initMenu(_menu_help, tr("帮助(&H)"), _menubar);
    initMenu(_menu_help_tutorial, tr("手册(&T)"), _menu_help);
    initMenu(_menu_help_version, tr("版本(&V)"), _menu_help);
    initMenu(_menu_help_license, tr("版权(&L)"), _menu_help);
    initMenu(_menu_help_feedback, tr("反馈(&F)"), _menu_help);
    initMenu(_menu_help_about, tr("关于(&A)"), _menu_help);

    connect(_menu_file_open, &QAction::triggered,
            &_file_dialog, &FileDialog::show, Qt::DirectConnection);
}
