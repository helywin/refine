//
// Created by jiang.wenqiang on 2018/10/30.
//

#ifndef REFINE_LOSTFOCUS_HPP
#define REFINE_LOSTFOCUS_HPP

#include <QtWidgets/QDialog>

class LostFocus : public QDialog
{
Q_OBJECT
public:
    explicit LostFocus(QWidget *parent = nullptr);
protected:
    bool eventFilter(QObject *object, QEvent *event) override;
};


#endif //REFINE_LOSTFOCUS_HPP
