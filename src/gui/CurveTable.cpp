//
// Created by jiang.wenqiang on 2018/7/23.
//

#include "CurveTable.h"
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>

CurveTable::CurveTable(QWidget *parent) : QTableWidget(parent) {
    setupUi();
}

void CurveTable::setupUi() {
    get_val = new GetVal(this);
    connect(get_val, &GetVal::get_val, this, &CurveTable::cell);
    setColumnCount(15);
    setRowCount(10);
    setHorizontalHeaderLabels(QStringList(
            {QString("名称"), QString("备注"), QString("类型"), QString("物理"),
             QString("宽度"), QString("颜色"), QString("地址"), QString("零字节"),
             QString("高字节"), QString("低字节"), QString("取点"), QString("时间"),
             QString("帧数"), QString("输入量程"), QString("输出量程")}));
    setSelectionMode(QAbstractItemView::SingleSelection);


}

void CurveTable::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        switch (currentColumn()) {
            case 2:
                get_val->get(GetVal::Type::Type, currentRow(), currentColumn(), event->globalPos());
                break;
            default:
                break;
        }
    }
}

void CurveTable::cell(QString s, int row, int column) {
    this->setItem(row, column, new QTableWidgetItem(s));
}
