//
// Created by jiang.wenqiang on 2019/1/2.
//

#ifndef REFINE_CANMESSAGE_HPP
#define REFINE_CANMESSAGE_HPP

#include <QtWidgets/QPlainTextEdit>

class CanMessage : public QPlainTextEdit
{
Q_OBJECT
private:
public:
    CanMessage(QWidget *parent = nullptr);

private:
    void setup();
protected:
    void paintEvent(QPaintEvent *e) override;
};


#endif //REFINE_CANMESSAGE_HPP
