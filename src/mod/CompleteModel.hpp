//
// Created by jiang.wenqiang on 2018/11/19.
//

#ifndef REFINE_COMPLETEMODEL_HPP
#define REFINE_COMPLETEMODEL_HPP

#include <QtCore/QStringListModel>
#include "Tribe.hpp"

class CompleteModel : public QStringListModel
{
Q_OBJECT
public:

private:
    Tribe *_tribe;
    QStringList _list;
    Tribe::Selection _selection;

public:
    explicit CompleteModel(Tribe *tribe, QObject *parent = nullptr);

    inline void setSelection(Tribe::Selection selection) { _selection = selection; }

public slots:
    void genData();
};


#endif //REFINE_COMPLETEMODEL_HPP
