//
// Created by jiang.wenqiang on 2018/11/6.
//

#ifndef REFINE_TRIBEVIEW_HPP
#define REFINE_TRIBEVIEW_HPP

#include <QtWidgets/QTableView>

class TribeView : public QTableView
{
Q_OBJECT
public:
    explicit TribeView(QAbstractItemModel *model,
                       QItemSelectionModel *selection,
                       QHeaderView *h, QHeaderView *v,
                       QWidget *parent = nullptr);
};


#endif //REFINE_TRIBEVIEW_HPP
