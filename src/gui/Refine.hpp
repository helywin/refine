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
#include "CommandBox.hpp"
#include "CurveEditor.hpp"
#include "MessagerPanel.hpp"
#include "Display.hpp"
#include "StatusBar.hpp"
#include "TribeBox.hpp"
#include "MarkBox.hpp"
#include "Settings.hpp"
#include "About.hpp"
#include "Message.hpp"
#include "BaudRate.hpp"
#include "Feedback.hpp"

class OutputBox;
class MessagerPanel;
class ChangeLog;
class Settings;

class Refine : public QMainWindow, public Message
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
    QAction *_menu_file_import_frame;
    QAction *_menu_file_import_mode;
    QMenu *_menu_file_export;
    QAction *_menu_file_export_config;
    QAction *_menu_file_export_data;
    QAction *_menu_file_export_frame;
    QAction *_menu_file_export_mode;
    QAction *_menu_file_settings;
    QAction *_menu_file_exit;
    QMenu *_menu_view;
    QMenu *_menu_view_display;
    QActionGroup *_menu_view_display_group;
    QAction *_menu_view_display_file;
    QAction *_menu_view_display_command;
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
    QMenu *_menu_control;
    QAction *_menu_control_start;
    QAction *_menu_control_pause;
    QAction *_menu_control_resume;
    QAction *_menu_control_finish;
    QMenu *_menu_tools;
    QAction *_menu_tool_editcurve;
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
    QAction *_menu_help_aboutqt;
    QToolBar *_toolbar_file;

    QWidget *_central;
    QHBoxLayout *_layout;
    CommandBox *_commandbox;
    OutputBox *_outputbox;
    TribeBox *_tribebox;
    MarkBox *_markbox;
    Display *_display;
    ChangeLog *_changelog;
    Feedback *_feedback;
    About *_about;
    Settings *_settings;
    CurveEditor *_editor;

    StatusBar *_statusbar;
    BaudRate *_baud_rate;

    Qt::WindowState _win_state;
    bool _presentation;
    //widgets
    FilePicker *_file_picker;
    QTime _timer[3];
    bool _timer_start[3];
    QTimer _wake_up;

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

    void setCurveViewerActions();

private slots:

    void fullScreen();

    void presentation();

    void exitPresentation();

    void startRevolve();

    void stopRevolve();

    void connectCan();

    void startTimers(QAction *action);

    void changeUpdateMsec(QAction *action);

    void displayAndHide(QAction *action);

    void setSmooth();

    void widgetsVisibilityChanged(bool visible);

    void setCollectMenuEnable(bool enable);

    void setWakeUp();

    void keepWakeUp();

signals:
    void message(int type, const QString &msg);

protected:
    void closeEvent(QCloseEvent *event) override;

    inline void emitMessage(MessageType type, const QString &msg) override
    {
        emit message(type, msg);
    }
};


#endif //REFINE_REFINE_HPP
