//
// Created by jiang.wenqiang on 2018/7/31.
//

#ifndef REFINE_MODEPANEL_H
#define REFINE_MODEPANEL_H

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

class ModePanel : public QWidget{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    QWidget *button_widget;
    QHBoxLayout * button_layout;
    QTableWidget *mode_list;
    QPushButton *start_mode;
    QPushButton *stop_mode;
public:
    explicit ModePanel(QWidget *parent);

private:
    void setupUi();

};


#endif //REFINE_MODEPANEL_H
