//
// Created by jiang.wenqiang on 2018/10/22.
//

#ifndef REFINE_REFINE_HPP
#define REFINE_REFINE_HPP

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QToolBar>
#include <QtGui/QKeyEvent>
#include <QtCore/QTranslator>
#include "Initializer.hpp"
#include "Revolve.hpp"
#include "FilePicker.hpp"
#include "Toolbox.hpp"
#include "CurveEditor.hpp"
#include "Messager.hpp"
#include "Display.hpp"
#include "StatusBar.hpp"
#include "CurveBox.hpp"
#include "MarkBox.hpp"

class Output;
class Messager;

class Refine : public QMainWindow
{
Q_OBJECT
private:
    Initializer _init;
    Revolve _revolve;
    QTranslator _translator;
    //ui
    QMenuBar *_menubar;
    QMenu *_menu_file;
    QAction *_menu_file_open;
    QAction *_menu_file_save;
    QAction *_menu_file_config;
    QAction *_menu_file_exit;
    QMenu *_menu_view;
    QMenu *_menu_view_display;
    QAction *_menu_view_display_tools;
    QAction *_menu_view_display_docker;
    QAction *_menu_view_display_output;
    QAction *_menu_view_display_curve;
    QAction *_menu_view_display_mark;
    QAction *_menu_view_full;
    QAction *_menu_view_presentation;
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
    QToolBar *_toolbar_menu;

    Toolbox *_toolbox;
    Output *_output;
    CurveBox *_curvebox;
    MarkBox *_markbox;
    Display *_display;

    StatusBar *_statusbar;

    Qt::WindowState _win_state;
    bool _presentation;
    //widgets
    FilePicker *_file_picker;

public:
    Refine();

private:
    void setup();

    void setLanguage();

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

    void initMenu(QAction *&action, QString &&title,
                  QMenu *m, const QString &tips,
                  bool checkable = false,
                  bool checked = false)
    {
        action = new QAction(title, m);
        m->addAction(action);
        action->setStatusTip(tips);
        action->setCheckable(checkable);
        action->setChecked(checked);
    }

    inline void initMenu(QAction *&action, QString &&title,
                         QMenu *m, const QString &tips,
                         const QKeySequence &seq,
                         bool checkable = false,
                         bool checked = false)
    {
        initMenu(action, qMove(title), m, tips, checkable, checked);
        action->setShortcut(seq);
    }

private slots:

    void fullScreen()
    {
        if (_menu_view_full->isChecked()) {
            emit message(Messager::MessageType::Debug, tr("进入全屏模式"));
            this->setWindowState(Qt::WindowFullScreen);
        } else {
            emit message(Messager::MessageType::Debug, tr("退出全屏模式"));
            this->setWindowState(Qt::WindowMaximized);
        }
    }

    void startRevolve();

    void connectCan();
signals:
    void message(Messager::MessageType type, const QString &msg);
};


#endif //REFINE_REFINE_HPP
