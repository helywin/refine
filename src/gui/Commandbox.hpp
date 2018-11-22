//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_DOCKER_HPP
#define REFINE_DOCKER_HPP

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include "BaudRate.hpp"

class Commandbox : public QDockWidget
{
Q_OBJECT
private:
    QWidget *_content;
    QVBoxLayout *_layout;
    QTabWidget *_tab;
public:
    explicit Commandbox(QWidget *parent);
private:
    void setup();
};


#endif //REFINE_DOCKER_HPP
