//
// Created by jiang.wenqiang on 2018/7/19.
//

#include "Window.h"
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtCore/QDebug>
#include <QtWidgets/QApplication>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QFileDialog>
#include <QtCore/QTimer>


Window::Window(QWidget *parent) : QMainWindow(parent) {
    setupUi();
    move(100, 100);
    setWindowState(Qt::WindowMaximized);
}

void Window::setupUi() {
    this->setDocumentMode(true);
    this->setWindowTitle(QString("Refine匹配软件"));
    buttom_widget = new QWidget(this);
    central_vsplitter = new QSplitter(Qt::Vertical, this);
    central_hsplitter = new QSplitter(Qt::Horizontal, central_vsplitter);
    central_vsplitter->addWidget(central_hsplitter);

    bottom_tab_widget = new QTabWidget(central_vsplitter);
    bottom_tab_widget->setContentsMargins(5,5,5,5);
    central_vsplitter->addWidget(bottom_tab_widget);
    bottom_tab_widget->addTab(new QTableWidget(), QString("日志"));
    central_vsplitter->setSizes
            (QList({this->width() / 5 * 4, this->width() / 5}));

    this->setCentralWidget(central_vsplitter);
//    whole_hbox_layout = new QHBoxLayout(buttom_widget);
    left_widget = new QWidget(this);
    left_widget->setMinimumWidth(200);
    mid_content = new QWidget(this);
    mid_content->setMinimumWidth(200);
    mid_widget = new Middle(this);
    right_widget = new QWidget(this);
    right_widget->setMinimumWidth(200);
    left_vbox_layout = new QVBoxLayout(left_widget);
    left_vbox_layout->setContentsMargins(5, 5, 0, 5);
    mid_container = new QVBoxLayout(mid_content);
    mid_container->setContentsMargins(0, 5, 0, 5);
    mid_vbox_layout = new QVBoxLayout(mid_widget);
    mid_vbox_layout->setMargin(0);
    right_vbox_layout = new QVBoxLayout(right_widget);
    right_vbox_layout->setContentsMargins(0, 5, 5, 5);

    opengl = new Sketch(mid_widget);

//    buttom_widget->setLayout(whole_hbox_layout);
    left_tab_widget = new QTabWidget(left_widget);
    mode = new ModePanel(left_tab_widget);
    left_tab_widget->addTab(mode, QString("工况"));
    left_tab_widget->addTab(new QWidget(left_tab_widget), QString("通信"));
    left_tab_widget->addTab(new QWidget(left_tab_widget), QString("数据"));
    left_tab_widget->addTab(new QWidget(left_tab_widget), QString("报告"));
    left_tab_widget->addTab(new QWidget(left_tab_widget), QString("坐标"));
//    left_tab_widget->setTabEnabled(1, false);
    left_tab_widget->setTabBarAutoHide(true);
    left_tab_widget->setTabPosition(QTabWidget::North);
    left_tab_widget->setTabBarAutoHide(true);
    //    whole_hbox_layout->setMargin(0);
    central_hsplitter->addWidget(left_widget);
    central_hsplitter->addWidget(mid_content);
    central_hsplitter->addWidget(right_widget);
    central_hsplitter->setSizes
            (QList({this->width() / 8, this->width() / 8 * 6,
                    this->width() / 8}));
    central_hsplitter->setStretchFactor(1, 20);
    central_hsplitter->setStretchFactor(0, 5);
    central_hsplitter->setStretchFactor(2, 5);

    left_widget->setLayout(left_vbox_layout);
    mid_widget->setLayout(mid_vbox_layout);
    right_widget->setLayout(right_vbox_layout);
//    left_vbox_layout->addWidget(new QPushButton());
    left_vbox_layout->addWidget(left_tab_widget);
    mid_container->addWidget(mid_widget);
    mid_content->setLayout(mid_container);

    mid_vbox_layout->addWidget(opengl, 10);
    opengl_scroller = new QScrollBar(mid_widget);
    opengl_scroller->setOrientation(Qt::Horizontal);
    mid_vbox_layout->addWidget(opengl_scroller, 1);
    right_vbox_layout->addWidget(new QLineEdit(right_widget));
    right_vbox_layout->addWidget(new QTabWidget(right_widget));

    menu_bar = new QMenuBar(this);
    menu_bar->setGeometry(QRect(0, 0, 300, 23));

    menu_file = new QMenu(QString("文件(&F)"), menu_bar);
    menu_connection = new QMenu(QString("连接(&N)"), menu_bar);
    menu_collect = new QMenu(QString("采集(&C)"), menu_bar);
    menu_match = new QMenu(QString("匹配(&M)"), menu_bar);
    menu_setting = new QMenu(QString("设置(&S)"));
    menu_setting_display = new QMenu(QString("显示(&D)"));
    menu_test = new QMenu(QString("测试(&T)"));
    menu_help = new QMenu(QString("帮助(&H)"));
    menu_file_new = new QAction(this);
    menu_file_new->setText(QString("新建(&N)"));
    menu_connection_channel = new QMenu(QString("通道(&C)"));
    menu_connection_channel_1 = new QAction(this);
    menu_connection_channel_1->setText(QString("通道1(&1)"));
    menu_connection_channel_1->setCheckable(true);
    menu_connection_channel_1->setChecked(true);
    menu_connection_channel_2 = new QAction(this);
    menu_connection_channel_2->setText(QString("通道2(&2)"));
    menu_connection_channel_2->setCheckable(true);
    menu_connection_channel_2->setChecked(false);
    menu_collect_load = new QAction(this);
    menu_collect_load->setText(QString("加载配置(&L)..."));
    menu_collect_config = new QAction(this);
    menu_collect_config->setText(QString("手动配置(&M)..."));
    menu_match_load = new QAction(this);
    menu_match_load->setText(QString("加载配置(&L)..."));
    menu_match_config = new QAction(this);
    menu_match_config->setText(QString("手动配置(&M)..."));
    menu_setting_skin = new QAction(this);
    menu_setting_skin->setText(QString("皮肤(&S)"));
    menu_setting_display_left = new QAction(this);
    menu_setting_display_left->setText(QString("左侧(&L)"));
    menu_setting_display_left->setCheckable(true);
    menu_setting_display_left->setChecked(true);
    menu_setting_display_right = new QAction(this);
    menu_setting_display_right->setText(QString("右侧(&R)"));
    menu_setting_display_right->setCheckable(true);
    menu_setting_display_right->setChecked(true);
    menu_setting_display_statu = new QAction(this);
    menu_setting_display_statu->setText(QString("状态栏(&S)"));
    menu_setting_display_statu->setCheckable(true);
    menu_setting_display_statu->setChecked(true);
    menu_setting_display_bottom = new QAction(this);
    menu_setting_display_bottom->setText(QString("底部(&B)"));
    menu_setting_display_bottom->setCheckable(true);
    menu_setting_display_bottom->setChecked(true);
    menu_setting_fullscreen = new QAction(this);
    menu_setting_fullscreen->setText(QString("全屏(&F)"));
    menu_test_paint = new QAction(this);
    menu_test_paint->setText(QString("绘图(&P)..."));
    menu_help_manual = new QAction(this);
    menu_help_manual->setText(QString("手册(&M)..."));
    menu_help_opensource = new QAction(this);
    menu_help_opensource->setText(QString("开源(&O)..."));
    menu_help_sysinfo = new QAction(this);
    menu_help_sysinfo->setText(QString("信息(&I)..."));
    menu_help_feedback = new QAction(this);
    menu_help_feedback->setText(QString("反馈(&F)..."));
    menu_help_about = new QAction(this);
    menu_help_about->setText(QString("关于(&A)..."));
    menu_file->addAction(menu_file_new);
    menu_connection->addMenu(menu_connection_channel);
    menu_connection_channel->addAction(menu_connection_channel_1);
    menu_connection_channel->addAction(menu_connection_channel_2);
    menu_collect->addAction(menu_collect_load);
    menu_collect->addAction(menu_collect_config);
    menu_match->addAction(menu_match_load);
    menu_match->addAction(menu_match_config);
    menu_setting->addAction(menu_setting_skin);
    menu_setting->addMenu(menu_setting_display);
    menu_setting_display->addAction(menu_setting_display_left);
    menu_setting_display->addAction(menu_setting_display_right);
    menu_setting_display->addAction(menu_setting_display_statu);
    menu_setting_display->addAction(menu_setting_display_bottom);
    menu_setting->addAction(menu_setting_fullscreen);
    menu_test->addAction(menu_test_paint);
    menu_help->addAction(menu_help_manual);
    menu_help->addAction(menu_help_opensource);
    menu_help->addAction(menu_help_sysinfo);
    menu_help->addAction(menu_help_feedback);
    menu_help->addAction(menu_help_about);

    menu_bar->addAction(menu_file->menuAction());
    menu_bar->addAction(menu_connection->menuAction());
    menu_bar->addAction(menu_collect->menuAction());
    menu_bar->addAction(menu_match->menuAction());
    menu_bar->addAction(menu_setting->menuAction());
    menu_bar->addAction(menu_test->menuAction());
    menu_bar->addAction(menu_help->menuAction());

    this->setMenuBar(menu_bar);
    status_bar = new QStatusBar(this);
    this->setStatusBar(status_bar);

    about_dialog = new About(this);
    select_dialog = new Select(this);
    curve_dialog = new Curve(this);
    paint_test_dialog = new PaintTest(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, opengl, &Sketch::animate);
//    timer->start(10);

    connect(menu_collect_load, &QAction::triggered, new QFileDialog,
            &QFileDialog::show);
    connect(menu_match_load, &QAction::triggered, new QFileDialog,
            &QFileDialog::show);
    connect(menu_setting_skin, &QAction::triggered, this, &Window::changeSkin);
    connect(menu_setting_display_left, &QAction::triggered, this,
            &Window::hideDisplay);
    connect(menu_setting_display_right, &QAction::triggered, this,
            &Window::hideDisplay);
    connect(menu_setting_display_statu, &QAction::triggered, this,
            &Window::hideDisplay);
    connect(menu_setting_display_bottom, &QAction::triggered, this,
            &Window::hideDisplay);
    connect(menu_setting_fullscreen, &QAction::triggered, this,
            &Window::fullScreen);
    connect(menu_match_config, &QAction::triggered, select_dialog,
            &Select::show);
    connect(menu_collect_config, &QAction::triggered, curve_dialog,
            &Curve::show);
    connect(menu_test_paint, &QAction::triggered, paint_test_dialog,
            &PaintTest::show);
    connect(menu_help_about, &QAction::triggered, about_dialog, &About::show);

    connect(mid_widget, &Middle::cancelFullScreen, this,
            &Window::fullScreenCancel);
    connect(menu_connection_channel_1, &QAction::triggered, this,
            &Window::changeToChannel1);
    connect(menu_connection_channel_2, &QAction::triggered, this,
            &Window::changeToChannel2);
    before_full_screen = mid_widget->windowFlags();
}

