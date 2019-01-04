//
// Created by jiang.wenqiang on 2019/1/4.
//

#ifndef REFINE_CANCONFIG_HPP
#define REFINE_CANCONFIG_HPP

#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>

#include "Message.hpp"

class CanConfig : public QDialog , public Message
{
Q_OBJECT
private:
    QHBoxLayout *_layout;
    QGroupBox *_param;
    QVBoxLayout *_param_layout;
    QWidget *_selection;
    QFormLayout *_selection_layout;
    QLabel *_device_type_label;
    QLabel *_device_index_label;
    QLabel *_device_channel_label;
    QLabel *_baudrate_label;
    QLabel *_acc_code_label;
    QLabel *_acc_mask_label;
    QLabel *_filter_label;
    QLabel *_mode_label;
    QComboBox *_device_type;
    QComboBox *_device_index;
    QComboBox *_device_channel;
    QComboBox *_baudrate;
    QComboBox *_acc_code;
    QComboBox *_acc_mask;
    QComboBox *_filter;
    QComboBox *_mode;
    QWidget *_control;
    QGridLayout *_control_layout;
    QPushButton *_open;
    QPushButton *_init;
    QPushButton *_start;
    QPushButton *_connect;
    QPushButton *_reset;
    QPushButton *_close;

    QGroupBox *_id;
    QGridLayout *_id_layout;
    QTableWidget *_send_id_list;
    QTableWidget *_recv_id_list;
    QLineEdit *_send_id;
    QLineEdit *_recv_id;
    QPushButton *_add_send;
    QPushButton *_add_recv;

public:
    explicit CanConfig(Message *message = nullptr, QWidget *parent = nullptr);
private:
    void setup();
};


#endif //REFINE_CANCONFIG_HPP
