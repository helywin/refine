//
// Created by jiang.wenqiang on 2018/7/19.
//

#ifndef REFINE_WINDOW_H
#define REFINE_WINDOW_H

#include <QtWidgets/QMainWindow>

class Window : public QMainWindow{
Q_OBJECT
private:
    QMenuBar *menu_bar;
    QMenu *menu;
    QStatusBar *status_bar;
public:
    explicit Window(QWidget *parent = nullptr);
    ~Window() final;

    void setup_ui();
    void break_ui();
};


#endif //REFINE_WINDOW_H
