//
// Created by jiang.wenqiang on 2018/10/30.
//

#ifndef REFINE_SETTINGS_HPP
#define REFINE_SETTINGS_HPP

#include <QtWidgets/QDialog>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QGroupBox>

class Settings : public QDialog
{
Q_OBJECT
private:
    QVBoxLayout *_layout;
    QPushButton *_button_ok;
    QPushButton *_button_cancel;
    QPushButton *_button_apply;
    QSplitter *_splitter;
    QWidget *_list;
    QScrollArea *_setting_page;
    QWidget *_page;
    QGroupBox *_group_look;

public:
    Settings();
private:
    void setup();
};


#endif //REFINE_SETTINGS_HPP
