//
// Created by jiang.wenqiang on 2019/1/4.
//

#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDebug>
#include "CanConfig.hpp"
#include "CanDefines.hpp"
#include "Revolve.hpp"
#include "Style.hpp"

CanConfig::CanConfig(Revolve *revolve, Message *message, QWidget *parent) :
        QDialog(parent),
        Message(message),
        _revolve(revolve)
{
    setup();
}

void CanConfig::setup()
{
    setMinimumWidth(750);
    setWindowTitle(tr("配置CAN"));
    _layout = new QHBoxLayout(this);
    setLayout(_layout);
    _param = new QGroupBox(tr("CAN参数"), this);
    _layout->addWidget(_param);
    _param->setFont(St::font_title);
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
    _device_type_label->setFont(St::font_regular);
    _device_index_label->setFont(St::font_regular);
    _device_channel_label->setFont(St::font_regular);
    _baudrate_label->setFont(St::font_regular);
    _acc_code_label->setFont(St::font_regular);
    _acc_mask_label->setFont(St::font_regular);
    _filter_label->setFont(St::font_regular);
    _mode_label->setFont(St::font_regular);
    _device_type = new QComboBox(_selection);
    _device_index = new QComboBox(_selection);
    _device_channel = new QComboBox(_selection);
    _baudrate = new QComboBox(_selection);
    _acc_code = new AccInput(_selection);
    _acc_mask = new AccInput(_selection);
    _filter = new QComboBox(_selection);
    _mode = new QComboBox(_selection);
    _device_type->setFont(St::font_regular);
    _device_index->setFont(St::font_regular);
    _device_channel->setFont(St::font_regular);
    _baudrate->setFont(St::font_regular);
    _filter->setFont(St::font_regular);
    _mode->setFont(St::font_regular);
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
//    _device_type.item
    _device_type->setCurrentIndex(3);
    _device_index->addItem(tr("0"), 0);
    _device_index->addItem(tr("1"), 1);
    _device_index->addItem(tr("2"), 2);
    _device_index->addItem(tr("3"), 3);
    _device_index->setCurrentIndex(0);
    _device_channel->addItem(tr("0 (CAN1)"), 0);
    _device_channel->addItem(tr("1 (CAN2)"), 1);
    _device_channel->addItem(tr("2"), 2);
    _device_channel->addItem(tr("3"), 3);
    _device_channel->setCurrentIndex(0);
    _baudrate->addItem(tr("   5 Kbps"), Cd::BR_5Kbps);
    _baudrate->addItem(tr("  10 Kbps"), Cd::BR_10Kbps);
    _baudrate->addItem(tr("  20 Kbps"), Cd::BR_20Kbps);
    _baudrate->addItem(tr("  40 Kbps"), Cd::BR_40Kbps);
    _baudrate->addItem(tr("  50 Kbps"), Cd::BR_50Kbps);
    _baudrate->addItem(tr("  80 Kbps"), Cd::BR_80Kbps);
    _baudrate->addItem(tr(" 100 Kbps"), Cd::BR_100Kbps);
    _baudrate->addItem(tr(" 125 Kbps"), Cd::BR_125Kbps);
    _baudrate->addItem(tr(" 200 Kbps"), Cd::BR_200Kbps);
    _baudrate->addItem(tr(" 250 Kbps (卡车)"), Cd::BR_250Kbps);
    _baudrate->addItem(tr(" 400 Kbps"), Cd::BR_400Kbps);
    _baudrate->addItem(tr(" 500 Kbps (小车)"), Cd::BR_500Kbps);
    _baudrate->addItem(tr(" 666 Kbps"), Cd::BR_666Kbps);
    _baudrate->addItem(tr(" 800 Kbps"), Cd::BR_800Kbps);
    _baudrate->addItem(tr("1000 Kbps"), Cd::BR_1000Kbps);
    _baudrate->setCurrentIndex(11);
    _acc_code->addAcc(0xFFFFFFFF);
    _acc_code->addAcc(0x00000000);
    _acc_code->setCurrentIndex(0);
    _acc_mask->addAcc(0xFFFFFFFF);
    _acc_mask->addAcc(0x00000000);
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
    _code = new QPushButton(tr("错误码"), _control);
    _open->setFont(St::font_regular);
    _init->setFont(St::font_regular);
    _start->setFont(St::font_regular);
    _connect->setFont(St::font_regular);
    _reset->setFont(St::font_regular);
    _close->setFont(St::font_regular);
    _code->setFont(St::font_regular);
    _control_layout->addWidget(_open, 0, 0, Qt::AlignVCenter | Qt::AlignHCenter);
    _control_layout->addWidget(_init, 0, 1, Qt::AlignVCenter | Qt::AlignHCenter);
    _control_layout->addWidget(_start, 0, 2, Qt::AlignVCenter | Qt::AlignHCenter);
    _control_layout->addWidget(_connect, 1, 0, Qt::AlignVCenter | Qt::AlignHCenter);
    _control_layout->addWidget(_reset, 1, 1, Qt::AlignVCenter | Qt::AlignHCenter);
    _control_layout->addWidget(_close, 1, 2, Qt::AlignVCenter | Qt::AlignHCenter);
    _control_layout->addWidget(_code, 2, 0, Qt::AlignVCenter | Qt::AlignHCenter);
    _param_layout->addWidget(new QWidget(_param), 1);

    _id = new QGroupBox(tr("ID选择"), this);
    _id->setFont(St::font_title);
    _layout->addWidget(_id);
    _id_layout = new QGridLayout(_id);
    _id->setLayout(_id_layout);

    _send_id = new HexInput(3, _id);
    _recv_id = new HexInput(3, _id);
    _burn_id = new HexInput(3, _id);
    _add_send = new QPushButton(tr("添加发送ID"), this);
    _add_recv = new QPushButton(tr("添加接收ID"), this);
    _add_burn = new QPushButton(tr("添加烧录ID"), this);
    _send_panel = new QScrollArea(_id);
    _recv_panel = new QScrollArea(_id);
    _burn_panel = new QScrollArea(_id);
    _send_list = new QFrame(_send_panel);
    _recv_list = new QFrame(_recv_panel);
    _burn_list = new QFrame(_burn_panel);
    _send_list->setContentsMargins(0, 0, 0, 0);
    _recv_list->setContentsMargins(0, 0, 0, 0);
    _burn_list->setContentsMargins(0, 0, 0, 0);
    _send_panel->setWidget(_send_list);
    _recv_panel->setWidget(_recv_list);
    _burn_panel->setWidget(_burn_list);
    _send_layout = new QVBoxLayout(_send_list);
    _recv_layout = new QVBoxLayout(_recv_list);
    _burn_layout = new QVBoxLayout(_burn_list);
    _send_layout->setSpacing(0);
    _recv_layout->setSpacing(0);
    _burn_layout->setSpacing(0);
    _send_layout->setContentsMargins(5, 0, 5, 0);
    _recv_layout->setContentsMargins(5, 0, 5, 0);
    _burn_layout->setContentsMargins(5, 0, 5, 0);
    _send_list->setLayout(_send_layout);
    _recv_list->setLayout(_recv_layout);
    _burn_list->setLayout(_burn_layout);
    _send_list->setFont(St::font_regular);
    _recv_list->setFont(St::font_regular);
    _burn_list->setFont(St::font_regular);
    _send_list->setFixedWidth(150);
    _recv_list->setFixedWidth(150);
    _burn_list->setFixedWidth(150);

    _send_id->setFont(St::font_regular);
    _recv_id->setFont(St::font_regular);
    _burn_id->setFont(St::font_regular);
    _add_send->setFont(St::font_regular);
    _add_recv->setFont(St::font_regular);
    _add_burn->setFont(St::font_regular);
    _send_label = new QLabel(tr("发送ID"), _id);
    _recv_label = new QLabel(tr("接收ID"), _id);
    _burn_label = new QLabel(tr("烧录ID"), _id);
    _id_layout->addWidget(_send_label, 0, 0, Qt::AlignHCenter);
    _id_layout->addWidget(_recv_label, 0, 1, Qt::AlignHCenter);
    _id_layout->addWidget(_burn_label, 0, 2, Qt::AlignHCenter);
    _id_layout->addWidget(_send_panel, 1, 0);
    _id_layout->addWidget(_recv_panel, 1, 1);
    _id_layout->addWidget(_burn_panel, 1, 2);
    _id_layout->addWidget(_send_id, 2, 0);//, Qt::AlignVCenter | Qt::AlignHCenter);
    _id_layout->addWidget(_recv_id, 2, 1);//, Qt::AlignVCenter | Qt::AlignHCenter);
    _id_layout->addWidget(_burn_id, 2, 2);//, Qt::AlignVCenter | Qt::AlignHCenter);
    _id_layout->addWidget(_add_send, 3, 0, Qt::AlignHCenter);
    _id_layout->addWidget(_add_recv, 3, 1, Qt::AlignHCenter);
    _id_layout->addWidget(_add_burn, 3, 2, Qt::AlignHCenter);
    _id_layout->setRowStretch(0, 0);
    _id_layout->setRowStretch(1, 1);
    _id_layout->setRowStretch(2, 0);
    _id_layout->setRowStretch(3, 0);

    addSendId(0x611, tr("小车引导"), true);
    addSendId(0x333, tr("卡车引导"), false);
    addRecvId(0x613, tr("小车引导"), true);
    addRecvId(0x335, tr("卡车引导"), true);
    addBurnId(0x611, tr("小车引导"), true);
    addBurnId(0x333, tr("卡车引导"), false);

    _connect_can = new QAction(tr("连接CAN(&C)"), this);
    _connect_can->setCheckable(true);
    _connect_can->setStatusTip(tr("连接/断开CAN"));

    _menu = new QMenu(tr("CAN"), this);
    _menu_show = new QAction(tr("配置CAN...(&C)"), _menu);
    _menu_open = new QAction(tr("打开(&O)"), _menu);
    _menu_init = new QAction(tr("初始化(&I)"), _menu);
    _menu_start = new QAction(tr("启动(&S)"), _menu);
    _menu_connect = new QAction(tr("快速启动(&N)"), _menu);
    _menu_reset = new QAction(tr("复位(&R)"), _menu);
    _menu_close = new QAction(tr("关闭(&D)"), _menu);
    _menu_code = new QAction(tr("错误码(&E)"), _menu);
    _menu_show->setIcon(QIcon(":res/icons/config.ico"));
    _menu_connect->setIcon(QIcon(":res/icons/connect.ico"));
    _menu_close->setIcon(QIcon(":res/icons/disconnect.ico"));

    _menu_show->setStatusTip(tr("配置CAN的连接属性和报文收发烧录ID"));
    _menu_open->setStatusTip(tr("一键连接CAN"));
    _menu_init->setStatusTip(tr("初始化CAN"));
    _menu_start->setStatusTip(tr("启动CAN并启动采集"));
    _menu_connect->setStatusTip(tr("一键启动CAN并启动采集"));
    _menu_reset->setStatusTip(tr("复位CAN回到初始化后状态"));
    _menu_close->setStatusTip(tr("关闭CAN并结束采集"));
    _menu_code->setStatusTip(tr("获取CAN的错误代码并输出"));

    _menu->addAction(_menu_show);
    _menu->addAction(_menu_open);
    _menu->addAction(_menu_init);
    _menu->addAction(_menu_start);
    _menu->addAction(_menu_connect);
    _menu->addAction(_menu_reset);
    _menu->addAction(_menu_close);
    _menu->addAction(_menu_code);
    _menu_simple.append(_menu_show);
    _menu_simple.append(_menu_connect);
    _menu_simple.append(_menu_close);

    connect(_add_send, &QPushButton::clicked,
            this, &CanConfig::addSendIdPushed);
    connect(_add_recv, &QPushButton::clicked,
            this, &CanConfig::addRecvIdPushed);
    connect(_add_burn, &QPushButton::clicked,
            this, &CanConfig::addBurnIdPushed);
    connect(_open, &QPushButton::clicked, this, &CanConfig::openCan);
    connect(_init, &QPushButton::clicked, this, &CanConfig::initCan);
    connect(_start, &QPushButton::clicked, this, &CanConfig::startCan);
    connect(_connect, &QPushButton::clicked, this, &CanConfig::connectCan);
    connect(_reset, &QPushButton::clicked, this, &CanConfig::resetCan);
    connect(_close, &QPushButton::clicked, this, &CanConfig::closeCan);
    connect(_code, &QPushButton::clicked, this, &CanConfig::reportCan);
    connect(_connect_can, &QAction::triggered, this, &CanConfig::connectCanByMenu);
    connect(_revolve, &Revolve::getNewRecvId, this, &CanConfig::addRecvIdByTransform);

    connect(_menu_show, &QAction::triggered, this, [this]() { show(); });
    connect(_menu_open, &QAction::triggered, this, &CanConfig::openCan);
    connect(_menu_init, &QAction::triggered, this, &CanConfig::initCan);
    connect(_menu_start, &QAction::triggered, this, &CanConfig::startCan);
    connect(_menu_connect, &QAction::triggered, this, &CanConfig::connectCan);
    connect(_menu_reset, &QAction::triggered, this, &CanConfig::resetCan);
    connect(_menu_close, &QAction::triggered, this, &CanConfig::closeCan);
    connect(_menu_code, &QAction::triggered, this, &CanConfig::reportCan);

    connect(_device_type, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                Q_UNUSED(index);
                _revolve->can().configLatest().setDeviceType(
                        (unsigned long) _device_type->currentData().toUInt());
                _revolve->can().setChanged(true);
            });

    connect(_device_index, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                Q_UNUSED(index);
                _revolve->can().configLatest().setDeviceIndex(
                        (unsigned long) _device_index->currentData().toUInt());
                _revolve->can().setChanged(true);
            });

    connect(_device_channel, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                Q_UNUSED(index);
//                qDebug() << "选择第几路CAN: " << _device_channel->currentData().toUInt();
                _revolve->can().configLatest().setDeviceChannel(
                        (unsigned long) _device_channel->currentData().toUInt());
                _revolve->can().setChanged(true);
            });

    connect(_baudrate, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                Q_UNUSED(index);
                _revolve->can().configLatest().setBaudRate(
                        (Cd::BaudRate) _baudrate->currentData().toInt());
                _revolve->can().setChanged(true);
            });

    connect(_filter, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                Q_UNUSED(index);
                _revolve->can().configLatest().setFilter(
                        (unsigned char) _filter->currentData().toChar().toLatin1());
                _revolve->can().setChanged(true);
            });
    connect(_mode, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                Q_UNUSED(index);
                _revolve->can().configLatest().setMode(
                        (unsigned char) _mode->currentData().toChar().toLatin1());
                _revolve->can().setChanged(true);
            });

    connect(_acc_code, &AccInput::valueChanged, this, [this](unsigned int v) {
        _revolve->can().configLatest().setAccCode((unsigned long) v);
        _revolve->can().setChanged(true);
    });

    connect(_acc_mask, &AccInput::valueChanged, this, [this](unsigned int v) {
        _revolve->can().configLatest().setAccMask((unsigned long) v);
        _revolve->can().setChanged(true);
    });
}

