//
// Created by jiang.wenqiang on 2018/7/23.
//

#include "Select.h"
#include <QtGui/QtEvents>

Select::Select(QWidget *parent) : QDialog(parent) {
    setupUi();
    move(0, 0);
    setWindowFlag(Qt::ToolTip);
//    this->setWindowState(Qt::WindowMaximized);
//    setWindowFlag(Qt::WindowMaximizeButtonHint, false);
//    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowState(Qt::WindowMaximized);
    setWindowTitle(QString("设置工况配置"));
//    this->setFixedSize(size());
}

void Select::setupUi() {
    layout = new QHBoxLayout(this);
    this->setLayout(layout);
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

    whole_modes = new QTreeWidget(this);
    modes = new QListWidget(this);
    criteria = new QListWidget(this);
    params = new QTableWidget(this);

    layout->addWidget(whole_modes, 1);
    layout->addWidget(modes, 1);
    layout->addWidget(criteria, 1);
    layout->addWidget(params, 1);

    //! code for demo
    whole_modes->setColumnCount(1);
    whole_modes->setHeaderLabel("工况选择");
    QTreeWidgetItem *mode_start = new QTreeWidgetItem(
            whole_modes, QStringList(QString("Acceleration")));
    QTreeWidgetItem *mode_Acceleration = new QTreeWidgetItem(
            mode_start, QStringList(QString("CylinderDeactivation")));
    QTreeWidgetItem *mode_CylinderReactivation = new QTreeWidgetItem(
            mode_start, QStringList(QString("CylinderReactivation")));
    QTreeWidgetItem *mode_Elasticity = new QTreeWidgetItem(
            mode_start, QStringList(QString("Elasticity")));
    QTreeWidgetItem *mode_FullLoad = new QTreeWidgetItem(
            mode_start, QStringList(QString("FullLoad")));
    QTreeWidgetItem *mode_FullLoadHundredKph = new QTreeWidgetItem(
            mode_start, QStringList(QString("FullLoadHundredKph")));
    QTreeWidgetItem *mode_FullLoadThroughGears = new QTreeWidgetItem(
            mode_start, QStringList(QString("FullLoadThroughGears")));
    QTreeWidgetItem *mode_LowEndTorque = new QTreeWidgetItem(
            mode_start, QStringList(QString("LowEndTorque")));
    QTreeWidgetItem *mode_PartLoadConstantPedal = new QTreeWidgetItem(
            mode_start, QStringList(QString("PartLoadConstantPedal")));
    QTreeWidgetItem *mode_PartLoadFallingPedal = new QTreeWidgetItem(
            mode_start, QStringList(QString("PartLoadFallingPedal")));
    QTreeWidgetItem *mode_PartLoadRisingPedal = new QTreeWidgetItem(
            mode_start, QStringList(QString("PartLoadRisingPedal")));

    modes->addItem(new QListWidgetItem(QString("Elasticity")));
    modes->addItem(new QListWidgetItem(QString("FullLoad")));
    modes->addItem(new QListWidgetItem(QString("LowEndTorque")));
    modes->addItem(new QListWidgetItem(QString("PartLoadRisingPedal")));

    criteria->addItem(new QListWidgetItem(QString("NinetyPercentTorqueRange")));
    criteria->addItem(
            new QListWidgetItem(QString("NinetyPercentTorqueThreshold")));
    criteria->addItem(new QListWidgetItem(QString("AccelerationGradient")));
    criteria->addItem(new QListWidgetItem(QString("AccelerationSteps")));
    criteria->addItem(new QListWidgetItem(QString("Correlation")));
    criteria->addItem(new QListWidgetItem(QString("Elasticity")));
    criteria->addItem(new QListWidgetItem(QString("EngineFreeAcceleration")));
    criteria->addItem(new QListWidgetItem(QString("EngineSpeedChange")));
    criteria->addItem(new QListWidgetItem(QString("ExpectedAcceleration")));
    criteria->addItem(new QListWidgetItem(QString("FullLoadHundredKph")));
    criteria->addItem(new QListWidgetItem(QString("FullLoadOverHundredKph")));
    criteria->addItem(new QListWidgetItem(QString("FullLoadThroughGears")));
    criteria->addItem(new QListWidgetItem(QString("LowEndTorque")));
    criteria->addItem(new QListWidgetItem(QString("MaximumExpectedTorque")));
    criteria->addItem(new QListWidgetItem(QString("Noise")));
    criteria->addItem(new QListWidgetItem(QString("PedalMap")));
    criteria->addItem(new QListWidgetItem(QString("ReferenceAcceleration")));
    criteria->addItem(new QListWidgetItem(QString("RpmLimiter")));
    criteria->addItem(new QListWidgetItem(QString("Steadiness")));
    criteria->addItem(new QListWidgetItem(QString("Surge")));
    criteria->addItem(new QListWidgetItem(QString("TorqueBuildUp")));
    criteria->addItem(new QListWidgetItem(QString("TorqueResponse")));
    criteria->addItem(new QListWidgetItem(QString("TorqueSmoothness")));
    criteria->addItem(new QListWidgetItem(QString("Vibrations")));

}

void Select::showEvent(QShowEvent *event) {
    move(0, 0);
}

void Select::closeEvent(QCloseEvent *event) {
    event->ignore();
    this->hide();
}

void Select::resizeEvent(QResizeEvent *event) {
    setWindowState(Qt::WindowMaximized);
}

