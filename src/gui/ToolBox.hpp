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

class ExtraPanel;
class MessagePanel;
class Revolve;

class CommandBox : public QDockWidget, public Message
{
Q_OBJECT
private:
    QWidget *_content;
    QVBoxLayout *_layout;
    ExtraPanel *_panel;
    Revolve *_revolve;

public:
    explicit CommandBox(Message *message, Revolve *revolve, QWidget *parent = nullptr);
private:
    void setup();
};


#endif //REFINE_DOCKER_HPP
