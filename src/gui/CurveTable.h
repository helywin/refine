//
// Created by jiang.wenqiang on 2018/7/23.
//

#ifndef REFINE_CURVETABLE_H
#define REFINE_CURVETABLE_H

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QDialog>

class CurveTable : public QTableWidget {
Q_OBJECT
private:
    QDialog test;
public:
    explicit CurveTable(QWidget *parent);

    ~CurveTable() final = default;

    void setup_ui();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};


#endif //REFINE_CURVETABLE_H
