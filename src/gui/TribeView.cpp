//
// Created by jiang.wenqiang on 2018/11/6.
//

#include <QtWidgets/QHeaderView>
#include "TribeView.hpp"
#include "TribeModel.hpp"

TribeView::TribeView(QAbstractItemModel *model,
                     QItemSelectionModel *selection,
                     QWidget *parent) :
        QTableView(parent)
{
    setModel(model);
    if (selection) {
        setSelectionModel(selection);
    }
    verticalHeader()->setDefaultSectionSize(20);
    verticalHeader()->setHidden(true);
    setAlternatingRowColors(true);
    setHorizontalScrollMode(QTableView::ScrollPerPixel);
    setVerticalScrollMode(QTableView::ScrollPerPixel);
    setSelectionMode(QTableView::SingleSelection);
    setSelectionBehavior(QTableView::SelectRows);

    setColumnWidth(TribeModel::IndexColumn, 50);
    setColumnWidth(TribeModel::NameColumn, 190);
    setColumnWidth(TribeModel::UnitColumn, 80);
    setColumnWidth(TribeModel::WidthColumn, 40);
    setColumnWidth(TribeModel::ColorColumn, 100);
    setColumnWidth(TribeModel::RangeOutColumn, 100);
    setColumnWidth(TribeModel::PrecisionColumn, 40);
    setColumnWidth(TribeModel::RemarkColumn, 200);
}


