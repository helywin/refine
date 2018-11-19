//
// Created by jiang.wenqiang on 2018/11/19.
//

#include <QtCore/QDebug>
#include "CompleteModel.hpp"

CompleteModel::CompleteModel(Tribe *tribe, QObject *parent) :
        _tribe(tribe),
        _selection(Tribe::SelectAll)
{
    _list = _tribe->header(_selection);
    beginResetModel();
    setStringList(_list);
    setParent(parent);
    endResetModel();
}

void CompleteModel::genData()
{
    _list = _tribe->header(_selection);
    beginResetModel();
    setStringList(_list);
//    qDebug() << "CompleteModel::genData" << this->stringList();
    endResetModel();
}
