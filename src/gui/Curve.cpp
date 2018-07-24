//
// Created by jiang.wenqiang on 2018/7/23.
//

#include "Curve.h"

Curve::Curve(QWidget *parent) : QDialog(parent) {
    setup_ui();
    move(0, 0);
    setWindowFlag(Qt::ToolTip);
    setFixedSize(1366, 706);
}

void Curve::setup_ui() {
    setWindowTitle(QString("曲线配置"));
    table = new CurveTable(this);
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 30, 10, 10);

    menu_bar = new QMenuBar(this);
    menu_file = new QMenu(QString("文件(&F)"), this);
    menu_edit = new QMenu(QString("编辑(&E)"), this);
    menu_action = new QMenu(QString("动作(&A)"), this);
    menu_file_open = new QAction(this);
    menu_file_open->setText(QString("打开(&O)..."));
    menu_file_save = new QAction(this);
    menu_file_save->setText(QString("保存(&S)"));
    menu_file_saveas = new QAction(this);
    menu_file_saveas->setText(QString("另存为(&A)..."));

    menu_file->addAction(menu_file_open);
    menu_file->addAction(menu_file_save);
    menu_file->addAction(menu_file_saveas);

    menu_edit_reset = new QAction(this);
    menu_edit_reset->setText(QString("重置(&R)"));

    menu_edit->addAction(menu_edit_reset);

    menu_action_finish = new QAction(this);
    menu_action_finish->setText(QString("完成(&F)"));
    menu_action_abort = new QAction(this);
    menu_action_abort->setText(QString("放弃(&A)"));

    menu_action->addAction(menu_action_finish);
    menu_action->addAction(menu_action_abort);

    menu_bar->addAction(menu_file->menuAction());
    menu_bar->addAction(menu_edit->menuAction());
    menu_bar->addAction(menu_action->menuAction());

    layout->addWidget(table);

}


