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

protected:
    void setupUi();

    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:

    void cell(QString s, int row, int column);

    void insert();

    void remove();

    void add();

    void clearAll();

};


#endif //REFINE_CURVETABLE_H
