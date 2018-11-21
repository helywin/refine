//
// Created by jiang.wenqiang on 2018/11/16.
//

#ifndef REFINE_TRIBESORTMODEL_HPP
#define REFINE_TRIBESORTMODEL_HPP

#include <QtCore/QSortFilterProxyModel>
#include "Tribe.hpp"

class TribeSortModel : public QSortFilterProxyModel
{
Q_OBJECT
private:
    Tribe::Selection _selection;
public:
    explicit TribeSortModel(QObject *parent = nullptr);
    void sort(int column, Qt::SortOrder order) override;

    inline void setSelection(Tribe::Selection selection)
    {
        _selection = selection;
        invalidateFilter();
    }

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    bool isDisplayRule(bool display) const;
};

#endif //REFINE_TRIBESORTMODEL_HPP
