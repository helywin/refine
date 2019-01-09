//
// Created by jiang.wenqiang on 2019/1/9.
//

#ifndef REFINE_ACCINPUT_HPP
#define REFINE_ACCINPUT_HPP

#include <QtWidgets/QComboBox>

class AccInput : public QComboBox
{
Q_OBJECT
private:
    unsigned int _last_value;
    QRegExpValidator *_validator;

public:
    explicit AccInput(QWidget *parent);
    void addAcc(unsigned int value);

protected:
    void focusOutEvent(QFocusEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private:
    void setup();

    inline QString toStr(unsigned int value)
    { return QString("0x%1").arg(value, 8, 16, QChar('0')); }

signals:
    void valueChanged(unsigned int value);
};


#endif //REFINE_ACCINPUT_HPP
