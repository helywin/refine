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
    main_layout->setContentsMargins(0,0,0,5);
    button_layout = new QVBoxLayout(button_widget);
    button_layout->setContentsMargins(0,0,0,0);
    main_widget->setLayout(main_layout);
    button_widget->setLayout(button_layout);

    ok_button = new QPushButton(button_widget);
    ok_button->setText("确定");
    button_layout->addWidget(ok_button);

}


TipType::TipType(QWidget *parent) : TableTips(parent) {
    this->setupUi();
}

void TipType::setupUi() {
    type = new QGroupBox(QString("类型"), main_widget);
    physical = new QRadioButton(QString("物理"),type);
    physical->setChecked(true);
    logical = new QRadioButton(QString("逻辑"),type);
    logical->setChecked(false);
    radio_layout = new QVBoxLayout(type);
    type->setLayout(radio_layout);
    radio_layout->addWidget(physical);
    radio_layout->addWidget(logical);
    main_layout->addWidget(type);
    this->setFixedSize(120,140);
    connect(ok_button, &QPushButton::clicked, this, &TipType::setVal);
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
    if (physical->isChecked()){
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
    move(pos.x(),pos.y());
    activateWindow();
}


GetVal::GetVal(QWidget *parent) :
        row(-1), column(-1),type(new TipType(parent)) {
    connect(type, &TipType::val, this, &GetVal::got_val);
}

void GetVal::get(GetVal::Type t, int row, int column, QPoint pos) {
    type->regen(pos);
    this->row = row;
    this->column = column;
}

void GetVal::got_val(QString s) {
    emit get_val(s, row, column);
}
