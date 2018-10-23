//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_STATUSBAR_HPP
#define REFINE_STATUSBAR_HPP

#include <QtWidgets/QStatusBar>

class StatusBar : public QStatusBar
{
Q_OBJECT
private:
public:
    StatusBar(QWidget *parent);
private:
    void setup();
};


#endif //REFINE_STATUSBAR_HPP
