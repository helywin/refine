//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_DISPLAY_HPP
#define REFINE_DISPLAY_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QActionGroup>
#include "Message.hpp"
#include "CurveViewer.hpp"

class Revolve;
class Sketch;
class SketchY;

class Display : public QSplitter, public Message
{
Q_OBJECT
private:
    CurveViewer *_viewer;
    QWidget *_left_panel;
    QTabWidget *_right_tab;
    QVBoxLayout *_layout;
    QToolBar *_toolbar;
    QMenuBar *_menubar;
    QMenu *_menu_zoom;
    QActionGroup *_menu_zoom_axis;
    QAction *_menu_zoom_x;
    QAction *_menu_zoom_y;
    QAction *_menu_zoom_xy;
    QActionGroup *_menu_zoom_group;
    QAction *_menu_zoom_plus;
    QAction *_menu_zoom_minus;
    QAction *_menu_zoom_origin;
    QAction *_menu_zoom_minimum;
    QAction *_zoom_plus;
    QAction *_zoom_minus;
    QPushButton *_btn_zoom;
    QPushButton *_btn_zoom_minus;
    QVector<QDockWidget *> _docks;
    QVector<QWidget *> _widgets;
    bool _presentation;
    Qt::WindowFlags _before_fullscreen;
    QLayout *_parent_layout;

public:
    Display(QWidget *parent, Revolve *revolve, Message *message = nullptr);

    inline Sketch *sketch() { return _viewer->sketch(); }

    inline SketchY *sketchY() { return _viewer->sketchY(); }

    inline SketchXTop *sketchXTop() { return _viewer->sketchXTop(); }

    inline CurveViewer *curveViewer() { return _viewer; }

    inline void addDock(QDockWidget *dock)
    {
        _docks.append(dock);
        _widgets.append(dock->widget());
    }

public slots:
    void enablePresentation();

    void disablePresentation();

    inline void setParentLayout(QLayout *layout) { _parent_layout = layout; }

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
private:
    void setup();

private slots:
    void changeZoomMode(QAction *action);

signals:
    void exitPresentation();
};


#endif //REFINE_DISPLAY_HPP
