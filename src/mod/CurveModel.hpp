//
// Created by jiang.wenqiang on 2018/11/2.
//

#ifndef REFINE_CURVEMODEL_HPP
#define REFINE_CURVEMODEL_HPP

#include <QtCore/QAbstractTableModel>
#include <QtGui/QStandardItemModel>


class Curve;

class CurveModel : public QAbstractTableModel
{
Q_OBJECT
public:
    enum Columns
    {
        ColumFirst = 0,
        NameColumn = ColumFirst,
        UnitColumn,
        WidthColumn,
        ColorColumn,
        CanIdColumn,
        ZeroByteColumn,
        HighByteColumn,
        LowByteColumn,
        RangeInColumn,
        RangeOutColumn,
        PrecisionColumn,
        RemarkColumn,
        ColumLast
    };
private:
    Curve *_curve;
public:
    explicit CurveModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index,
                 const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;
public slots:
    void genData(Curve *curve);

public:
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());

    bool appendRow(int row, const QModelIndex &parent = QModelIndex());

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
};


#endif //REFINE_CURVEMODEL_HPP
