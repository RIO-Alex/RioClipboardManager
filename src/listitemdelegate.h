#ifndef LISTITEMDELEGATE_H
#define LISTITEMDELEGATE_H

#include <QStyledItemDelegate>

class ListItemDelegate : public QStyledItemDelegate
{
public:
    ListItemDelegate();

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // LISTITEMDELEGATE_H
