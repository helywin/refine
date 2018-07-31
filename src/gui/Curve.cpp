//
// Created by jiang.wenqiang on 2018/7/23.
//

#include <QtCore/QDebug>
#include "Curve.h"

Curve::Curve(QWidget *parent) : QDialog(parent) {
    setupUi();
    move(0, 0);
    setWindowFlag(Qt::ToolTip);
    setWindowState(Qt::WindowMaximized);
}

void Curve::setupUi() {
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
    menu_edit_reset->setText(QString("重置(&R)\tCTRL+R"));
    menu_edit_reset->setShortcut(QKeySequence(tr("Ctrl+R")));

    menu_edit_add = new QAction(this);
    menu_edit_add->setText(QString("增加(&A)\tCTRL+A"));
    menu_edit_add->setShortcut(QKeySequence(tr("Ctrl+A")));

    menu_edit_insert = new QAction(this);
    menu_edit_insert->setText(QString("插入(&I)\tCTRL+I"));
    menu_edit_insert->setShortcut(QKeySequence(tr("Ctrl+I")));

    menu_edit_delete = new QAction(this);
    menu_edit_delete->setText(QString("删除(&D)\tCTRL+D"));
    menu_edit_delete->setShortcut(QKeySequence(tr("Ctrl+D")));

    menu_edit->addAction(menu_edit_reset);
    menu_edit->addSeparator();
    menu_edit->addAction(menu_edit_add);
    menu_edit->addAction(menu_edit_insert);
    menu_edit->addAction(menu_edit_delete);

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

    connect(menu_edit_reset, &QAction::triggered, table, &CurveTable::clearAll);
    connect(menu_edit_add, &QAction::triggered, table, &CurveTable::add);
    connect(menu_edit_insert, &QAction::triggered, table, &CurveTable::insert);
    connect(menu_edit_delete, &QAction::triggered, table, &CurveTable::remove);
    connect(menu_action_abort, &QAction::triggered, this, &Curve::hide);

}

void Curve::resizeEvent(QResizeEvent *event) {
    setWindowState(Qt::WindowMaximized);
}


