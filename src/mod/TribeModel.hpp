//
// Created by jiang.wenqiang on 2018/11/6.
//

#ifndef REFINE_TRIBEMODEL_HPP
#define REFINE_TRIBEMODEL_HPP

#include <QtCore/QAbstractTableModel>

class Tribe;

class TribeModel : public QAbstractTableModel
{
Q_OBJECT
public:
    enum Columns
    {
        ColumFirst = 0,
        IndexColumn = ColumFirst,
        NameColumn,
        UnitColumn,
        WidthColumn,
        ColorColumn,
        RangeOutColumn,
        RemarkColumn,
        ColumLast = RemarkColumn
    };

private:
    Tribe *_tribe;

public:
    explicit TribeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index,
                 const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;

public slots:
    void genData(Tribe *tribe = nullptr);

signals:
    void tribeChanged();
};


#endif //REFINE_TRIBEMODEL_HPP