void CanConfig::sendListChanged(int state)
{
    Q_UNUSED(state);
    QStringList prefixs;
    QVector<unsigned int> ids;
    int checked_data = 0;
    int checked_box = 0;
    Id *p = nullptr;
    for (auto &iter : _send_list_data) {
        checked_data += iter.isChecked();
        checked_box += (bool)iter.widget()->checkState();
        if (iter.isChecked()) {
            p = &iter;
        }
    }
    if (checked_data == 1 && checked_box == 0 && p) {
//        QMessageBox::information(this, tr("提示"), tr("至少留下一个发送ID"), tr("确定"));
        p->widget()->setCheckState(Qt::Checked);
    }
    for (auto &iter : _send_list_data) {
        iter.setChecked(iter.widget()->checkState());
        if (iter.widget()->checkState()) {
            prefixs.append(QString("%1").arg(iter.data(), 3, 16, QChar('0')));
            ids.append(iter.data());
        }
    }
    _revolve->setSendId(ids);
    emit sendIdChanged(prefixs);
}

void CanConfig::recvListChanged(int state)
{
    Q_UNUSED(state);
    QMap<unsigned int, bool> ids;
    for (auto &iter : _recv_list_data) {
        if (iter.data() == 0x777
            && (iter.isChecked() != (bool) iter.widget()->checkState())) {
            int flag = QMessageBox::warning(this, tr("警告"),
                                            tr("接收显示0x777的报文会在打开调试报文后"
                                               "\n由于海量报文直接卡死，确定接收吗？"),
                                            tr("无所畏惧"), tr("放弃"), QString(), 1);
            if (flag == 1) {
//                emitMessage(Re::Debug, "no");
                iter.widget()->setCheckState(Qt::Unchecked);
            }
        }
        iter.setChecked(iter.widget()->checkState());
        ids[iter.data()] = iter.isChecked();
    }
//    qDebug() << "CanConfig::recvListChanged: " << ids.keys() << ids.values();
    _revolve->setRecvId(ids);
}

