//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_DOCKER_HPP
#define REFINE_DOCKER_HPP

#include <QtWidgets/QDockWidget>
#include "ByteRangeFrame.hpp"
#include "InfiniteProgress.hpp"

class Toolbox : public QDockWidget
{
Q_OBJECT
private:
    InfiniteProgress *_progress;
public:
    explicit Toolbox(QWidget *parent);
private:
    void setup();
};


#endif //REFINE_DOCKER_HPP
