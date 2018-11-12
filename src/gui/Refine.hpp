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
#include <QtWidgets/QMessageBox>
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
#include "Settings.hpp"

class Output;
class Messager;
class ChangeLog;
class Settings;

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
    QMenu *_menu_file_import;
    QAction *_menu_file_import_config;
    QAction *_menu_file_import_data;
    QMenu *_menu_file_export;
    QAction *_menu_file_export_config;
    QAction *_menu_file_export_data;
    QAction *_menu_file_settings;
    QAction *_menu_file_exit;
    QMenu *_menu_view;
    QMenu *_menu_view_display;
    QActionGroup *_menu_view_display_group;
    QAction *_menu_view_display_file;
    QAction *_menu_view_display_tools;
    QAction *_menu_view_display_output;
    QAction *_menu_view_display_curve;
    QAction *_menu_view_display_mark;
    QAction *_menu_view_full;
    QAction *_menu_view_presentation;
    QMenu *_menu_view_sketchmsec;
    QActionGroup *_menu_view_sketchmsec_group;
    QAction *_menu_view_sketchmsec_10;
    QAction *_menu_view_sketchmsec_20;
    QAction *_menu_view_sketchmsec_30;
    QAction *_menu_view_sketchmsec_50;
    QAction *_menu_view_sketchmsec_100;
    QAction *_menu_view_smooth;
    QMenu *_menu_init;
    QAction *_menu_init_option;
    QAction *_menu_init_can;
    QAction *_menu_init_curve;
    QAction *_menu_init_editcurve;
    QAction *_menu_init_mode;
    QMenu *_menu_control;
    QAction *_menu_control_start;
    QAction *_menu_control_pause;
    QAction *_menu_control_resume;
    QAction *_menu_control_finish;
    QMenu *_menu_tools;
    QMenu *_menu_tools_timer;
    QActionGroup *_menu_tools_timer_group;
    QAction *_menu_tools_timers[3];
    QAction *_menu_tools_wakeup;
    QMenu *_menu_help;
    QAction *_menu_help_tutorial;
    QAction *_menu_help_changelog;
    QAction *_menu_help_license;
    QAction *_menu_help_feedback;
    QAction *_menu_help_about;
    QToolBar *_toolbar_file;

    Toolbox *_toolbox;
    Output *_output;
    CurveBox *_curvebox;
    MarkBox *_markbox;
    Display *_display;
    ChangeLog *_changelog;
    Settings *_settings;
    CurveEditor *_editor;

    StatusBar *_statusbar;

    Qt::WindowState _win_state;
    bool _presentation;
    //widgets
    FilePicker *_file_picker;
    QTime _timer[3];
    bool _timer_start[3];

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

    void initMenu(QAction *&action, QString &&title,
                  QActionGroup *g, QMenu *m, const QString &tips,
                  bool checkable = false,
                  bool checked = false)
    {
        action = new QAction(title, g);
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

    inline void initMenu(QAction *&action, QString &&title,
                         QActionGroup *g, QMenu *m, const QString &tips,
                         const QKeySequence &seq,
                         bool checkable = false,
                         bool checked = false)
    {
        initMenu(action, qMove(title), g, m, tips, checkable, checked);
        action->setShortcut(seq);
    }

private slots:

    void fullScreen();

    void startRevolve();

    void stopRevolve();

    void connectCan();

    void startTimers(QAction *action);

    void changeUpdateMsec(QAction *action);

    void displayAndHide(QAction *action);

    void setSmooth();
signals:
    void message(int type, const QString &msg);

protected:
    void closeEvent(QCloseEvent *event) override;
};


#endif //REFINE_REFINE_HPP
