//
// Created by jiang.wenqiang on 2018/11/16.
//

#ifndef REFINE_TRIBECOMPLETEMODEL_HPP
#define REFINE_TRIBECOMPLETEMODEL_HPP

#include <QtCore/QStringListModel>

class TribeCompleteModel : public QStringListModel
{
Q_OBJECT
private:
public:
    TribeCompleteModel(QObject * parent = nullptr);
};


#endif //REFINE_TRIBECOMPLETEMODEL_HPP
