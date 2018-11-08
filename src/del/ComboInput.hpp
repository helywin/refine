//
// Created by jiang.wenqiang on 2018/11/8.
//

#ifndef REFINE_UNITINPUT_HPP
#define REFINE_UNITINPUT_HPP

#include <QtWidgets/QAbstractItemDelegate>
#include <QtWidgets/QComboBox>

class ComboInput : public QAbstractItemDelegate
{
Q_OBJECT
private:
    bool _editable;
    QStringList _content;
public:
    explicit ComboInput(bool editable, const QStringList &content,
                        QWidget *parent = nullptr);
    explicit ComboInput(bool editable, QStringList &&content,
                        QWidget *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void destroyEditor(QWidget *editor,
                       const QModelIndex &index) const override;
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};


#endif //REFINE_UNITINPUT_HPP