void CanConfig::addSendId(unsigned int id, const QString &remark, bool checked)
{
    _send_list_data.append(Id(id, remark, checked));
    Id &e = _send_list_data.last();
    e.widget() = new QCheckBox(e.title(), _send_list);
    connect(e.widget(), &QCheckBox::stateChanged,
            this, &CanConfig::sendListChanged);
    _send_list->setFixedHeight(e.widget()->height() * _send_list_data.size());
    _send_layout->insertWidget(_send_list_data.size(), e.widget(), Qt::AlignVCenter);
    e.setChecked(true);
    if (checked) {
        e.setChecked(true);
        e.widget()->setCheckState(Qt::Checked);
    } else {
        e.setChecked(false);
        e.widget()->setCheckState(Qt::Unchecked);
    }
}

void CanConfig::addRecvId(unsigned int id, const QString &remark, bool checked)
{
    _recv_list_data.append(Id(id, remark, checked));
    Id &e = _recv_list_data.last();
    e.widget() = new QCheckBox(e.title(), _recv_list);
    connect(e.widget(), &QCheckBox::stateChanged,
            this, &CanConfig::recvListChanged);
    _recv_list->setFixedHeight(e.widget()->height() * _recv_list_data.size());
    _recv_layout->insertWidget(_recv_list_data.size(), e.widget(), Qt::AlignVCenter);
    if (checked) {
        e.setChecked(true);
        e.widget()->setCheckState(Qt::Checked);
    } else {
        e.setChecked(false);
        e.widget()->setCheckState(Qt::Unchecked);
    }
}

