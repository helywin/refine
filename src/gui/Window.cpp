//
// Created by jiang.wenqiang on 2018/7/19.
//

#include "Window.h"
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTabWidget>

#include <QtCore/QDebug>
#include <QtWidgets/QApplication>

Window::Window(QWidget *parent) : QMainWindow(parent) {
    setup_ui();
    move(100,100);
    this->setWindowState(Qt::WindowMaximized);
}

void Window::setup_ui() {
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("MainWindow"));
    
    this->setDocumentMode(true);
    this->setWindowTitle(QString("Refine匹配软件"));
    central_widget = new QWidget(this);
    this->setCentralWidget(central_widget);
    whole_hbox_layout = new QHBoxLayout(central_widget);
    left_widget = new QWidget(this);
    mid_widget = new QWidget(this);
    right_widget = new QWidget(this);
    left_vbox_layout = new QVBoxLayout(left_widget);
    mid_vbox_layout = new QVBoxLayout(mid_widget);
    right_vbox_layout = new QVBoxLayout(right_widget);
    
    opengl = new QOpenGLWidget(mid_widget);
    
    central_widget->setLayout(whole_hbox_layout);
    left_tab_widget = new QTabWidget(left_widget);
    left_tab_widget->addTab(new QWidget(left_tab_widget), QString("工况"));
    left_tab_widget->addTab(new QWidget(left_tab_widget), QString("通信"));
    left_tab_widget->addTab(new QWidget(left_tab_widget), QString("文件"));
    left_tab_widget->addTab(new QWidget(left_tab_widget), QString("报告"));
    left_tab_widget->setTabEnabled(0, false);
    left_tab_widget->setTabBarAutoHide(true);
    left_tab_widget->setTabPosition(QTabWidget::West);
    left_tab_widget->setTabBarAutoHide(true);
    //    whole_hbox_layout->setMargin(0);
    whole_hbox_layout->addWidget(left_widget, 1);
    whole_hbox_layout->addWidget(mid_widget, 7);
    whole_hbox_layout->addWidget(right_widget, 1);
    left_widget->setLayout(left_vbox_layout);
    mid_widget->setLayout(mid_vbox_layout);
    right_widget->setLayout(right_vbox_layout);
    left_vbox_layout->addWidget(new QPushButton());
    left_vbox_layout->addWidget(left_tab_widget);
    mid_vbox_layout->addWidget(opengl,10);
    mid_vbox_layout->setMargin(0);
    opengl_scroller = new QScrollBar(mid_widget);
    opengl_scroller->setOrientation(Qt::Horizontal);
    mid_vbox_layout->addWidget(opengl_scroller, 1);
    right_vbox_layout->addWidget(new QLineEdit(right_widget));
    right_vbox_layout->addWidget(new QTabWidget(right_widget));

    menu_bar = new QMenuBar(this);
    menu_bar->setObjectName(QStringLiteral("menu_bar"));
    menu_bar->setGeometry(QRect(0, 0, 300, 23));

    menu_file = new QMenu(QString("文件(&F)"), menu_bar);
    menu_collect = new QMenu(QString("采集(&C)"), menu_bar);
    menu_match = new QMenu(QString("匹配(&M)"), menu_bar);
    menu_setting = new QMenu(QString("设置(&S)"));
    menu_help = new QMenu(QString("帮助(&H)"));
    menu_file_new = new QAction(this);
    menu_file_new->setText("新建(&N)");
    menu_collect_load = new QAction(this);
    menu_collect_load->setText("加载配置(&L)...");
    menu_collect_config = new QAction(this);
    menu_collect_config->setText("手动配置(&M)...");
    menu_match_load = new QAction(this);
    menu_match_load->setText("加载配置(&L)...");
    menu_match_config = new QAction(this);
    menu_match_config->setText("手动配置(&M)...");
    menu_setting_skin = new QAction(this);
    menu_setting_skin->setText("皮肤(&S)");
    menu_help_about = new QAction(this);
    menu_help_about->setText("关于(&A)...");
    menu_help_feedback->setText("反馈(&A)...");
    menu_file->addAction(menu_file_new);
    menu_collect->addAction(menu_collect_load);
    menu_collect->addAction(menu_collect_config);
    menu_match->addAction(menu_match_load);
    menu_match->addAction(menu_match_config);
    menu_setting->addAction(menu_setting_skin);
    menu_help->addAction(menu_help_about);

    menu_bar->addAction(menu_file->menuAction());
    menu_bar->addAction(menu_collect->menuAction());
    menu_bar->addAction(menu_match->menuAction());
    menu_bar->addAction(menu_setting->menuAction());
    menu_bar->addAction(menu_help->menuAction());

    this->setMenuBar(menu_bar);
    status_bar = new QStatusBar(this);
    status_bar->setObjectName(QStringLiteral("status_bar"));

    this->setStatusBar(status_bar);

    connect(menu_setting_skin, &QAction::triggered, this, &Window::change_skin);
    connect(menu_help_about, &QAction::triggered, this, &Window::about);
    about_dialog = new About(this);
}

Window::~Window() {
    delete about_dialog;
}

void Window::resizeEvent(QResizeEvent *event) {
    qDebug() << this->size();
}

void Window::change_skin() {
    static int flag = 0;
    flag += 1;
    flag %= 3;
    switch (flag) {
        case 0:
            QApplication::setStyle("fusion");
            break;
        case 1:
            QApplication::setStyle("windows");
            break;
        case 2:
            QApplication::setStyle("windowsvista");
            break;
        default:
            break;
    }
}

void Window::about() {
    about_dialog->show();
}

void Window::hide_about() {
    about_dialog->hide();
}