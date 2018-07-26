//
// Created by jiang.wenqiang on 2018/7/26.
//

#ifndef REFINE_PAINTTEST_H
#define REFINE_PAINTTEST_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QSpinBox>
#include <QtCore/QTimer>
#include "Sketch.h"

class PaintTest : public QDialog{
Q_OBJECT
private:
    QHBoxLayout *layout;
    QWidget *setting_widget;
    QFormLayout *setting_layout;
    Sketch *paint;
    QSpinBox *interval;
    QSpinBox *point_num;
    QSpinBox *line_num;
    QSpinBox *line_width;
    QSpinBox *freq;
    QPushButton *ok;
    QTimer *timer;
public:
protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit PaintTest(QWidget *parent = nullptr);

private:
    void setup_ui();

private slots:
    void setting();
};


#endif //REFINE_PAINTTEST_H
