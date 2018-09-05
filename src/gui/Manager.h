//
// Created by jiang.wenqiang on 2018/9/5.
//

#ifndef REFINE_MANAGER_H
#define REFINE_MANAGER_H

#include <QtWidgets/QMainWindow>

class Manager : public QMainWindow{
Q_OBJECT
private:
public:
    explicit Manager(QWidget *parent = nullptr);



private:
    void setupUi();
};


#endif //REFINE_MANAGER_H
