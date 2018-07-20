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
#include "About.h"

class Window : public QMainWindow{
Q_OBJECT
private:
    QWidget *central_widget;
    QMenuBar *menu_bar;
    QMenu *menu_file;
    QMenu *menu_collect;
    QMenu *menu_match;
    QMenu *menu_setting;
    QMenu *menu_help;
    QStatusBar *status_bar;
    QAction *menu_file_new;
    QAction *menu_collect_load;
    QAction *menu_collect_config;
    QAction *menu_match_load;
    QAction *menu_match_config;
    QAction *menu_setting_skin;
    QAction *menu_help_about;
    QAction *menu_help_feedback;
    QHBoxLayout *whole_hbox_layout;
    QVBoxLayout *left_vbox_layout;
    QVBoxLayout *mid_vbox_layout;
    QVBoxLayout *right_vbox_layout;
    QWidget *left_widget;
    QWidget *mid_widget;
    QWidget *right_widget;
    QOpenGLWidget *opengl;
    QScrollBar *opengl_scroller;
    QTabWidget *left_tab_widget;

    About *about_dialog;
public:
    explicit Window(QWidget *parent = nullptr);
    ~Window() final;

    void setup_ui();

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

public slots:
    void change_skin();

    void about();

    void hide_about();
};


#endif //REFINE_WINDOW_H
