//
// Created by jiang.wenqiang on 2018/7/19.
//

#ifndef REFINE_WINDOW_H
#define REFINE_WINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QScrollBar>

class Window : public QMainWindow{
Q_OBJECT
private:
    QWidget *central_widget;
    QMenuBar *menu_bar;
    QMenu *menu_file;
    QMenu *menu_setting;
    QStatusBar *status_bar;
    QAction *menu_action_new;
    QAction *menu_setting_skin;
    QHBoxLayout *whole_hbox_layout;
    QVBoxLayout *mid_vbox_layout;
    QWidget *mid_widget;
    QOpenGLWidget *opengl;
    QScrollBar *opengl_scroller;


    QTabWidget *left_tab_widget;

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window() final;

    void setup_ui();

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

public slots:
    void change_skin();

};


#endif //REFINE_WINDOW_H
