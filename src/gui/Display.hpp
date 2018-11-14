//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_DISPLAY_HPP
#define REFINE_DISPLAY_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include "Message.hpp"

class CurveViewer;
class Revolve;
class Sketch;

class Display : public QWidget, public Message
{
Q_OBJECT
private:
    CurveViewer *_viewer;
    QVBoxLayout *_layout;
    QToolBar *_toolbar;
    QAction *_zoom_plus;
    QAction *_zoom_minus;
public:
    Display(QWidget *parent, Revolve *revolve, Message *message = nullptr);

    Sketch &sketch();
private:
    void setup();
};


#endif //REFINE_DISPLAY_HPP
