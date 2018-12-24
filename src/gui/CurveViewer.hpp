//
// Created by jiang.wenqiang on 2018/10/24.
//

#ifndef REFINE_CURVEVIEWER_HPP
#define REFINE_CURVEVIEWER_HPP

#include <QtCore/QTimer>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QActionGroup>
#include "Message.hpp"
#include "Sketch.hpp"
#include "Global.hpp"

class SketchXTop;
class SketchX;
class SketchY;
class Revolve;
class Tribe;

class CurveViewer : public QWidget, public Message
{
Q_OBJECT
private:
    SketchY *_sketch_y;
    SketchXTop *_sketch_xtop;
    SketchX *_sketch_x;
    Sketch *_sketch;
    QOpenGLWidget *_sketch_yright;
    QGridLayout *_layout;
    QWidget *_widget_sketch;
    QGridLayout *_layout_sketch;
    QScrollBar *_h_scroll;
    QScrollBar *_v_scroll;
    Tribe *_tribe;
    bool _zoom_x;
    bool _zoom_y;
    int _msec;
    Re::RunningStatus _status;
    Re::RunningCommand _command;
    QTimer _timer;

    bool _h_scroll_pressed;
    bool _is_rolling;

    QMenu *_menu;
    QAction *_menu_start;
    QAction *_menu_pause;
    QAction *_menu_resume;
    QAction *_menu_stop;

    QAction *_menu_load;
    QAction *_menu_save;
    QAction *_menu_current;

    QAction *_menu_copy_img;
    QAction *_menu_save_img;
    QAction *_menu_clip_img;

    QAction *_menu_vernier;
    QAction *_menu_pattern;
    QAction *_menu_marks;

    QMenu *_menu_style;
    QAction *_menu_settings;

public:
    explicit CurveViewer(QWidget *parent, Revolve *revolve, Message *message = nullptr);

    inline Sketch *sketch() { return _sketch; }

    inline SketchY *sketchY() { return _sketch_y; }

    inline SketchXTop *sketchXTop() { return _sketch_xtop; }

    void resetHScroll(int len, bool reset);
    void resetVScroll(int len);

    inline void setMsec(int msec)
    {
        _msec = msec;
        _timer.setInterval(_msec);
    }

    void start();
    void pause();
    void resume();
    void stop();

    void regen(bool zoom = true);

public slots:
    void zoomPlus(double x_rate, double x_start, double y_rate, double y_start);
    void zoomMinus(double x_rate, double x_start,
                   double y_rate, double y_start, Sketch::ZoomEdges edge);
    void parallelMove(double delta_x, double delta_y);
//    void zoomPlusFixed();
//    void zoomMinusFixed();
    void zoomDefault();
    void zoomMinimum();
    void setXZoom(bool flag);
    void setYZoom(bool flag);

    inline void setMenuStart(QAction *action) { _menu_start = action; };
    inline void setMenuPause(QAction *action) { _menu_pause = action; };
    inline void setMenuResume(QAction *action) { _menu_resume = action; };
    inline void setMenuStop(QAction *action) { _menu_stop = action; };
    inline void setMenuLoad(QAction *action) { _menu_load = action; };
    inline void setMenuSave(QAction *action) { _menu_save = action; };
    inline void setMenuCurrent(QAction *action) { _menu_current = action; };
    inline void setMenuSettings(QAction *action) { _menu_settings = action; };
    void finishMenuSet();

private:
    void setup();
    void zoomX(double rate, double start);
    void zoomY(double rate, double start);
    void zoomXMinusEdgeLeft(double rate);
    void zoomXMinusEdgeRight(double rate);
    void zoomYMinusEdgeBottom(double rate);
    void zoomYMinusEdgeTop(double rate);
    void zoomXDefault();
    void zoomYDefault();
    void zoomXMinimum();
    void zoomYMinimum();
    void rollViewer();

    inline void initMenu(QMenu *&menu, QString &&title,
                         QMenu *m)
    {
        menu = new QMenu(title, m);
        m->addMenu(menu);
    }

    void initMenu(QAction *&action, QString &&title,
                  QMenu *m)
    {
        action = new QAction(title, m);
        m->addAction(action);
        action->setStatusTip(title);
    }

    QImage piecePicture();

private slots:
    void hScrollChanged(int value);
    void vScrollChanged(int value);
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};


#endif //REFINE_CURVEVIEWER_HPP