Window::~Window() {
//    delete about_dialog;
}

void Window::resizeEvent(QResizeEvent *event) {
}

void Window::changeSkin() {
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

void Window::hideDisplay() {
    if (menu_setting_display_left->isChecked()) {
        left_widget->show();
    } else {
        left_widget->hide();
    }
    if (menu_setting_display_right->isChecked()) {
        right_widget->show();
    } else {
        right_widget->hide();
    }
    if (menu_setting_display_statu->isChecked()) {
        status_bar->show();
    } else {
        status_bar->hide();
    }
    if (menu_setting_display_bottom->isChecked()) {
        bottom_tab_widget->show();
    } else {
        bottom_tab_widget->hide();
    }
}

void Window::fullScreen() {
    mid_container->removeWidget(mid_widget);
    mid_widget->hide();
    mid_widget->setWindowFlag(Qt::Window);
    mid_widget->showFullScreen();
}

void Window::fullScreenCancel() {
    mid_widget->setWindowFlags(before_full_screen);
    mid_widget->hide();
    mid_widget->setWindowFlag(Qt::Widget);
    mid_container->insertWidget(0, mid_widget);
    mid_widget->show();
}

void Window::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_F11:
            fullScreen();
            break;
        default:
            break;
    }
}

void Window::changeToChannel1() {
    menu_connection_channel_2->setChecked(false);
    menu_connection_channel_1->setChecked(true);
}

void Window::changeToChannel2() {
    menu_connection_channel_1->setChecked(false);
    menu_connection_channel_2->setChecked(true);
}