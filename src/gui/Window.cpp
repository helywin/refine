//
// Created by jiang.wenqiang on 2018/7/19.
//

#include "Window.h"
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QStatusBar>

Window::Window(QWidget *parent) : QMainWindow(parent) {
    setup_ui();
}

void Window::setup_ui() {
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("MainWindow"));
    this->setWindowTitle(QString("Refine匹配软件"));
    menu_bar = new QMenuBar(this);
    menu_bar->setObjectName(QStringLiteral("menu_bar"));
    menu = new QMenu(QString("文件"), menu_bar);
    menu->setObjectName(QStringLiteral("menu"));
    menu_bar->setGeometry(QRect(0, 0, 300, 23));
    this->setMenuBar(menu_bar);
    status_bar = new QStatusBar(this);
    status_bar->setObjectName(QStringLiteral("status_bar"));
    this->setStatusBar(status_bar);
}

void Window::break_ui() {
    delete menu;
    delete menu_bar;
    delete status_bar;
}

Window::~Window() {
    break_ui();
}