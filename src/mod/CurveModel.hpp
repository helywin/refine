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
        TypeColumn,
        UnitColumn,
        WidthColumn,
        ColorColumn,
        CanIdColumn,
        ZeroByteColumn,
        HighByteColumn,
        LowByteColumn,
        FrameMsecColumn,
        RangeInColumn,
        RangeOutColumn,
        RemarkColumn,
        ColumLast = RemarkColumn
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

public slots:
    void genData(Curve *curve);
};


#endif //REFINE_CURVEMODEL_HPP
