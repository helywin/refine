//
// Created by jiang.wenqiang on 2018/11/6.
//

#include <QtWidgets/QHeaderView>
#include "TribeView.hpp"
#include "TribeModel.hpp"

TribeView::TribeView(QAbstractItemModel *model,
                     QItemSelectionModel *selection,
                     QHeaderView *h, QHeaderView *v,
                     QWidget *parent) :
        QTableView(parent)
{
    setModel(model);
    setSelectionModel(selection);
    setHorizontalHeader(h);
    setVerticalHeader(v);
    verticalHeader()->setDefaultSectionSize(20);
    setAlternatingRowColors(true);
    setHorizontalScrollMode(QTableView::ScrollPerPixel);
    setVerticalScrollMode(QTableView::ScrollPerPixel);
    setSelectionMode(QTableView::SingleSelection);
    setSelectionBehavior(QTableView::SelectRows);

    setColumnWidth(TribeModel::NameColumn, 220);
    setColumnWidth(TribeModel::BundleColumn, 100);
    setColumnWidth(TribeModel::UnitColumn, 80);
    setColumnWidth(TribeModel::WidthColumn, 40);
    setColumnWidth(TribeModel::ColorColumn, 100);
    setColumnWidth(TribeModel::RangeOutColumn, 100);
    setColumnWidth(TribeModel::RemarkColumn, 200);
}


