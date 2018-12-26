//
// Created by jiang.wenqiang on 2018/12/26.
//

#ifndef REFINE_INFORMATIONMODEL_HPP
#define REFINE_INFORMATIONMODEL_HPP

#include <QtCore/QAbstractItemModel>
#include "Global.hpp"

class Information;

class InformationModel : public QAbstractItemModel
{
Q_OBJECT
private:
    Information *_info = nullptr;
    Re::MessageTypes _type = Re::MessageType::NoMessageType;
public:
    InformationModel();
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:
    void genData(Information *info = nullptr, Re::MessageTypes type = Re::NoMessageType);
};


#endif //REFINE_INFORMATIONMODEL_HPP
