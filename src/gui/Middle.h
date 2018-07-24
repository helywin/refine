//
// Created by jiang.wenqiang on 2018/7/23.
//

#ifndef REFINE_MIDDLE_H
#define REFINE_MIDDLE_H

#include <QtWidgets/QWidget>

class Middle : public QWidget {
Q_OBJECT
public:
    explicit Middle(QWidget *parent);

    ~Middle() final = default;

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:

    void cancel_full_screen();
};


#endif //REFINE_MIDDLE_H