void CanConfig::addRecvIdByTransform(unsigned int id)
{
    addRecvId(id, tr("自动添加"), false);
}

void CanConfig::addSendIdPushed()
{
    if (_send_id->text().isEmpty()) {
        return;
    }
    unsigned int id = _send_id->text().toUInt(nullptr, 16);
    for (const auto &iter : _send_list_data) {
        if (iter.data() == id) {
            return;
        }
    }
    addSendId(id, tr("用户添加"), false);
}

void CanConfig::addRecvIdPushed()
{
    if (_recv_id->text().isEmpty()) {
        return;
    }
    unsigned int id = _recv_id->text().toUInt(nullptr, 16);
    for (const auto &iter : _recv_list_data) {
        if (iter.data() == id) {
            return;
        }
    }
    addRecvId(id, tr("用户添加"), true);
}

void CanConfig::addBurnId(unsigned int id, const QString &remark, bool checked)
{
    _burn_list_data.append(Id(id, remark, checked));
    Id &e = _burn_list_data.last();
    e.widget() = new QCheckBox(e.title(), _burn_list);
    connect(e.widget(), &QCheckBox::stateChanged,
            this, &CanConfig::burnListChanged);
//    qDebug() << "CanConfig::addSendId before height" << e.widget()->height();
    _burn_list->setFixedHeight(e.widget()->height() * _burn_list_data.size());
    _burn_layout->insertWidget(_burn_list_data.size(), e.widget(), Qt::AlignVCenter);
//    qDebug() << "CanConfig::addSendId after height" << e.widget()->height();
    if (checked) {
        for (int i = 0; i < _burn_list_data.size() - 1; ++i) {
            _burn_list_data[i].setChecked(false);
            _burn_list_data[i].widget()->setCheckState(Qt::Unchecked);
        }
        e.setChecked(true);
        e.widget()->setCheckState(Qt::Checked);
    }
//    qDebug() << "CanConfig::addSendId rect height" << _send_list->rect().height();
}

