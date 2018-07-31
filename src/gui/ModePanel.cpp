//
// Created by jiang.wenqiang on 2018/7/31.
//

#include <QtCore/QDebug>
#include "ModePanel.h"

ModePanel::ModePanel(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void ModePanel::setupUi() {
    layout = new QVBoxLayout(this);
    button_widget = new QWidget(this);
    button_layout = new QHBoxLayout(button_widget);
    button_widget->setLayout(button_layout);
    this->setLayout(layout);
    mode_list = new QTableWidget(this);
    mode_list->setColumnCount(2);
    mode_list->setRowCount(8);
    mode_list->setSelectionMode(QAbstractItemView::SingleSelection);
    mode_list->setSelectionBehavior(QAbstractItemView::SelectRows);
//    mode_list->resize(mode_list->frameRect().width(), mode_list->frameRect().height());
    mode_list->setColumnWidth(0, 230);
    mode_list->setColumnWidth(1, 30);
    mode_list->setHorizontalHeaderLabels(QStringList({"工况", "状态"}));
    start_mode = new QPushButton(this);
    start_mode->setText(QString("开始"));
    start_mode->setFixedHeight(30);
    stop_mode = new QPushButton(this);
    stop_mode->setText(QString("中止"));
    stop_mode->setFixedHeight(30);
    layout->addWidget(mode_list);
    layout->addWidget(button_widget);
    button_layout->addWidget(start_mode);
    button_layout->addWidget(stop_mode);


}

