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
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtGui/QRegExpValidator>
#include "Message.hpp"
#include "HexInput.hpp"
#include "AccInput.hpp"

class Revolve;

class CanConfig : public QDialog, public Message
{
Q_OBJECT
public:
    class Id
    {
    private:
        unsigned int _data;
        QString _remark;
        bool _is_checked;
        QCheckBox *_widget = nullptr;
    public:
        explicit Id(unsigned int data, const QString &remark = QString(), bool is_checked = false) :
                _data(data), _remark(remark), _is_checked(is_checked)
        {}

        inline unsigned int data() const
        { return _data; }

        inline QString dataStr() const
        { return QString("0x%1").arg(_data, 3, 16, QChar('0')); }

        inline QString remark() const
        { return _remark; }

        inline bool isChecked() const
        { return _is_checked; }

        inline void setData(unsigned int data)
        { _data = data; }

        inline void setRemark(const QString &remark)
        { _remark = remark; }

        inline void setChecked(bool checked)
        { _is_checked = checked; }

        QString title() const
        {
            return QString("0x%1 - %2").arg(_data, 3, 16, QChar('0')).arg(_remark);
        }

        inline QCheckBox *&widget()
        { return _widget; }
    };
private:
    Revolve *_revolve;
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
    AccInput *_acc_code;
    AccInput *_acc_mask;
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
    QPushButton *_code;

    QGroupBox *_id;
    QGridLayout *_id_layout;
    QLabel *_send_label;
    QLabel *_recv_label;
    QLabel *_burn_label;
    QScrollArea *_send_panel;
    QFrame *_send_list;
    QVBoxLayout *_send_layout;
    QList<Id> _send_list_data;
    QScrollArea *_recv_panel;
    QFrame *_recv_list;
    QVBoxLayout *_recv_layout;
    QList<Id> _recv_list_data;
    QScrollArea *_burn_panel;
    QFrame *_burn_list;
    QVBoxLayout *_burn_layout;
    QList<Id> _burn_list_data;
    HexInput *_send_id;
    HexInput *_recv_id;
    HexInput *_burn_id;
    QPushButton *_add_send;
    QPushButton *_add_recv;
    QPushButton *_add_burn;

    QMenu *_menu;
    QAction *_menu_show;
    QAction *_menu_open;
    QAction *_menu_init;
    QAction *_menu_start;
    QAction *_menu_connect;
    QAction *_menu_reset;
    QAction *_menu_close;
    QAction *_menu_code;
    QList<QAction *> _menu_simple;

    QAction *_connect_can;

public:
    explicit CanConfig(Revolve *revolve, Message *message = nullptr, QWidget *parent = nullptr);
//    inline QAction *connectCanAction() { return _connect_can; }
    inline QList<QAction *> simpleControlActions() { return _menu_simple; }
    inline QMenu *fullMenu() { return _menu; }

public slots:
    void openCan();
    void initCan();
    void startCan();
    void connectCan();
    void connectCanByMenu();
    void resetCan();
    void closeCan();
    void reportCan();
    void addRecvIdByTransform(unsigned int id);

private:
    void setup();
    void addSendId(unsigned int id, const QString &remark, bool checked);
    void addRecvId(unsigned int id, const QString &remark, bool checked);
    void addBurnId(unsigned int id, const QString &remark, bool checked);

private slots:
    void sendListChanged(int state);
    void recvListChanged(int state);
    void burnListChanged(int state);
    void addSendIdPushed();
    void addRecvIdPushed();
    void addBurnIdPushed();


signals:
    void sendIdChanged(const QStringList &id);
};


#endif //REFINE_CANCONFIG_HPP