void CanConfig::burnListChanged(int state)
{
//    qDebug() << "CanConfig::sendListChanged";
    Q_UNUSED(state);
    int index = -1;
    for (int i = 0; i < _burn_list_data.size(); ++i) {
        if (_burn_list_data[i].isChecked() != _burn_list_data[i].widget()->isChecked()) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return;
    }
    if (_burn_list_data[index].isChecked()) {
        _burn_list_data[index].widget()->setCheckState(Qt::Checked);
    } else {
        for (int i = 0; i < _burn_list_data.size(); ++i) {
            if (i == index) {
                _burn_list_data[index].setChecked(true);
                _burn_list_data[index].widget()->setCheckState(Qt::Checked);
            } else {
                _burn_list_data[i].setChecked(false);
                _burn_list_data[i].widget()->setCheckState(Qt::Unchecked);
            }
        }
        _revolve->setBurnId(_burn_list_data[index].data());
    }
}

void CanConfig::addBurnIdPushed()
{
    if (_burn_id->text().isEmpty()) {
        return;
    }
    unsigned int id = _burn_id->text().toUInt(nullptr, 16);
    for (const auto &iter : _burn_list_data) {
        if (iter.data() == id) {
            return;
        }
    }
    addBurnId(id, tr("用户添加"), false);
}

