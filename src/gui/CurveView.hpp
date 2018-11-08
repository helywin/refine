//
// Created by jiang.wenqiang on 2018/11/5.
//

#ifndef REFINE_CURVEVIEW_HPP
#define REFINE_CURVEVIEW_HPP

#include <QtWidgets/QTableView>

class CurveView : public QTableView
{
private:
    QTableView *_frozen;

public:
    CurveView(QAbstractItemModel *model,
                       QItemSelectionModel *selection,
                       QHeaderView *h, QHeaderView *v,
                       QWidget *parent = nullptr);
protected:
    void resizeEvent(QResizeEvent *event) override;
    QModelIndex moveCursor(CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers) override;
public:
    void scrollTo(const QModelIndex &index, ScrollHint hint) override;
private:
    void updateFrozenTableGeometry();

private slots:
    void updateSectionWidth(int index, int old_size, int new_size);
    void updateSectionHeight(int index, int old_size, int new_size);
};


#endif //REFINE_CURVEVIEW_HPP
