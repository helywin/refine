//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_OUTPUT_HPP
#define REFINE_OUTPUT_HPP

#include <QtWidgets/QDockWidget>

class Output : public QDockWidget
{
Q_OBJECT
private:
public:
    explicit Output(QWidget *parent);
private:
    void setup();
};


#endif //REFINE_OUTPUT_HPP
