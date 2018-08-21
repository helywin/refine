//
// Created by jiang.wenqiang on 2018/7/19.
//

#ifndef REFINE_WINDOW_H
#define REFINE_WINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QSplitter>
#include "About.h"
#include "Select.h"
#include "Middle.h"
#include "CurveDialog.h"
#include "Sketch.h"
#include "PaintTest.h"
#include "ModePanel.h"

class Window : public QMainWindow {
Q_OBJECT
private:
    QWidget *buttom_widget;
    QMenuBar *menu_bar;
    QSplitter *central_hsplitter;
    QSplitter *central_vsplitter;
    QMenu *menu_file;
    QMenu *menu_connection;
    QMenu *menu_connection_channel;
    QMenu *menu_collect;
    QMenu *menu_match;
    QMenu *menu_setting;
    QMenu *menu_setting_display;
    QMenu *menu_test;
    QMenu *menu_help;
    QStatusBar *status_bar;
    QAction *menu_file_new;
    QAction *menu_connection_channel_1;
    QAction *menu_connection_channel_2;
    QAction *menu_collect_load;
    QAction *menu_collect_config;
    QAction *menu_match_load;
    QAction *menu_match_config;
    QAction *menu_setting_skin;
    QAction *menu_setting_display_left;
    QAction *menu_setting_display_right;
    QAction *menu_setting_display_statu;
    QAction *menu_setting_display_bottom;
    QAction *menu_setting_fullscreen;
    QAction *menu_test_paint;
    QAction *menu_help_manual;
    QAction *menu_help_opensource;
    QAction *menu_help_sysinfo;
    QAction *menu_help_feedback;
    QAction *menu_help_about;
    QVBoxLayout *mid_container;
    QVBoxLayout *left_vbox_layout;
    QVBoxLayout *mid_vbox_layout;
    QVBoxLayout *right_vbox_layout;
    QWidget *left_widget;
    QWidget *mid_content;
    Middle *mid_widget;
    QWidget *right_widget;
    Sketch *opengl;
    QScrollBar *opengl_scroller;
    QTabWidget *left_tab_widget;
    QTabWidget *bottom_tab_widget;
    Qt::WindowFlags before_full_screen;

    ModePanel *mode;

    CurveDialog *curve_dialog;
    Select *select_dialog;
    PaintTest *paint_test_dialog;
    About *about_dialog;
public:
    explicit Window(QWidget *parent = nullptr);

    ~Window() final;

    void setupUi();

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:

    void changeSkin();

    void hideDisplay();

    void fullScreen();

    void changeToChannel1();

    void changeToChannel2();

public slots:


    void fullScreenCancel();

protected:
    void keyPressEvent(QKeyEvent *event) override;

public:
};


#endif //REFINE_WINDOW_H
