//
// Created by jiang.wenqiang on 2018/12/20.
//

#ifndef REFINE_COMMANDPANEL_HPP
#define REFINE_COMMANDPANEL_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include "Message.hpp"

class Revolve;

class CommandPanel : public QWidget, public Message
{
Q_OBJECT
private:
    QVBoxLayout *_layout;
    QTextEdit *_text;
    QLineEdit *_command;
    QGroupBox *_burn_frame;
    QVBoxLayout *_burn_layout;
    QComboBox *_msec;
    QComboBox *_frames;
    QComboBox *_file_name;
    QPushButton *_browser;
    QPushButton *_burn;
    Revolve *_revolve;

public:
    CommandPanel(Message *message, Revolve *revolve, QWidget *parent = nullptr);

private:
    void setup();
};


#endif //REFINE_COMMANDPANEL_HPP
