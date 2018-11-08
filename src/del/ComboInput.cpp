//
// Created by jiang.wenqiang on 2018/11/8.
//

#include <QtGui/QPainter>
#include "ComboInput.hpp"

ComboInput::ComboInput(bool editable, const QStringList &content,
                       QWidget *parent) :
        _editable(editable),
        _content(content),
        QAbstractItemDelegate(parent) {}

ComboInput::ComboInput(bool editable, QStringList &&content,
                       QWidget *parent) :
        _editable(editable),
        _content(content),
        QAbstractItemDelegate(parent) {}

QWidget *ComboInput::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    auto *editor = new QComboBox(parent);
    editor->setEditable(_editable);
    if (!_content.isEmpty()) {
        editor->addItems(_content);
        editor->setCurrentIndex(0);
    }
    return editor;
}

void ComboInput::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    delete dynamic_cast<QComboBox *>(editor);
}

void ComboInput::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString s = index.data(Qt::EditRole).toString();
    if (_content.contains(s)) {
        dynamic_cast<QComboBox *>(editor)
                ->setCurrentIndex(_content.indexOf(s));
    } else {
        dynamic_cast<QComboBox *>(editor)
                ->setCurrentText(s);
    }

}

void ComboInput::setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const
{
    model->setData(index,
                   dynamic_cast<QComboBox *>(editor)
                           ->currentText(),
                   Qt::EditRole);
}

void ComboInput::updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    dynamic_cast<QComboBox *>(editor)->setFixedSize(option.rect.size());
    dynamic_cast<QComboBox *>(editor)->move(option.rect.topLeft());
}

void ComboInput::paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    painter->drawText(option.rect.x() + 4,
                      option.rect.y() + (option.rect.height() / 2 + 5),
                      index.data().toString());
}

QSize ComboInput::sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    return QSize();
}
