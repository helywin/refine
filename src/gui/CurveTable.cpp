//
// Created by jiang.wenqiang on 2018/7/23.
//

#include "CurveTable.h"
#include <QtGui/QMouseEvent>
#include <QtWidgets/QHeaderView>

CurveTable::CurveTable(QWidget *parent) : QTableWidget(parent){
    setup_ui();
}

void CurveTable::setup_ui() {
    test.setWindowTitle(QString("test dialog!"));
    setColumnCount(15);
    setRowCount(10);
    setHorizontalHeaderLabels(QStringList({QString("名称"),QString("备注"),QString("类型"),QString("物理"),QString("宽度"),QString("颜色"),QString("地址"),QString("零字节"),QString("高字节"),QString("低字节"),QString("取点"),QString("时间"),QString("帧数"),QString("输入量程"),QString("输出量程")}));
    setSelectionMode(QAbstractItemView::SingleSelection);


}

void CurveTable::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        int row = currentRow();
        int column = currentColumn();
        test.setWindowTitle(QString("当前%1行%2列").arg(row).arg(column));
        test.show();
    }
}
