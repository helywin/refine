//
// Created by jiang.wenqiang on 2018/11/16.
//

#ifndef REFINE_TRIBESORTMODEL_HPP
#define REFINE_TRIBESORTMODEL_HPP

#include <QtCore/QSortFilterProxyModel>

class TribeSortModel : public QSortFilterProxyModel
{
Q_OBJECT
private:
public:
    explicit TribeSortModel(QObject *parent = nullptr);
};


#endif //REFINE_TRIBESORTMODEL_HPP
