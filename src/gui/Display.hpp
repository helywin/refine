//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_DISPLAY_HPP
#define REFINE_DISPLAY_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>

class CurveViewer;
class Revolve;
class Sketch;

class Display : public QWidget
{
Q_OBJECT
private:
    CurveViewer *_viewer;
    QVBoxLayout *_layout;
    QToolBar *_toolbar;
public:
    Display(QWidget *parent, Revolve *revolve);

    Sketch &sketch();
private:
    void setup();
};


#endif //REFINE_DISPLAY_HPP
