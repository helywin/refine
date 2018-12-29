//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_DOCKER_HPP
#define REFINE_DOCKER_HPP

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include "BaudRate.hpp"
#include "Message.hpp"

class ExtraPanel;
class MessagePanel;
class Revolve;

class ToolBox : public QDockWidget, public Message
{
Q_OBJECT
private:
    QWidget *_panel;
    QVBoxLayout *_layout;
    QTabWidget *_messages;
    QTextEdit *_text;
    MessagePanel *_message_panel;
    QLineEdit *_command;
    QGroupBox *_burn_frame;
    QVBoxLayout *_burn_layout;
    QComboBox *_msec;
    QComboBox *_frames;
    QComboBox *_file_name;
    QPushButton *_browser;
    QPushButton *_burn;
    QWidget *_content;

    Revolve *_revolve;

public:
    explicit ToolBox(Message *message, Revolve *revolve, QWidget *parent = nullptr);

    MessagePanel *messagePanel()
    { return _message_panel; }

public slots:
    void getCanMessage(const QString &msg);

private:
    void setup();
};


#endif //REFINE_DOCKER_HPP
