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
    return ColumLast + 1;
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
        qCritical("CurveModel::data 数组越界");
    }
    const Curve::Cell &cell = _curve->_cells[row];
    switch (role) {
        case Qt::DisplayRole:
            switch (column) {
                case NameColumn:
                    return QVariant(cell.nameStr());
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
                case RangeInColumn:
                    return QVariant(cell.rangeInStr());
                case RangeOutColumn:
                    return QVariant(cell.rangeOutStr());
                case RemarkColumn:
                    return QVariant(cell.remarkStr());
                default:
                    return QVariant();
            }
        case Qt::EditRole:
            switch (column) {
                case NameColumn:
                    return QVariant(cell.name());
                case UnitColumn:
                    return QVariant(cell.unit());
                case WidthColumn:
                    return QVariant(cell._width);
                case ColorColumn:
                    return QVariant(cell._color);
                case CanIdColumn:
                    return QVariant(cell._can_id);
                case ZeroByteColumn:
                    return QVariant(cell.zeroByteStr());
                case HighByteColumn:
                    return QVariant(cell.highByteStr());
                case LowByteColumn:
                    return QVariant(cell.lowByteStr());
                case RangeInColumn:
                    return QVariant(cell.rangeInStr());
                case RangeOutColumn:
                    return QVariant(cell.rangeOutStr());
                case RemarkColumn:
                    return QVariant(cell.remark());
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
                return QVariant(QColor((unsigned int)cell.color()));
            }
            return QVariant();
        case Qt::ToolTipRole:
            if (column == NameColumn) {
                return QVariant(tr("序号:") + cell.indexStr());
            }
        default:
            return QVariant();
    }
}

bool CurveModel::setData(const QModelIndex &index,
                         const QVariant &value, int role)
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
        qCritical("CurveModel::setData 数组越界");
    }
    Curve::Cell &cell = _curve->_cells[row];
    switch (role) {
        case Qt::EditRole:
            switch (column) {
                case NameColumn:
                    cell.setNameByStr(value.toString());
                    _curve->updateHeader();     //更新表头
                    return true;
                case UnitColumn:
                    cell.setUnitByStr(value.toString());
                    return true;
                case WidthColumn:
                    cell._width = value.toInt();
                    return true;
                case ColorColumn:
                    cell._color = value.toUInt();
                    return true;
                case CanIdColumn:
                    cell._can_id = value.toInt();
                    return true;
                case ZeroByteColumn:
                    cell.setZeroByteByStr(value.toString());
                    return true;
                case HighByteColumn:
                    cell.setHighByteByStr(value.toString());
                    return true;
                case LowByteColumn:
                    cell.setLowByteByStr(value.toString());
                    return true;
                case RangeInColumn:
                    cell.setRangeInByStr(value.toString());
                    return true;
                case RangeOutColumn:
                    cell.setRangeOutByStr(value.toString());
                    return true;
                case RemarkColumn:
                    cell.setRemarkByStr(value.toString());
                    return true;
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
        case UnitColumn:
        case WidthColumn:
        case ColorColumn:
        case CanIdColumn:
        case ZeroByteColumn:
        case HighByteColumn:
        case LowByteColumn:
        case RangeInColumn:
        case RangeOutColumn:
        case RemarkColumn:
            return flags;
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

QVariant CurveModel::headerData(int section,
                                Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Orientation::Horizontal) {
        switch (role) {
            case Qt::DisplayRole:
                switch (section) {
                    case NameColumn:return tr("名称");
                    case UnitColumn:return tr("单位");
                    case WidthColumn:return tr("线宽");
                    case ColorColumn:return tr("颜色");
                    case CanIdColumn:return tr("报文ID");
                    case ZeroByteColumn:return tr("零字节");
                    case HighByteColumn:return tr("高字节");
                    case LowByteColumn:return tr("低字节");
                    case RangeInColumn:return tr("输入范围");
                    case RangeOutColumn:return tr("输出范围");
                    case RemarkColumn:return tr("备注");
                    default:return QVariant();
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

bool CurveModel::insertRow(int row, const QModelIndex &parent)
{
    if (row >= _curve->size()) {
        return false;
    } else if (row < 0) {
        row = 0;
    }
    beginResetModel();
    _curve->insert(row, Curve::Cell(row));
    endResetModel();
    return true;
}

bool CurveModel::appendRow(int row, const QModelIndex &parent)
{
    if (row >= _curve->size()) {
        return false;
    } else if (row < 0) {
        row = 0;
    } else {
        row += 1;
    }
    beginResetModel();
    _curve->insert(row, Curve::Cell(row));
    endResetModel();
    return true;
}

bool CurveModel::removeRow(int row, const QModelIndex &parent)
{
    if (row < 0 || row >= _curve->size()) {
        return false;
    }
    beginRemoveRows(parent, row, row);
    _curve->remove(row);
    endRemoveRows();
    return true;
}
