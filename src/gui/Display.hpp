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
#include "Message.hpp"

class CurveViewer;
class Revolve;
class Sketch;

class Display : public QSplitter, public Message
{
Q_OBJECT
private:
    CurveViewer *_viewer;
    QWidget *_left_panel;
    QTabWidget *_right_tab;
    QVBoxLayout *_layout;
    QToolBar *_toolbar;
    QAction *_zoom_plus;
    QAction *_zoom_minus;
    QVector<QDockWidget *> _docks;
    QVector<QWidget *> _widgets;
    bool _presentation;
    Qt::WindowFlags _before_fullscreen;
    QLayout *_parent_layout;
public:
    Display(QWidget *parent, Revolve *revolve, Message *message = nullptr);

    Sketch &sketch();

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

signals:
    void exitPresentation();
};


#endif //REFINE_DISPLAY_HPP
