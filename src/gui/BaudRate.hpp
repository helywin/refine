//
// Created by jiang.wenqiang on 2018/11/22.
//

#ifndef REFINE_BAUDRATE_HPP
#define REFINE_BAUDRATE_HPP

#include <QtWidgets/QProgressBar>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include "FiniteProgress.hpp"

class BaudRate : public QWidget
{
Q_OBJECT
private:
    QHBoxLayout *_layout;
    FiniteProgress *_progress;
    QLabel *_label;
    int _maximum_baud_rate;

public:
    explicit BaudRate(QWidget *parent = nullptr);

    inline void setMaximumBaudRate(int maximum)
    {
        _maximum_baud_rate = maximum;
        _progress->setMaximum(_maximum_baud_rate);
    }

    inline int maximumBaudRate() const { return _maximum_baud_rate; }

public slots:
    void setBaudRate(double baud_rate);

private:
    void setup();
};


#endif //REFINE_BAUDRATE_HPP
