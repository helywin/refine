//
// Created by jiang.wenqiang on 2018/12/27.
//

#include "Informations.hpp"
#include <QtWidgets/QLabel>
#include "Information.hpp"
#include "Refine.hpp"

const QColor Informations::INFO = QColor(0x2e9950);
const QColor Informations::WARNING = QColor(0xfb7a02);
const QColor Informations::CRITICAL = QColor(0xbf180f);
const QColor Informations::FATAL = QColor(0x252525);
const QColor Informations::DEBUG = QColor(0x65728c);

Informations::Informations(QWidget *parent) : 
    QWidget(parent)
{
    setup();
}

void Informations::setup()
{
    setWindowTitle(tr("信息"));
    _messager = new Information(this);
    resize(200, 100);
    _layout = new QVBoxLayout(this);
    setLayout(_layout);
    _layout->setContentsMargins(5, 0, 0, 0);
    _labels = new QWidget(this);
    _layout_labels = new QHBoxLayout(_labels);
    _labels->setLayout(_layout_labels);
    _label_info = new QLabel(_labels);
    _label_warning = new QLabel(_labels);
    _label_critical = new QLabel(_labels);
    _label_fatal = new QLabel(_labels);
    _label_debug = new QLabel(_labels);
    _label_info->setMinimumHeight(25);
    _label_warning->setMinimumHeight(25);
    _label_critical->setMinimumHeight(25);
    _label_fatal->setMinimumHeight(25);
    _label_debug->setMinimumHeight(25);
    _label_info->setAutoFillBackground(true);
    _label_warning->setAutoFillBackground(true);
    _label_critical->setAutoFillBackground(true);
    _label_fatal->setAutoFillBackground(true);
    _label_debug->setAutoFillBackground(true);
    QPalette palette_info = _label_info->palette();
    QPalette palette_warning = _label_warning->palette();
    QPalette palette_critical = _label_critical->palette();
    QPalette palette_fatal = _label_fatal->palette();
    QPalette palette_debug = _label_debug->palette();
    palette_info.setColor(QPalette::ColorRole::Window, OutputBox::INFO);
    palette_warning.setColor(QPalette::ColorRole::Window, OutputBox::WARNING);
    palette_critical.setColor(QPalette::ColorRole::Window, OutputBox::CRITICAL);
    palette_fatal.setColor(QPalette::ColorRole::Window, OutputBox::FATAL);
    palette_debug.setColor(QPalette::ColorRole::Window, OutputBox::DEBUG);
    _label_info->setPalette(palette_info);
    _label_warning->setPalette(palette_warning);
    _label_critical->setPalette(palette_critical);
    _label_fatal->setPalette(palette_fatal);
    _label_debug->setPalette(palette_debug);
    _label_info->setToolTip(tr("信息"));
    _label_warning->setToolTip(tr("警告"));
    _label_critical->setToolTip(tr("严重"));
    _label_fatal->setToolTip(tr("致命"));
    _label_debug->setToolTip(tr("调试"));
    _label_info->update();
    _label_warning->update();
    _label_critical->update();
    _label_fatal->update();
    _label_debug->update();
    _layout_labels->addWidget(_label_info);
    _layout_labels->addWidget(_label_warning);
    _layout_labels->addWidget(_label_critical);
    _layout_labels->addWidget(_label_fatal);
    _layout_labels->addWidget(_label_debug);
    _layout_labels->setContentsMargins(10, 10, 10, 0);
    _layout->addWidget(_labels);
    _layout->addWidget(_messager);
}

void Informations::connectToMessager(Refine *emitter)
{
    connect(emitter, &Refine::message, _messager,
            &Information::showMessage);        //多个连接一个不能直接连接
}

void Informations::connectToMessager(Revolve *emitter)
{
    connect(emitter, &Revolve::message, _messager,
            &Information::showMessage);
}

void Informations::connectToMessager(FilePicker *emitter)
{
    connect(emitter, &FilePicker::message, _messager,
            &Information::showMessage);
}