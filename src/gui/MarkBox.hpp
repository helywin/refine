//
// Created by jiang.wenqiang on 2018/10/24.
//

#ifndef REFINE_MARKBOX_HPP
#define REFINE_MARKBOX_HPP

#include <QtWidgets/QDockWidget>

class MarkBox : public QDockWidget
{
    Q_OBJECT
private:
public:
    explicit MarkBox(QWidget *parent);
private:
    void setup();
};

#endif //REFINE_MARKBOX_HPP
