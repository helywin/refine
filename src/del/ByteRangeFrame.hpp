//
// Created by jiang.wenqiang on 2018/11/7.
//

#ifndef REFINE_BYTERANGEWIDGET_HPP
#define REFINE_BYTERANGEWIDGET_HPP

#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>

class ByteRangeFrame : public QFrame
{
Q_OBJECT
private:
    QHBoxLayout *_layout;
    QCheckBox *_check;
    QSpinBox *_byte;
    QSpinBox *_low;
    QLabel *_label;
    QSpinBox *_high;
    int _last_low;
    int _last_high;
    bool _checkable;
public:
    explicit ByteRangeFrame(bool checkable, QWidget *parent = nullptr);

    inline bool checkable() const { return _checkable; }

    inline bool isChecked() const
    {
        if (checkable()) {
            return _check->isChecked();
        } else {
            return true;
        }
    }

    void setData(const QString &s);

    QString data() const;

private slots:

    void checkChanged(int state);

    void lowChanged(int value);

    void highChanged(int value);

private:
    void setup();
};


#endif //REFINE_BYTERANGEWIDGET_HPP
