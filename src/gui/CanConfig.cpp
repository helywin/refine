//
// Created by jiang.wenqiang on 2019/1/4.
//

#include <QtWidgets/QHeaderView>
#include <QtCore/QDebug>
#include "CanConfig.hpp"
#include "CanDefines.hpp"

CanConfig::CanConfig(Message *message, QWidget *parent) :
        QDialog(parent),
        Message(message)
{
    setup();
}

void CanConfig::setup()
{
    QFont font("微软雅黑", 9);
    QFont font_combo("微软雅黑", 10);
    setMinimumWidth(750);
    setWindowTitle(tr("配置CAN"));
    _layout = new QHBoxLayout(this);
    setLayout(_layout);
    _param = new QGroupBox(tr("CAN参数"), this);
    _layout->addWidget(_param);
    _param->setFont(font);
    _param_layout = new QVBoxLayout(_param);
    _param->setLayout(_param_layout);
    _selection = new QWidget(_param);
    _param_layout->addWidget(_selection);
    _selection_layout = new QFormLayout(_selection);
    _selection->setLayout(_selection_layout);
    _device_type_label = new QLabel(tr("设备类型："), _selection);
    _device_index_label = new QLabel(tr("索引号："), _selection);
    _device_channel_label = new QLabel(tr("第几路："), _selection);
    _baudrate_label = new QLabel(tr("波特率："), _selection);
    _acc_code_label = new QLabel(tr("接收码："), _selection);
    _acc_mask_label = new QLabel(tr("屏蔽码："), _selection);
    _filter_label = new QLabel(tr("滤波方式："), _selection);
    _mode_label = new QLabel(tr("工作模式："), _selection);
    _device_type_label->setFont(font_combo);
    _device_index_label->setFont(font_combo);
    _device_channel_label->setFont(font_combo);
    _baudrate_label->setFont(font_combo);
    _acc_code_label->setFont(font_combo);
    _acc_mask_label->setFont(font_combo);
    _filter_label->setFont(font_combo);
    _mode_label->setFont(font_combo);
    _device_type = new QComboBox(_selection);
    _device_index = new QComboBox(_selection);
    _device_channel = new QComboBox(_selection);
    _baudrate = new QComboBox(_selection);
    _acc_code = new QComboBox(_selection);
    _acc_mask = new QComboBox(_selection);
    _filter = new QComboBox(_selection);
    _mode = new QComboBox(_selection);
    _device_type->setFont(font_combo);
    _device_index->setFont(font_combo);
    _device_channel->setFont(font_combo);
    _baudrate->setFont(font_combo);
    _acc_code->setFont(font_combo);
    _acc_mask->setFont(font_combo);
    _filter->setFont(font_combo);
    _mode->setFont(font_combo);
    _selection_layout->addRow(_device_type_label, _device_type);
    _selection_layout->addRow(_device_index_label, _device_index);
    _selection_layout->addRow(_device_channel_label, _device_channel);
    _selection_layout->addRow(_baudrate_label, _baudrate);
    _selection_layout->addRow(_acc_code_label, _acc_code);
    _selection_layout->addRow(_acc_mask_label, _acc_mask);
    _selection_layout->addRow(_filter_label, _filter);
    _selection_layout->addRow(_mode_label, _mode);
    _device_type->addItem(tr("PCI5121"), Cd::PCI5121);
    _device_type->addItem(tr("PCI9810"), Cd::PCI9810);
    _device_type->addItem(tr("USBCAN1"), Cd::USBCAN1);
    _device_type->addItem(tr("USBCAN2"), Cd::USBCAN2);
    _device_type->addItem(tr("USBCAN2A"), Cd::USBCAN2A);
    _device_type->addItem(tr("PCI9820"), Cd::PCI9820);
    _device_type->addItem(tr("CAN232"), Cd::CAN232);
    _device_type->addItem(tr("PCI5110"), Cd::PCI5110);
    _device_type->setCurrentIndex(3);
    _device_index->addItem(tr("0"), 0);
    _device_index->addItem(tr("1"), 1);
    _device_index->addItem(tr("2"), 2);
    _device_index->addItem(tr("3"), 3);
    _device_index->setCurrentIndex(0);
    _device_channel->addItem(tr("1"), 1);
    _device_channel->addItem(tr("2"), 2);
    _device_channel->addItem(tr("3"), 3);
    _device_channel->addItem(tr("4"), 4);
    _device_channel->setCurrentIndex(0);
    _baudrate->addItem(tr("5Kbps"), Cd::BR_5Kbps);
    _baudrate->addItem(tr("10Kbps"), Cd::BR_10Kbps);
    _baudrate->addItem(tr("20Kbps"), Cd::BR_20Kbps);
    _baudrate->addItem(tr("40Kbps"), Cd::BR_40Kbps);
    _baudrate->addItem(tr("50Kbps"), Cd::BR_50Kbps);
    _baudrate->addItem(tr("80Kbps"), Cd::BR_80Kbps);
    _baudrate->addItem(tr("100Kbps"), Cd::BR_100Kbps);
    _baudrate->addItem(tr("125Kbps"), Cd::BR_125Kbps);
    _baudrate->addItem(tr("200Kbps"), Cd::BR_200Kbps);
    _baudrate->addItem(tr("250Kbps"), Cd::BR_250Kbps);
    _baudrate->addItem(tr("400Kbps"), Cd::BR_400Kbps);
    _baudrate->addItem(tr("500Kbps"), Cd::BR_500Kbps);
    _baudrate->addItem(tr("666Kbps"), Cd::BR_666Kbps);
    _baudrate->addItem(tr("800Kbps"), Cd::BR_800Kbps);
    _baudrate->addItem(tr("1000Kbps"), Cd::BR_1000Kbps);
    _baudrate->setCurrentIndex(11);
    _acc_code->addItem(tr("0xFFFFFFFF"), 0xFFFFFFFF);
    _acc_code->addItem(tr("0x00000000"), 0x00000000);
    _acc_code->setCurrentIndex(0);
    _acc_mask->addItem(tr("0xFFFFFFFF"), 0xFFFFFFFF);
    _acc_mask->addItem(tr("0x00000000"), 0x00000000);
    _acc_mask->setCurrentIndex(1);
    _filter->addItem(tr("单滤波"), Cd::Single);
    _filter->addItem(tr("双滤波"), Cd::Double);
    _filter->setCurrentIndex(1);
    _mode->addItem(tr("正常模式"), Cd::Normal);
    _mode->addItem(tr("只听模式"), Cd::ListenOnly);
    _mode->setCurrentIndex(0);
    _control = new QWidget(_param);
    _param_layout->addWidget(_control);
    _control_layout = new QGridLayout(_control);
    _control->setLayout(_control_layout);
    _open = new QPushButton(tr("打开设备"), _control);
    _init = new QPushButton(tr("初始化"), _control);
    _start = new QPushButton(tr("启动设备"), _control);
    _connect = new QPushButton(tr("快速启动"), _control);
    _reset = new QPushButton(tr("复位"), _control);
    _close = new QPushButton(tr("关闭"), _control);
    _open->setFont(font_combo);
    _init->setFont(font_combo);
    _start->setFont(font_combo);
    _connect->setFont(font_combo);
    _reset->setFont(font_combo);
    _close->setFont(font_combo);
    _control_layout->addWidget(_open, 0, 0, Qt::AlignVCenter | Qt::AlignHCenter);
    _control_layout->addWidget(_init, 0, 1, Qt::AlignVCenter | Qt::AlignHCenter);
    _control_layout->addWidget(_start, 0, 2, Qt::AlignVCenter | Qt::AlignHCenter);
    _control_layout->addWidget(_connect, 1, 0, Qt::AlignVCenter | Qt::AlignHCenter);
    _control_layout->addWidget(_reset, 1, 1, Qt::AlignVCenter | Qt::AlignHCenter);
    _control_layout->addWidget(_close, 1, 2, Qt::AlignVCenter | Qt::AlignHCenter);
    _param_layout->addWidget(new QWidget(_param), 1);

    _id = new QGroupBox(tr("ID选择"), this);
    _id->setFont(font);
    _layout->addWidget(_id);
    _id_layout = new QGridLayout(_id);
    _id->setLayout(_id_layout);

    _send_id = new HexInput(3, _id);
    _recv_id = new HexInput(3, _id);
    _add_send = new QPushButton(tr("添加发送ID"), this);
    _add_recv = new QPushButton(tr("添加接收ID"), this);
    _send_panel = new QScrollArea(_id);
    _recv_panel = new QScrollArea(_id);
    _send_list = new QFrame(_send_panel);
    _recv_list = new QFrame(_recv_panel);
    _send_list->setContentsMargins(0,0,0,0);
    _recv_list->setContentsMargins(0,0,0,0);
    _send_panel->setWidget(_send_list);
    _recv_panel->setWidget(_recv_list);
    _send_layout = new QVBoxLayout(_send_list);
    _recv_layout = new QVBoxLayout(_recv_list);
    _send_layout->setSpacing(0);
    _recv_layout->setSpacing(0);
    _send_layout->setContentsMargins(5,5,5,5);
    _recv_layout->setContentsMargins(5,5,5,5);
    _send_list->setLayout(_send_layout);
    _recv_list->setLayout(_recv_layout);
    _send_list->setFont(font_combo);
    _recv_list->setFont(font_combo);
    _send_list_data.append(Id(0x611, tr("引导程序"), true));
    _send_list_data.append(Id(0x333, tr("引导程序"), false));
    _send_list_data.append(Id(0x333, tr("引导程序"), false));
    _send_list_data.append(Id(0x333, tr("引导程序"), false));
    _send_list_data.append(Id(0x333, tr("引导程序"), false));
    _send_list_data.append(Id(0x333, tr("引导程序"), false));
    _send_list_data.append(Id(0x333, tr("引导程序"), false));
    _send_list->setFixedSize(180, 400);
    _recv_list->setFixedSize(180, 400);

    _send_spacer = new QWidget(_send_list);
    _recv_spacer = new QWidget(_recv_list);
    placeSpacer();
    _send_id->setFont(font_combo);
    _recv_id->setFont(font_combo);
    _add_send->setFont(font_combo);
    _add_recv->setFont(font_combo);
    _send_label = new QLabel(tr("发送ID"), _id);
    _recv_label = new QLabel(tr("接收ID"), _id);
    _id_layout->addWidget(_send_label, 0, 0, Qt::AlignHCenter);
    _id_layout->addWidget(_recv_label, 0, 1, Qt::AlignHCenter);
    _id_layout->addWidget(_send_panel, 1, 0);
    _id_layout->addWidget(_recv_panel, 1, 1);
    _id_layout->addWidget(_send_id, 2, 0);//, Qt::AlignVCenter | Qt::AlignHCenter);
    _id_layout->addWidget(_recv_id, 2, 1);//, Qt::AlignVCenter | Qt::AlignHCenter);
    _id_layout->addWidget(_add_send, 3, 0, Qt::AlignHCenter);
    _id_layout->addWidget(_add_recv, 3, 1, Qt::AlignHCenter);
    _id_layout->setRowStretch(0, 0);
    _id_layout->setRowStretch(1, 1);
    _id_layout->setRowStretch(2, 0);
    _id_layout->setRowStretch(3, 0);
//    _id_layout->setHorizontalSpacing(5);
    updateId();

}

