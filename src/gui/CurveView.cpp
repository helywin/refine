//
// Created by jiang.wenqiang on 2018/11/5.
//

#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollBar>
#include "CurveView.hpp"
#include "CurveModel.hpp"

CurveView::CurveView(QAbstractItemModel *model, QItemSelectionModel *selection,
                     QHeaderView *h, QHeaderView *v, QWidget *parent) :
        QTableView(parent)
{
    setModel(model);
    setSelectionModel(selection);
    setHorizontalHeader(h);
    setVerticalHeader(v);

    setColumnWidth(CurveModel::NameColumn, 200);
    setColumnWidth(CurveModel::BundleColumn, 100);
    setColumnWidth(CurveModel::UnitColumn, 80);
    setColumnWidth(CurveModel::WidthColumn, 40);
    setColumnWidth(CurveModel::ColorColumn, 100);
    setColumnWidth(CurveModel::CanIdColumn, 80);
    setColumnWidth(CurveModel::ZeroByteColumn, 80);
    setColumnWidth(CurveModel::HighByteColumn, 80);
    setColumnWidth(CurveModel::LowByteColumn, 80);
    setColumnWidth(CurveModel::FrameMsecColumn, 80);
    setColumnWidth(CurveModel::RangeInColumn, 100);
    setColumnWidth(CurveModel::RangeOutColumn, 100);
    setColumnWidth(CurveModel::RemarkColumn, 200);

    setSelectionMode(QTableView::SingleSelection);
    setSelectionBehavior(QTableView::SelectRows);
    setHorizontalScrollMode(QTableView::ScrollPerPixel);
    setVerticalScrollMode(QTableView::ScrollPerPixel);
    _frozen = new QTableView(this);
    _frozen->setModel(model);
    _frozen->setSelectionModel(selection);

    for (int col = 1; col < model->columnCount(); ++col) {
        _frozen->setColumnHidden(col, true);
    }
    _frozen->verticalHeader()->hide();
//    _frozen->setFocusPolicy(Qt::NoFocus);
    _frozen->setSelectionMode(QTableView::SingleSelection);
    _frozen->setSelectionBehavior(QTableView::SelectRows);
    _frozen->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    viewport()->stackUnder(_frozen);
    _frozen->setColumnWidth(0, columnWidth(0));
    _frozen->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _frozen->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _frozen->setVerticalScrollMode(ScrollPerPixel);
    _frozen->show();
    connect(horizontalHeader(), &QHeaderView::sectionResized, this,
            &CurveView::updateSectionWidth);
    connect(verticalHeader(), &QHeaderView::sectionResized, this,
            &CurveView::updateSectionHeight);
    connect(_frozen->verticalScrollBar(), &QAbstractSlider::valueChanged,
            verticalScrollBar(), &QAbstractSlider::setValue);
    connect(verticalScrollBar(), &QAbstractSlider::valueChanged,
            _frozen->verticalScrollBar(), &QAbstractSlider::setValue);
    updateFrozenTableGeometry();
}

void CurveView::resizeEvent(QResizeEvent *event)
{
    QAbstractItemView::resizeEvent(event);
    updateFrozenTableGeometry();
}

QModelIndex CurveView::moveCursor(QAbstractItemView::CursorAction cursorAction,
                                  Qt::KeyboardModifiers modifiers)
{
    QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);

    if (cursorAction == MoveLeft && current.column() > 0
        && visualRect(current).topLeft().x() < _frozen->columnWidth(0)) {
        const int newValue = horizontalScrollBar()->value() +
                             visualRect(current).topLeft().x()
                             - _frozen->columnWidth(0);
        horizontalScrollBar()->setValue(newValue);
    }
    return current;
}

void CurveView::scrollTo(const QModelIndex &index,
                         QAbstractItemView::ScrollHint hint)
{
    if (index.column() > 0) {
        QTableView::scrollTo(index, hint);
    }
}

void CurveView::updateSectionWidth(int index, int old_size, int new_size)
{
    if (index == 0) {
        _frozen->setColumnWidth(0, new_size);
        updateFrozenTableGeometry();
    }
}

void CurveView::updateSectionHeight(int index, int old_size, int new_size)
{
    _frozen->setRowHeight(index, new_size);
}

void CurveView::updateFrozenTableGeometry()
{
    _frozen->setGeometry(verticalHeader()->width() + frameWidth() - 1,
                         frameWidth() - 1,
                         columnWidth(0) + 1,
                         viewport()->height() + horizontalHeader()->height() +
                         1);
}

