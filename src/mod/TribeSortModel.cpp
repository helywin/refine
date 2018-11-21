//
// Created by jiang.wenqiang on 2018/11/16.
//

#include "TribeSortModel.hpp"

TribeSortModel::TribeSortModel(QObject *parent) :
        QSortFilterProxyModel(parent),
        _selection(Tribe::SelectAll)
{
    setDynamicSortFilter(true);
}

bool TribeSortModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex col_name = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex col_index = sourceModel()->index(source_row, 0, source_parent);
    return (sourceModel()->data(col_name).toString().contains(filterRegExp()) &&
            isDisplayRule(sourceModel()->data(col_index, Qt::CheckStateRole).toBool()));
}

void TribeSortModel::sort(int column, Qt::SortOrder order)
{
    QSortFilterProxyModel::sort(column, order);
}

bool TribeSortModel::isDisplayRule(bool display) const
{
    return (_selection == Tribe::Selection::SelectAll) ||
           (_selection == Tribe::Selection::SelectChecked && display) ||
           (_selection == Tribe::Selection::SelectUnchecked && !display);
}