void CanConfig::openCan()
{
    if (_revolve->can().open()) {
        emitMessage(Re::Info, tr("CAN打开成功"));
    } else {
        emitMessage(Re::Warning, tr("CAN已经打开或连接出现问题"));
    }
}

void CanConfig::initCan()
{
    if (_revolve->can().init()) {
        emitMessage(Re::Info, tr("CAN初始化成功"));
    } else {
        emitMessage(Re::Warning, tr("CAN已经初始化或连接出现问题"));
    }
}

void CanConfig::startCan()
{
    if (_revolve->startCan()) {
        emitMessage(Re::Info, tr("CAN启动成功"));
        _connect_can->setChecked(true);
    } else {
        emitMessage(Re::Warning, tr("CAN已经启动或连接出现问题"));
    }
}

void CanConfig::connectCan()
{
    if (_revolve->connectCan()) {
        emitMessage(Re::Info, tr("CAN连接成功"));
        _connect_can->setChecked(true);
    } else {
        emitMessage(Re::Warning, tr("CAN已经连接或连接出现问题"));
    }
}

void CanConfig::resetCan()
{
    if (_revolve->resetCan()) {
        emitMessage(Re::Info, tr("CAN复位成功"));
        _connect_can->setChecked(false);
    } else {
        emitMessage(Re::Warning, tr("CAN连接出现问题"));
    }
}

void CanConfig::closeCan()
{
    if (_revolve->disConnectCan()) {
        emitMessage(Re::Info, tr("CAN关闭成功"));
        _connect_can->setChecked(false);
    } else {
        emitMessage(Re::Warning, tr("CAN连接出现问题"));
    }
}

void CanConfig::reportCan()
{
    _revolve->can().reportError();
}

void CanConfig::connectCanByMenu()
{
    if (_connect_can->isChecked()) {
        if (_revolve->connectCan()) {
            emitMessage(Re::Info, tr("连接成功"));
        } else {
            emitMessage(Re::Warning | Re::Popout,
                        tr("连接失败，检查CAN占用或连接情况"));
        }
    } else {
        if (_revolve->disConnectCan()) {
            emitMessage(Re::Info, tr("断开成功"));
        } else {
            emitMessage(Re::Warning | Re::Popout,
                        tr("断开失败，检查CAN占用或连接情况"));
        }
    }
    _connect_can->setChecked(_revolve->can().isConnected());
}
