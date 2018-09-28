//
// Created by jiang.wenqiang on 2018/9/28.
//

#ifndef REFINE_SCOPE_HPP
#define REFINE_SCOPE_HPP

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QSplitter>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include "Buffer.hpp"
#include "Curve.hpp"
#include "Can.hpp"
#include "Revolve.hpp"
#include "Tribe.hpp"
#include "Collect.hpp"
#include "Scope.hpp"
#include "Display.hpp"

class Scope : public QMainWindow
{
Q_OBJECT
private:
    QMenuBar *_menubar;
    QMenu *_menu_init;
    QAction *_menu_init_can;
    QAction *_menu_init_curve;
    QMenu *_menu_collect;
    QAction *_menu_collect_start;
    QAction *_menu_collect_pause;
    QAction *_menu_collect_resume;
    QAction *_menu_collect_stop;
    QMenu *_menu_help;
    QAction *_menu_help_about;
    QSplitter *_central_splitter;
    Display *_display;
    QStatusBar *_statusbar;
    QTimer *_timer;

private:
    Can::Config *_config;
    Can *_can;
    Curve *_curve;
    QFile *_file_frames;
    Buffer *_buffer;
    Collect *_collect;
    Tribe *_tribe;
    Transform *_transform;
    Revolve *_revolve;
    bool _core_initialized;

public:
    explicit Scope(QWidget *parent = nullptr);
    ~Scope();

    void setRevolve(Revolve *revolve);

private:
    void setupUi();

    bool setupCore();

    void releseCore();

private slots:

    void start() { _revolve->startRevolve(); _timer->setInterval(10); _timer->start(); }

    void pause() { _revolve->pauseRevolve(); _timer->stop(); }

    void resume() { _revolve->resumeRevolve(); _timer->setInterval(10); _timer->start(); }

    void stop() { _revolve->stopRevolve(); _timer->stop(); }
};


#endif //REFINE_SCOPE_HPP
