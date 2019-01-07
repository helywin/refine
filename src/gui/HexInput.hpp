//
// Created by jiang.wenqiang on 2019/1/7.
//

#ifndef REFINE_HEXINPUT_HPP
#define REFINE_HEXINPUT_HPP

#include <QtWidgets/QLineEdit>
#include <QtCore/QRegExp>
#include <QtGui/QRegExpValidator>

class HexInput : public QLineEdit
{
Q_OBJECT
private:
    int _num = 3;
    QFont _font;
    QRegExpValidator *_validator;
public:
    explicit HexInput(int num, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
private:
    void setup();
};


#endif //REFINE_HEXINPUT_HPP
