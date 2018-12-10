//
// Created by jiang.wenqiang on 2018/12/10.
//

#include <QtGui/QPainter>
#include "SpinInput.hpp"

SpinInput::SpinInput(int max, int min, QWidget *parent, int step, const QString &prefix,
                     const QString &suffix) :
        QAbstractItemDelegate(parent),
        _max(max),
        _min(min),
        _step(step),
        _prefix(prefix),
        _suffix(suffix)
{
    Q_ASSERT(_max >= _min);
}

QWidget *SpinInput::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    auto *edit = new QSpinBox(parent);
    edit->setMaximum(_max);
    edit->setMinimum(_min);
    edit->setSingleStep(_step);
    edit->setPrefix(_prefix);
    edit->setSuffix(_suffix);
    return edit;
}

void SpinInput::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    delete dynamic_cast<QSpinBox *>(editor);
}

void SpinInput::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    dynamic_cast<QSpinBox *>(editor)->setValue(index.data(Qt::EditRole).toInt());
}

void
SpinInput::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    model->setData(index,
                   dynamic_cast<QSpinBox *>(editor)->value(),
                   Qt::EditRole
    );
}

void SpinInput::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    dynamic_cast<QSpinBox *>(editor)->setFixedSize(option.rect.size());
    dynamic_cast<QSpinBox *>(editor)->move(option.rect.topLeft());
}

void SpinInput::paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    painter->drawText(option.rect.x() + 4,
                      option.rect.y() + (option.rect.height() / 2 + 5),
                      index.data().toString());
}

QSize SpinInput::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return option.rect.size();
}

