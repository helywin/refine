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
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include "Message.hpp"
#include "HexInput.hpp"

class CanConfig : public QDialog, public Message
{
Q_OBJECT
public:
    class Id
    {
    private:
        int _data;
        QString _remark;
        bool _is_checked;
    public:
        explicit Id(int data, const QString &remark = QString(), bool is_checked = false) :
                _data(data), _remark(remark), _is_checked(is_checked)
        {}

        inline int data() const { return _data;}

        inline QString remark() const { return _remark; }

        inline bool isChecked() const { return _is_checked;}

        inline void setData(int data) { _data = data; }

        inline void setRemark(const QString &remark) { _remark = remark; }

        inline void setChecked(bool checked) { _is_checked = checked; }

        QString title() const {
            return QString("0x%1(%2)").arg(_data, 3, 16, QChar('0')).arg(_remark);
        }
    };
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
    QLabel *_send_label;
    QLabel *_recv_label;
    QScrollArea *_send_panel;
    QFrame *_send_list;
    QVBoxLayout *_send_layout;
    QWidget *_send_spacer;
    QList<QCheckBox *> _send_widget_list;
    QList<Id> _send_list_data;
    QScrollArea *_recv_panel;
    QFrame *_recv_list;
    QVBoxLayout *_recv_layout;
    QWidget *_recv_spacer;
    QList<QCheckBox *> _recv_widget_list;
    QList<Id> _recv_list_data;
    HexInput *_send_id;
    HexInput *_recv_id;
    QPushButton *_add_send;
    QPushButton *_add_recv;

public:
    explicit CanConfig(Message *message = nullptr, QWidget *parent = nullptr);

private:
    void setup();
    void updateId();
    void placeSpacer();
    void removeSpacer();
};


#endif //REFINE_CANCONFIG_HPP
