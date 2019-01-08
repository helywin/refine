//
// Created by jiang.wenqiang on 2018/10/23.
//

#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QStandardPaths>
#include <QtGui/QTextCursor>
#include "ToolBox.hpp"
#include "Revolve.hpp"
#include "MessagePanel.hpp"

ToolBox::ToolBox(Message *message, Revolve *revolve, QWidget *parent) :
        QDockWidget(parent),
        Message(message),
        _revolve(revolve)
{
    setup();
}

void ToolBox::setup()
{
    this->setWindowTitle(tr("工具"));
//    _content = new QWidget(this);
//    _layout = new QVBoxLayout(_content);
//    _content->setLayout(_layout);
    _panel = new QWidget(this);
    setWidget(_panel);
//    _layout->setContentsMargins(5,0,0,0);
    QFont font("微软雅黑", 10);
    _layout = new QVBoxLayout(_panel);
    _layout->setContentsMargins(5, 5, 0, 5);
    _panel->setLayout(_layout);
    _messages = new QTabWidget(_panel);
    _text = new CanMessage(_messages);
    _text->setFont(font);
    _message_panel = new MessagePanel(_messages);
    _messages->addTab(_text, tr("CAN信息"));
    _messages->addTab(_message_panel, "软件信息");
    _command = new Command(_panel);
    _burn_frame = new QGroupBox(tr("烧录"), _panel);
    _burn_layout = new QVBoxLayout(_burn_frame);
    _burn_layout->setContentsMargins(0, 0, 0, 0);
    _burn_layout->setSpacing(0);
    _burn_frame->setLayout(_burn_layout);
    auto speed = new QWidget(_burn_frame);
    auto speed_layout = new QHBoxLayout(speed);
    speed->setLayout(speed_layout);
    _msec = new QComboBox(speed);
    _msec->setFont(font);
    _msec->addItem(tr("间隔 10ms"), 10);
    _msec->addItem(tr("间隔 20ms"), 20);
    _msec->addItem(tr("间隔 30ms"), 30);
    _msec->addItem(tr("间隔 40ms"), 40);
    _msec->addItem(tr("间隔 50ms"), 50);
    _msec->addItem(tr("间隔 100ms"), 100);
    _msec->addItem(tr("间隔 200ms"), 200);
    _msec->addItem(tr("间隔 500ms"), 500);
    _msec->setCurrentIndex(4);
    _frames = new QComboBox(speed);
    _frames->setFont(font);
    _frames->addItem(tr("10帧 每次"), 10);
    _frames->addItem(tr("20帧 每次"), 20);
    _frames->addItem(tr("50帧 每次"), 50);
    _frames->addItem(tr("100帧 每次"), 100);
    _frames->addItem(tr("200帧 每次"), 200);
    _frames->addItem(tr("500帧 每次"), 500);
    _frames->addItem(tr("1000帧 每次"), 1000);
    _frames->setCurrentIndex(3);
    speed_layout->addWidget(_msec);
    speed_layout->addWidget(_frames);
    auto file_view = new QWidget(_burn_frame);
    auto file_layout = new QHBoxLayout(file_view);
    file_view->setLayout(file_layout);

    _file_name = new QComboBox(file_view);
    _file_name->setFont(font);
    _file_name->setEditable(true);
    _browser = new QPushButton(tr("..."), file_view);
    _browser->setFont(font);
    _browser->setFixedWidth(27);
    _burn = new QPushButton(tr("烧录"), file_view);
    _burn->setFont(font);
    file_layout->addWidget(_file_name, 1);
    file_layout->addWidget(_browser);
//    file_layout->addWidget(_burn);

    _burn_layout->addWidget(speed);
    _burn_layout->addWidget(file_view);

    _layout->addWidget(_messages);
    _layout->addWidget(_command);
    _layout->addWidget(_burn_frame);
    _layout->addWidget(_burn);

    connect(_browser, &QPushButton::clicked, this, [=]() {
        QString file = QFileDialog::getOpenFileName(
                this, tr("打开程序烧录文件"),
                QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first(),
                tr("HEX文件(*.hex);;s19烧录文件(*.s19)")
        );
        if (file.isEmpty()) {
            return;
        }
        if (_file_name->findText(file, Qt::MatchFixedString) == -1) { //找出返回的是index
            _file_name->addItem(file);
        }
        _file_name->setCurrentText(file);
    });

    connect(_msec, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            [=](int index) {
                _revolve->setTransmitMsec((unsigned long) _msec->itemData(index).toUInt());
            }
    );

    connect(_frames, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            [=](int index) {
                _revolve->setTransmitFrameNum(_frames->itemData(index).toInt());
            }
    );

    connect(_burn, &QPushButton::clicked, this, [=]() {
        QFileInfo info(_file_name->currentText());
        _revolve->beginBurning(info.absoluteFilePath(),
                               _msec->currentData().toUInt(),
                               _frames->currentData().toInt());
    });

    connect(_command, &Command::commandConfirmed,
            this, &ToolBox::sendCanMessage);
}

void ToolBox::getCanMessage(const QString &msg)
{
    _text->insertMessage(msg, CanMessage::Receive);
}

void ToolBox::sendCanMessage(const QString &msg)
{
    if (_revolve->can().status() & Can::Started) {
        _revolve->sendCommand(msg);
        _text->insertMessage(msg + "\n", CanMessage::Send);
        if (_command_clear) {
            _command->setText(QString());
        }
    }
}

void ToolBox::setCommandPrefix(const QStringList &prefix)
{
    _command->setPrefix(prefix);
}

