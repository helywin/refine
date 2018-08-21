//
// Created by jiang.wenqiang on 2018/7/25.
//

#include "TableTips.h"
#include <QtWidgets/QPushButton>
#include <QtCore/QDebug>

TableTips::TableTips(QWidget *parent) :
        QDialog(parent, Qt::ToolTip) {
    this->setupUi();
    installEventFilter(this);
}

void TableTips::setupUi() {
    layout = new QVBoxLayout(this);
    layout->setMargin(10);
    main_widget = new QWidget(this);
    button_widget = new QWidget(this);
    layout->addWidget(main_widget);
    layout->addWidget(button_widget);
    main_layout = new QVBoxLayout(main_widget);
    main_layout->setContentsMargins(0, 0, 0, 5);
    button_layout = new QVBoxLayout(button_widget);
    button_layout->setContentsMargins(0, 0, 0, 0);
    main_widget->setLayout(main_layout);
    button_widget->setLayout(button_layout);

    ok_button = new QPushButton(button_widget);
    ok_button->setText("确定");
    button_layout->addWidget(ok_button);
    connect(ok_button, &QPushButton::clicked, this, &TableTips::setVal);
}


TipType::TipType(QWidget *parent) : TableTips(parent) {
    this->setupUi();
}

void TipType::setupUi() {
    type = new QGroupBox(QString("类型"), main_widget);
    physical = new QRadioButton(QString("物理"), type);
    physical->setChecked(true);
    logical = new QRadioButton(QString("逻辑"), type);
    logical->setChecked(false);
    radio_layout = new QVBoxLayout(type);
    type->setLayout(radio_layout);
    radio_layout->addWidget(physical);
    radio_layout->addWidget(logical);
    main_layout->addWidget(type);
//    this->setFixedSize(120,140);
//    this->resize(120,140);
}

bool TableTips::eventFilter(QObject *object, QEvent *event) {
    if (object == this) {
        if (event->type() == QEvent::WindowDeactivate) {
            hide();
            return true;
        }
    }
    return false;
}

void TipType::setVal() {
    qDebug("clicked");
    if (physical->isChecked()) {
        emit val(QString("物理"));
    } else if (logical->isChecked()) {
        emit val(QString("逻辑"));
    } else {
        emit val(QString("未知"));
    }
    this->hide();
}

void TipType::regen(QPoint pos) {
    show();
    move(pos.x(), pos.y());
    activateWindow();
}


GetVal::GetVal(QWidget *parent) :
        row(-1), column(-1), type(new TipType(parent)),
        zero(new TipZero(parent)), sample(new TipSample(parent)) {
    connect(type, &TipType::val, this, &GetVal::gotVal);
    connect(zero, &TipZero::val, this, &GetVal::gotVal);
    connect(sample, &TipSample::val, this, &GetVal::gotVal);
}

void GetVal::get(GetVal::Type t, int row, int column, QPoint pos) {
    switch (t) {
        case GetVal::Type::Type :
            type->regen(pos);
            break;
        case GetVal::Type::ZeroByte :
            zero->regen(pos);
            break;
        case GetVal::Type::Sample :
            sample->regen(pos);
            break;
        default:
            break;
    }
    this->row = row;
    this->column = column;
}

void GetVal::gotVal(QString s) {
    emit getVal(s, row, column);
}

TipZero::TipZero(QWidget *parent) : TableTips(parent) {
    setupUi();
}

void TipZero::regen(QPoint pos) {
    show();
    move(pos.x(), pos.y());
    activateWindow();
}

void TipZero::setupUi() {
    zero = new QGroupBox(this);
    zero->setTitle("零字节");
    zero->setCheckable(true);
    zero->setChecked(true);
    group_layout = new QVBoxLayout(zero);
    line = new QSpinBox(zero);
    line->setMinimum(0);
    line->setMaximum(7);
    line->setValue(0);
    main_layout->addWidget(zero);
    zero->setLayout(group_layout);
    group_layout->addWidget(line);
}

void TipZero::setVal() {
    if (zero->isChecked()) {
        emit val(QString("有;%1").arg(line->value()));
    } else {
        emit val(QString("无;"));
    }
    this->hide();
}

TipSample::TipSample(QWidget *parent) : TableTips(parent) {
    setupUi();
}

void TipSample::regen(QPoint pos) {
    show();
    move(pos.x(), pos.y());
    activateWindow();
}

void TipSample::setupUi() {
    sample = new QGroupBox(this);
    sample->setTitle(QString("采样"));
    group_layout = new QVBoxLayout(sample);
    time = new QRadioButton(sample);
    time->setText(QString("时间"));
    time->setChecked(true);
    frame = new QRadioButton(sample);
    frame->setText(QString("帧数"));
    main_layout->addWidget(sample);
    sample->setLayout(group_layout);
    group_layout->addWidget(time);
    group_layout->addWidget(frame);
    line = new QSpinBox(this);
    main_layout->addWidget(line);
    line->setMinimum(0);
    line->setMaximum(200);
    line->setSingleStep(10);
    line->setValue(10);
    line->setSuffix(QString("毫秒"));
    connect(time, &QRadioButton::released, this, &TipSample::changeSelection);
    connect(frame, &QRadioButton::released, this, &TipSample::changeSelection);
}

void TipSample::setVal() {
    if (time->isChecked()) {
        emit val(QString("时间;%1").arg(line->value()));
    } else if (frame->isChecked()) {
        emit val(QString("帧数;%1").arg(line->value()));
    } else {
        emit val(QString());
    }
    this->hide();
}

void TipSample::changeSelection() {
    if (time->isChecked()) {
        line->setSuffix(QString("毫秒"));
    } else {
        line->setSuffix(QString("帧"));
    }
}
