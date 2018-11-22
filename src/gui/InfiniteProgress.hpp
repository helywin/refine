//
// Created by jiang.wenqiang on 2018/11/14.
//

#ifndef REFINE_INFINITEPROGRESS_HPP
#define REFINE_INFINITEPROGRESS_HPP

#include <QtWidgets/QWidget>
#include <QtCore/QTimer>

class InfiniteProgress : public QWidget
{
Q_OBJECT
private:
    QColor _border;
    QColor _fill;
    QTimer _timer;
    double _process;
    double _len;
public:
    explicit InfiniteProgress(QWidget *parent, int msec = 10);
protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateProgress();
};


#endif //REFINE_INFINITEPROGRESS_HPP
