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
{
    if (!_curve) {
        return 0;
    } else {
        return ColumLast;
    }
}

QVariant CurveModel::data(const QModelIndex &index, int role) const
{
    if (!_curve) {
        return QVariant();
    }
    if (!index.isValid()) {
        return QVariant();
    }
    int row = index.row();
    int column = index.column();
    if (row >= _curve->size()) {
        qCritical("数组越界");
    }
    const Curve::Cell &cell = _curve->_cells[row];
    switch (role) {
        case Qt::DisplayRole:
            switch (column) {
                case NameColumn:
                    return QVariant(cell.nameStr());
                case TypeColumn:
                    return QVariant(cell.typeStr());
                case UnitColumn:
                    return QVariant(cell.unitStr());
                case WidthColumn:
                    return QVariant(cell.widthStr());
                case ColorColumn:
                    return QVariant(cell.colorStr());
                case CanIdColumn:
                    return QVariant(cell.canIdStr());
                case ZeroByteColumn:
                    return QVariant(cell.zeroByteStr());
                case HighByteColumn:
                    return QVariant(cell.highByteStr());
                case LowByteColumn:
                    return QVariant(cell.lowByteStr());
                case FrameMsecColumn:
                    return QVariant(cell.frameMsecStr());
                case RangeInColumn:
                    return QVariant(cell.rangeInStr());
                case RangeOutColumn:
                    return QVariant(cell.rangeOutStr());
                case RemarkColumn:
                    return QVariant(cell.remarkStr());
                default:
                    return QVariant();
            }
        case Qt::CheckStateRole:
        case Qt::UserRole:
            if (column == NameColumn) {
                if (cell.display())
                    return Qt::Checked;
                else
                    return Qt::Unchecked;
            }
            return QVariant();
        case Qt::DecorationRole:
            if (column == ColorColumn) {
                return QColor(QRgb(cell.color()));
            }
            return QVariant();
        default:
            return QVariant();
    }
}

bool
CurveModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!_curve) {
        return false;
    }
    if (!index.isValid()) {
        return false;
    }
    int row = index.row();
    int column = index.column();
    if (row >= _curve->size()) {
        qCritical("数组越界");
    }
    Curve::Cell &cell = _curve->_cells[row];
    switch (role) {
        case Qt::DisplayRole:
            switch (column) {
                case NameColumn:cell.setNameByStr(value.toString()); return true;
                case TypeColumn:cell.setTypeByStr(value.toString()); return true;
                case UnitColumn:cell.setUnitByStr(value.toString()); return true;
                case WidthColumn:cell.setWidthByStr(value.toString()); return true;
                case ColorColumn:cell.setColorByStr(value.toString()); return true;
                case CanIdColumn:cell.setCanIdByStr(value.toString()); return true;
                case ZeroByteColumn:cell.setZeroByteByStr(value.toString()); return true;
                case HighByteColumn:cell.setHighByteByStr(value.toString()); return true;
                case LowByteColumn:cell.setLowByteByStr(value.toString()); return true;
                case FrameMsecColumn:cell.setFrameMsecByStr(value.toString()); return true;
                case RangeInColumn:cell.setRangeInByStr(value.toString()); return true;
                case RangeOutColumn:cell.setRangeOutByStr(value.toString()); return true;
                case RemarkColumn:cell.setRemarkByStr(value.toString()); return true;
                default:
                    return false;
            }
        case Qt::CheckStateRole:
            if (column == NameColumn) {
                cell._display = value.toBool();
                dataChanged(index, index);
            }
        default:
            return false;
    }
}

Qt::ItemFlags CurveModel::flags(const QModelIndex &index) const
{
    if (!_curve || !index.isValid()) {
        return QAbstractTableModel::flags(index);
    }
    Qt::ItemFlags flags = Qt::ItemIsSelectable |
            Qt::ItemIsEnabled |
            Qt::ItemIsEditable;
    switch (index.column()) {
        case NameColumn: return flags | Qt::ItemIsUserCheckable;
        case TypeColumn:
        case UnitColumn:
        case WidthColumn:
        case ColorColumn:
        case CanIdColumn:
        case ZeroByteColumn:
        case HighByteColumn:
        case LowByteColumn:
        case FrameMsecColumn:
        case RangeInColumn:
        case RangeOutColumn:
        case RemarkColumn: return flags;
        default:
            return QAbstractTableModel::flags(index);
    }
}

void CurveModel::genData(Curve *curve)
{
    beginResetModel();
    _curve = curve;
    endResetModel();
}
