//
// Created by jiang.wenqiang on 2018/7/30.
//

#ifndef REFINE_SENDDIALOG_H
#define REFINE_SENDDIALOG_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include "Curve.h"
#include "Can.h"
#include "Transforo.h"
#include "Transmit.h"
#include "Revolvo.h"
#include "Load.h"


class SendDialog : public QMainWindow {
Q_OBJECT
private:
    QWidget *_widget_central;
    QMenuBar *_menu_bar;
    QMenu *_menu_file;
    QMenu *_menu_file_data;
    QMenu *_menu_file_config;
    QMenu *_menu_file_exit;
    QPushButton *_button_start;

    Can::Config _config;
    Can _can;
    Buffer _buffer;
    QFile

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit SendDialog(QWidget *parent = nullptr);

    ~SendDialog() final = default;

private:
    void setupUi();
};


#endif //REFINE_SENDDIALOG_H
