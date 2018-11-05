//
// Created by jiang.wenqiang on 2018/11/2.
//

#include <QtCore/QDebug>
#include "CurveModel.hpp"
#include "Curve.hpp"

CurveModel::CurveModel(QObject *parent) :
        _curve(nullptr),
        QAbstractTableModel(parent) {}

int CurveModel::rowCount(const QModelIndex &parent) const
{
    if (!_curve) {
        return 0;
    } else {
        return _curve->size();
    }
}

int CurveModel::columnCount(const QModelIndex &parent) const
{if (!_curve) {
        return 0;
    } else {
        return 14;
    }
}

QVariant CurveModel::data(const QModelIndex &index, int role) const
{
    if (!_curve) {
        return QVariant();
    }
    if (role == Qt::DisplayRole) {
        qDebug() << "display";
        if (index.row() >= _curve->size()) {
            qCritical("数组越界");
        }
        Curve::Cell &cell = _curve->_cells[index.row()];
        switch (index.column()) {
            case 0:
                return QVariant(cell.indexStr());
            case 1:
                return QVariant(cell.displayStr());
            case 2:
                return QVariant(cell.nameStr());
            case 3:
                return QVariant(cell.typeStr());
            case 4:
                return QVariant(cell.unitStr());
            case 5:
                return QVariant(cell.widthStr());
            case 6:
                return QVariant(cell.colorStr());
            case 7:
                return QVariant(cell.canIdStr());
            case 8:
                return QVariant(cell.zeroByteStr());
            case 9:
                return QVariant(cell.highByteStr());
            case 10:
                return QVariant(cell.lowByteStr());
            case 11:
                return QVariant(cell.frameMsecStr());
            case 12:
                return QVariant(cell.rangeInStr());
            case 13:
                return QVariant(cell.rangeOutStr());
            case 14:
                return QVariant(cell.remarkStr());
            default:
                return QVariant();
        }
    }
    return QVariant();
}

bool
CurveModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!_curve) {
        return false;
    }
    return QAbstractItemModel::setData(index, value, role);
}

Qt::ItemFlags CurveModel::flags(const QModelIndex &index) const
{
    if (!_curve) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
void CurveModel::genData(Curve *curve)
{
    beginResetModel();
    _curve = curve;
    endResetModel();
}
