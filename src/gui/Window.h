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
#include "Curve.h"

class Window : public QMainWindow{
Q_OBJECT
private:
    QWidget *central_widget;
    QMenuBar *menu_bar;
    QSplitter *central_splitter;
    QMenu *menu_file;
    QMenu *menu_collect;
    QMenu *menu_match;
    QMenu *menu_setting;
    QMenu *menu_setting_display;
    QMenu *menu_help;
    QStatusBar *status_bar;
    QAction *menu_file_new;
    QAction *menu_collect_load;
    QAction *menu_collect_config;
    QAction *menu_match_load;
    QAction *menu_match_config;
    QAction *menu_setting_skin;
    QAction *menu_setting_display_left;
    QAction *menu_setting_display_right;
    QAction *menu_setting_display_statu;
    QAction *menu_setting_fullscreen;
    QAction *menu_help_about;
    QAction *menu_help_feedback;
    QVBoxLayout *mid_container;
    QVBoxLayout *left_vbox_layout;
    QVBoxLayout *mid_vbox_layout;
    QVBoxLayout *right_vbox_layout;
    QWidget *left_widget;
    QWidget *mid_content;
    Middle *mid_widget;
    QWidget *right_widget;
    QOpenGLWidget *opengl;
    QScrollBar *opengl_scroller;
    QTabWidget *left_tab_widget;
    Qt::WindowFlags before_full_screen;

    Curve *curve_dialog;
    Select *select_dialog;
    About *about_dialog;
public:
    explicit Window(QWidget *parent = nullptr);
    ~Window() final;

    void setup_ui();

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

public slots:
    void change_skin();

    void hide_display();

    void full_screen();

    void full_screen_cancel();

protected:
    void keyPressEvent(QKeyEvent *event) override;

public:
};


#endif //REFINE_WINDOW_H
