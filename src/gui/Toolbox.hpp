//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_DOCKER_HPP
#define REFINE_DOCKER_HPP

#include <QtWidgets/QDockWidget>
#include "ByteRangeFrame.hpp"

class Toolbox : public QDockWidget
{
Q_OBJECT
private:
public:
    explicit Toolbox(QWidget *parent);
private:
    void setup();
};


#endif //REFINE_DOCKER_HPP
