//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_DOCKER_HPP
#define REFINE_DOCKER_HPP

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include "BaudRate.hpp"
#include "Message.hpp"

class CommandPanel;

class Commandbox : public QDockWidget, public Message
{
Q_OBJECT
private:
    QWidget *_content;
    QVBoxLayout *_layout;
    QTabWidget *_tab;
    CommandPanel *_panel;

public:
    explicit Commandbox(Message *message, QWidget *parent = nullptr);
private:
    void setup();
};


#endif //REFINE_DOCKER_HPP
