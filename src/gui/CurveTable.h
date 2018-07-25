//
// Created by jiang.wenqiang on 2018/7/23.
//

#ifndef REFINE_CURVETABLE_H
#define REFINE_CURVETABLE_H

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QDialog>
#include "TableTips.h"


class CurveTable : public QTableWidget {
Q_OBJECT
private:
    GetVal *get_val;
public:
    explicit CurveTable(QWidget *parent);

    ~CurveTable() final = default;

    void setup_ui();


protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:
    void cell(QString s, int row, int column);

};


#endif //REFINE_CURVETABLE_H
