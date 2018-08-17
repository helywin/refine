//
// Created by jiang.wenqiang on 2018/7/30.
//

#ifndef REFINE_SENDDIALOG_H
#define REFINE_SENDDIALOG_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

class SendDialog : public QMainWindow {
Q_OBJECT
public:
private:
public:
protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit SendDialog(QWidget *parent = nullptr);

    ~SendDialog() final = default;

private:
    void setupUi();
};


#endif //REFINE_SENDDIALOG_H
