//
// Created by jiang.wenqiang on 2018/11/5.
//

#ifndef REFINE_CURVEVIEW_HPP
#define REFINE_CURVEVIEW_HPP

#include <QtWidgets/QTableView>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>

class CurveView : public QTableView
{
private:
    QMenu *_menu_edit;
    QTableView *_frozen;

public:
    CurveView(QAbstractItemModel *model,
              QItemSelectionModel *selection,
              QHeaderView *h, QHeaderView *v,
              QWidget *parent = nullptr);
    void scrollTo(const QModelIndex &index, ScrollHint hint) override;

    void setRightMenu(QMenu *menu);

public slots:
    inline void setFroze(bool froze) { _frozen->setVisible(froze); };

protected:
    void resizeEvent(QResizeEvent *event) override;
    QModelIndex moveCursor(CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers) override;
private:
    void setup();

    void updateFrozenTableGeometry();

private slots:
    void updateSectionWidth(int index, int old_size, int new_size);
    void updateSectionHeight(int index, int old_size, int new_size);
    void popRightMenu(const QPoint &pos);
};


#endif //REFINE_CURVEVIEW_HPP