void CanConfig::updateId()
{
    removeSpacer();
    for (auto p : _send_widget_list) {
        delete p;
    }
    _send_widget_list.clear();
    int h = 0;
    for (const auto &iter : _send_list_data) {
        auto p = new QCheckBox(iter.title(), _send_list);
        p->setChecked(iter.isChecked());
        _send_layout->addWidget(p, 0);
        _send_widget_list.append(p);
        h = p->height();
        qDebug() << "CanConfig::updateId()";
    }
    _send_list->setFixedHeight(10 + h * _send_list_data.size());
    placeSpacer();
//    _send_layout.widget
}

void CanConfig::placeSpacer()
{
    _send_layout->addWidget(_send_spacer, 1, Qt::AlignVCenter);
    _recv_layout->addWidget(_recv_spacer, 1, Qt::AlignVCenter);
}

void CanConfig::removeSpacer()
{
    _send_layout->removeWidget(_send_spacer);
    _recv_layout->removeWidget(_recv_spacer);
}
/*
_send_id_list = new QTableWidget(_id);
_recv_id_list = new QTableWidget(_id);
_send_id_list->setMinimumSize(150, 300);
_recv_id_list->setMinimumSize(150, 300);
_send_id_list->setColumnCount(2);
_recv_id_list->setColumnCount(2);
_send_id_list->setHorizontalHeaderLabels({tr("发送ID"), tr("备注")});
_recv_id_list->setHorizontalHeaderLabels({tr("接收ID"), tr("备注")});
_send_id_list->setColumnWidth(0, 60);
_send_id_list->setColumnWidth(1, 85);
_recv_id_list->setColumnWidth(0, 60);
_recv_id_list->setColumnWidth(1, 85);
_send_id_list->resize(150, 300);
_recv_id_list->resize(150, 300);
_send_id_list->setFont(font_combo);
_recv_id_list->setFont(font_combo);
_send_id_list->verticalHeader()->hide();
_recv_id_list->verticalHeader()->hide();
_send_id_list->setSelectionMode(QAbstractItemView::SingleSelection);
_recv_id_list->setSelectionMode(QAbstractItemView::SingleSelection);
_send_id_list->setSelectionBehavior(QAbstractItemView::SelectRows);
_recv_id_list->setSelectionBehavior(QAbstractItemView::SelectRows);
_send_id_list->setRowCount(2);
_send_id_list->setItem(0, 0, new QTableWidgetItem("0x611"));
_send_id_list->setItem(0, 1, new QTableWidgetItem("引导程序"));
_id_layout->addWidget(_send_id_list, 0, 0);//, Qt::AlignVCenter | Qt::AlignHCenter);
_id_layout->addWidget(_recv_id_list, 0, 1);//, Qt::AlignVCenter | Qt::AlignHCenter);
*/
