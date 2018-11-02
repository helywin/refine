//
// Created by jiang.wenqiang on 2018/10/31.
//

#include <QtWidgets/QHeaderView>
#include "CurvePanel.hpp"
#include "Tribe.hpp"

CurvePanel::CurvePanel(QWidget *parent) :
        QWidget(parent)
{
    setup();
}

void CurvePanel::setup()
{
    _layout = new QVBoxLayout(this);
    _list = new QTableWidget(this);
    setLayout(_layout);
    _layout->addWidget(_list);
    _layout->setContentsMargins(0, 5, 0, 5);
}

void CurvePanel::updateCurve()
{
    _list->setRowCount(0);
    _list->setSelectionMode(QAbstractItemView::SingleSelection);
    _list->setSelectionBehavior(QAbstractItemView::SelectRows);
    _list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _list->verticalHeader()->hide();
    _list->horizontalHeader()->setHighlightSections(false);
    _list->setColumnWidth(0, 20);
    _list->setColumnWidth(1, 36);
    _list->setColumnWidth(2, 180);
    _list->setColumnWidth(3, 300);
    _list->setHorizontalScrollMode(QTableWidget::ScrollPerPixel);
    _list->setAlternatingRowColors(true);
    QStringList header({tr(""), tr("颜色"), tr("名称"), tr("备注")});
    _list->setHorizontalHeaderLabels(header);
    _list->setColumnCount(header.size());
    if (!_tribe->size()) {
        return;
    }
    _list->clearContents();
    _list->setRowCount(_tribe->size());
    for (int i = 0; i < _tribe->size(); ++i) {
        _list->setRowHeight(i, TABLE_ROW_HEIGHT);
        const Tribe::Style &cell = (*_tribe).style(i);
        auto *display = new QTableWidgetItem();
        if (cell.display()) {
            display->setCheckState(Qt::Checked);
        } else {
            display->setCheckState(Qt::Unchecked);
        }
        _list->setItem(i, 0, display);
        auto *color = new QTableWidgetItem();
        color->setBackgroundColor(QColor(cell.color()));
        color->setForeground(QBrush(QColor(cell.color())));
        color->setFlags(Qt::ItemIsEnabled);
        _list->setItem(i, 1, color);
        auto *name = new QTableWidgetItem();
        name->setText(cell.name());
        _list->setItem(i, 2, name);
        auto *remark = new QTableWidgetItem();
        remark->setText(cell.remark());
        _list->setItem(i, 3, remark);
    }
}
