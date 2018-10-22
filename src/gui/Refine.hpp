//
// Created by jiang.wenqiang on 2018/10/22.
//

#ifndef REFINE_REFINE_HPP
#define REFINE_REFINE_HPP

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStatusBar>
#include "Initializer.hpp"
#include "Revolve.hpp"
#include "FileDialog.hpp"

class Refine : public QMainWindow
{
Q_OBJECT
private:
    Initializer _init;
    Revolve _revolve;

    //ui
    QMenuBar *_menubar;
    QMenu *_menu_file;
    QAction *_menu_file_open;
    QAction *_menu_file_save;
    QAction *_menu_file_exit;
    QMenu *_menu_view;
    QMenu *_menu_view_display;
    QAction *_menu_view_display_west;
    QAction *_menu_view_display_east;
    QAction *_menu_view_display_south;
    QAction *_menu_view_display_tools;
    QAction *_menu_view_full;
    QMenu *_menu_init;
    QAction *_menu_init_setting;
    QAction *_menu_init_can;
    QAction *_menu_init_curve;
    QAction *_menu_init_mode;
    QMenu *_menu_control;
    QAction *_menu_control_start;
    QAction *_menu_control_pause;
    QAction *_menu_control_resume;
    QAction *_menu_control_finish;
    QMenu *_menu_help;
    QAction *_menu_help_tutorial;
    QAction *_menu_help_version;
    QAction *_menu_help_license;
    QAction *_menu_help_feedback;
    QAction *_menu_help_about;

    //widgets
    FileDialog _file_dialog;

public:
    Refine();

private:
    inline void initMenu(QMenu *&menu, QString &&title,
                          QMenuBar *m)
    {
        menu = new QMenu(title, m);
        m->addMenu(menu);
    }

    inline void initMenu(QMenu *&menu, QString &&title,
                         QMenu *m)
    {
        menu = new QMenu(title, m);
        m->addMenu(menu);
    }

    inline void initMenu(QAction *&action, QString &&title,
                         QMenu *m)
    {
        action = new QAction(title, m);
        m->addAction(action);

    }

    void setup();
};


#endif //REFINE_REFINE_HPP
