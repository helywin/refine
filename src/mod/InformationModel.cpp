//
// Created by jiang.wenqiang on 2018/12/26.
//

#include "InformationModel.hpp"
#include "Information.hpp"

InformationModel::InformationModel()
        : QAbstractItemModel()
{}

int InformationModel::rowCount(const QModelIndex &parent) const
{
    if (!_info) {
        return 0;
    }
    return _info->size(_type);
}

int InformationModel::columnCount(const QModelIndex &parent) const
{
    return 2;       //时间+类型+数据
}

QVariant InformationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    int row = index.row();
    int column = index.column();
    if (row > _info->size(_type)) {
        return QVariant();
    }
    const Information::Cell &cell = _info->data(row, _type);
    switch (role) {
        case Qt::DisplayRole:
            switch (column) {
                case 0:
                    return cell.time().toString("hh:mm:ss");
                case 1:
                    return static_cast<int>(cell.type());
                case 2:
                    return cell.message();
                default:
                    return QVariant();
            }
        default:
            return QVariant();
    }
}

Qt::ItemFlags InformationModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

void InformationModel::genData(Information *info, Re::MessageTypes type)
{
    if (type == Re::NoMessageType) {
        beginResetModel();
        endResetModel();
    } else {
        beginResetModel();
        _info = info;
        _type = type;
        endResetModel();
    }
}
