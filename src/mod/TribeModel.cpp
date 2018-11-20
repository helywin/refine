//
// Created by jiang.wenqiang on 2018/11/6.
//
#include <QtCore/QDebug>
#include <QtGui/QColor>
#include <QtCore/QSize>
#include "Tribe.hpp"
#include "TribeModel.hpp"

TribeModel::TribeModel(QObject *parent) :
        QAbstractTableModel(parent),
        _tribe(nullptr) {}

int TribeModel::rowCount(const QModelIndex &parent) const
{
    if (!_tribe) {
        return 0;
    } else {
        return _tribe->size();
    }
}

int TribeModel::columnCount(const QModelIndex &parent) const
{
    return ColumLast + 1;
}

QVariant TribeModel::data(const QModelIndex &index, int role) const
{
    if (!_tribe) {
        return QVariant();
    }
    if (!index.isValid()) {
        return QVariant();
    }
    int row = index.row();
    int column = index.column();
    if (row >= _tribe->size()) {
        qCritical("CurveModel::data 数组越界");
    }
    const Tribe::Style &cell = _tribe->_styles[row];
    switch (role) {
        case Qt::DisplayRole:
            switch (column) {
                case IndexColumn:
                    return QVariant(cell.index());
                case NameColumn:
                    return QVariant(cell.nameStr());
                case UnitColumn:
                    return QVariant(cell.unitStr());
                case WidthColumn:
                    return QVariant(cell.widthStr());
                case ColorColumn:
                    return QVariant(cell.colorStr());
                case RangeOutColumn:
                    return QVariant(cell.rangeOutStr());
                case RemarkColumn:
                    return QVariant(cell.remarkStr());
                default:
                    return QVariant();
            }
        case Qt::CheckStateRole:
        case Qt::UserRole:
            if (column == IndexColumn) {
                if (cell.display())
                    return Qt::Checked;
                else
                    return Qt::Unchecked;
            }
            return QVariant();
        case Qt::DecorationRole:
            if (column == NameColumn || column == ColorColumn) {
                return QColor(cell.color());
            }
            return QVariant();
        default:
            return QVariant();
    }
}

bool TribeModel::setData(const QModelIndex &index,
                         const QVariant &value, int role)
{
    if (!_tribe) {
        return false;
    }
    if (!index.isValid()) {
        return false;
    }
    int row = index.row();
    int column = index.column();
    if (row >= _tribe->size()) {
        qCritical("CurveModel::setData 数组越界");
    }
    Tribe::Style &cell = _tribe->_styles[row];
    switch (role) {
        case Qt::DisplayRole:
            return false;
        case Qt::CheckStateRole:
            if (column == IndexColumn) {
                cell._display = value.toBool();
                dataChanged(index, index);
            }
            tribeChanged();
            return true;
        default:
            return false;
    }
}

Qt::ItemFlags TribeModel::flags(const QModelIndex &index) const
{
    if (!_tribe || !index.isValid()) {
        return QAbstractTableModel::flags(index);
    }
    Qt::ItemFlags flags = Qt::ItemIsSelectable |
                          Qt::ItemIsEnabled;
//                          |
//                          Qt::ItemIsEditable;
    switch (index.column()) {
        case IndexColumn:
            return flags | Qt::ItemIsUserCheckable;
        case NameColumn:
        case UnitColumn:
        case WidthColumn:
        case ColorColumn:
        case RangeOutColumn:
        case RemarkColumn:
            return flags;
        default:
            return QAbstractTableModel::flags(index);
    }
}

void TribeModel::genData(Tribe *tribe)
{
    if (tribe) {
        beginResetModel();
        _tribe = tribe;
//        qDebug() << "TribeModel::genData" << _tribe->header();
        endResetModel();
    } else {
        beginResetModel();
        endResetModel();
    }
    tribeChanged();
}

QVariant TribeModel::headerData(int section,
                                Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Orientation::Horizontal) {
        switch (role) {
            case Qt::DisplayRole:
                switch (section) {
                    case NameColumn:
                        return tr("名称");
                    case UnitColumn:
                        return tr("单位");
                    case WidthColumn:
                        return tr("线宽");
                    case ColorColumn:
                        return tr("颜色");
                    case RangeOutColumn:
                        return tr("输出范围");
                    case RemarkColumn:
                        return tr("备注");
                    default:
                        return QVariant();
                }
            default:
                return QVariant();
        }
    } else {
        switch (role) {
            case Qt::DisplayRole:
                return section;
            default:
                return QVariant();
        }
    }
}
