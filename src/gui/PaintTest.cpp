//
// Created by jiang.wenqiang on 2018/7/26.
//

#include "PaintTest.h"
#include <QtWidgets/QPushButton>


PaintTest::PaintTest(QWidget *parent) :QDialog(parent) {
    setupUi();
    move(0, 0);
    setWindowFlag(Qt::ToolTip);
    setWindowState(Qt::WindowMaximized);
    setWindowTitle(QString("测试绘图性能"));
}

void PaintTest::setupUi() {
    layout = new QHBoxLayout(this);
    paint = new Sketch(this);
    setting_widget = new QWidget(this);
    setting_layout = new QFormLayout(setting_widget);
    setting_widget->setLayout(setting_layout);
    interval = new QSpinBox(setting_widget);
    point_num = new QSpinBox(setting_widget);
    line_num = new QSpinBox(setting_widget);
    line_width = new QSpinBox(setting_widget);
    freq = new QSpinBox(setting_widget);
    ok = new QPushButton(setting_widget);
    ok->setText(QString("确定"));

    interval->setValue(10);
    interval->setSingleStep(10);
    interval->setSuffix(QString("刷新间隔/ms"));

    point_num->setMaximum(5000);
    point_num->setSingleStep(500);
    point_num->setValue(1000);
    point_num->setSuffix(QString("单条曲线点数"));

    line_num->setValue(20);
    line_num->setSingleStep(10);
    line_num->setSuffix(QString("曲线条数"));

    freq->setValue(5);
    freq->setSingleStep(1);
    freq->setSuffix(QString("波形宽度系数"));

    line_width->setValue(1);
    line_width->setSingleStep(1);
    line_width->setSuffix(QString("曲线宽度"));


    layout->addWidget(paint, 10);
    layout->addWidget(setting_widget,2);
    setting_layout->addRow(QString("间隔"), interval);
    setting_layout->addRow(QString("点数"), point_num);
    setting_layout->addRow(QString("线数"), line_num);
    setting_layout->addRow(QString("线宽"), line_width);
    setting_layout->addRow(QString("频率"), freq);
    setting_layout->addRow(QString(""), ok);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, paint, &Sketch::animate);
    connect(ok, &QPushButton::clicked, this, &PaintTest::setting);
    timer->start(10);
}

void PaintTest::setting() {
    timer->start(interval->value());
    paint->set_param(freq->value(), line_num->value(),
                     point_num->value(), line_width->value());

}

void PaintTest::resizeEvent(QResizeEvent *event) {
    setWindowState(Qt::WindowMaximized);
}
