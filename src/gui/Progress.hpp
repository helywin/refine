//
// Created by jiang.wenqiang on 2018/11/14.
//

#ifndef REFINE_PROGRESS_HPP
#define REFINE_PROGRESS_HPP

#include <QtWidgets/QDialog>

class Progress : QDialog
{
    Q_OBJECT
private:
public:
    Progress(QWidget *parent = nullptr);
private:
    void setup();
    void paintEvent(QPaintEvent *event) override;
};


#endif //REFINE_PROGRESS_HPP
