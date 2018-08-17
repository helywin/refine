//
// Created by jiang.wenqiang on 2018/7/23.
//

#include "CurveTable.h"
#include <QtGui/QMouseEvent>

CurveTable::CurveTable(QWidget *parent) : QTableWidget(parent) {
    setupUi();
}

void CurveTable::setupUi() {
    get_val = new GetVal(this);
    connect(get_val, &GetVal::getVal, this, &CurveTable::cell);
    setColumnCount(13);
    setRowCount(0);
    setHorizontalHeaderLabels(QStringList(
            {QString("名称"), QString("类型"), QString("单位"),
             QString("宽度"), QString("颜色"), QString("地址"), QString("零字节"),
             QString("高字节"), QString("低字节"), QString("采样"),
             QString("输入量程"), QString("输出量程"), QString("备注")}));
    setSelectionMode(QAbstractItemView::SingleSelection);
    setColumnWidth(0, 150);
    setColumnWidth(1, 60);
    setColumnWidth(2, 70);
    setColumnWidth(3, 60);
    setColumnWidth(4, 60);
    setColumnWidth(5, 80);
    setColumnWidth(6, 80);
    setColumnWidth(7, 80);
    setColumnWidth(8, 80);
    setColumnWidth(9, 80);
    setColumnWidth(10, 120);
    setColumnWidth(11, 120);
    setColumnWidth(12, 240);


}

void CurveTable::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        switch (currentColumn()) {
            case 1:
                get_val->get(GetVal::Type::Type, currentRow(),
                             currentColumn(), event->globalPos());
                break;
            case 6:
                get_val->get(GetVal::Type::ZeroByte, currentRow(),
                             currentColumn(), event->globalPos());
                break;
            case 9:
                get_val->get(GetVal::Type::Sample, currentRow(),
                             currentColumn(), event->globalPos());
                break;
            default:
                break;
        }
    }
}

void CurveTable::cell(QString s, int row, int column) {
    item(row, column)->setText(s);
//    this->setItem(row, column, new QTableWidgetItem(s));
}

void CurveTable::insert() {
    int index = 0;
    if (rowCount() == 0 | currentRow() == -1) {
        index = 0;
    } else {
        index = currentRow();
    }
//    qDebug() << "current row" << currentRow();
    insertRow(index);
    for (int i = 0; i < columnCount(); ++i) {
        setItem(index, i, new QTableWidgetItem());
    }
    item(index, 1)->setFlags(item(index, 1)->flags() & ~Qt::ItemIsEditable);
    item(index, 2)->setFlags(item(index, 2)->flags() & ~Qt::ItemIsEditable);
    item(index, 4)->setFlags(item(index, 4)->flags() & ~Qt::ItemIsEditable);
    item(index, 5)->setFlags(item(index, 5)->flags() & ~Qt::ItemIsEditable);
    item(index, 6)->setFlags(item(index, 6)->flags() & ~Qt::ItemIsEditable);
    item(index, 7)->setFlags(item(index, 7)->flags() & ~Qt::ItemIsEditable);
    item(index, 8)->setFlags(item(index, 8)->flags() & ~Qt::ItemIsEditable);
    item(index, 9)->setFlags(item(index, 9)->flags() & ~Qt::ItemIsEditable);
    item(index, 10)->setFlags(item(index, 10)->flags() & ~Qt::ItemIsEditable);
    item(index, 11)->setFlags(item(index, 11)->flags() & ~Qt::ItemIsEditable);
}

void CurveTable::remove() {
    if (rowCount() != 0) {
        removeRow(currentRow());
    }
}

void CurveTable::add() {
    int index = rowCount();
    insertRow(index);
    for (int i = 0; i < columnCount(); ++i) {
        setItem(index, i, new QTableWidgetItem());
    }
    item(index, 1)->setFlags(item(index, 1)->flags() & ~Qt::ItemIsEditable);
    item(index, 2)->setFlags(item(index, 2)->flags() & ~Qt::ItemIsEditable);
    item(index, 4)->setFlags(item(index, 4)->flags() & ~Qt::ItemIsEditable);
    item(index, 5)->setFlags(item(index, 5)->flags() & ~Qt::ItemIsEditable);
    item(index, 6)->setFlags(item(index, 6)->flags() & ~Qt::ItemIsEditable);
    item(index, 7)->setFlags(item(index, 7)->flags() & ~Qt::ItemIsEditable);
    item(index, 8)->setFlags(item(index, 8)->flags() & ~Qt::ItemIsEditable);
    item(index, 9)->setFlags(item(index, 9)->flags() & ~Qt::ItemIsEditable);
    item(index, 10)->setFlags(item(index, 10)->flags() & ~Qt::ItemIsEditable);
    item(index, 11)->setFlags(item(index, 11)->flags() & ~Qt::ItemIsEditable);
}

void CurveTable::clearAll() {
    while (rowCount() > 0) {
        removeRow(0);
    }
}
