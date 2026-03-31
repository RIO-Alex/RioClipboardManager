#include "listitemdelegate.h"
#include <QPainter>
#include "clipboardmodel.h"

ListItemDelegate::ListItemDelegate() {}

QSize ListItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(option.rect.width(), 50);
}

void ListItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();

    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::Antialiasing);

    option.widget->style()->drawControl(QStyle::CE_ItemViewItem, &option, painter, option.widget);

    QString MainText = index.data(ClipboardModel::PreviewRole).toString();
    QString TimeText = index.data(ClipboardModel::TimeRole).toString();

    QFont mainFont = option.font;
    mainFont.setBold(true);
    mainFont.setPixelSize(14);
    mainFont.setStyleStrategy(QFont::PreferAntialias);

    QFont timeFont = option.font;
    timeFont.setPixelSize(12);

    QRect MainRect = option.rect.adjusted(10, 5, -10, -25);
    QRect TimeRect = option.rect.adjusted(10, 30, -10, -5);

    painter->setFont(mainFont);
    painter->drawText(MainRect, Qt::AlignLeft | Qt::AlignTop, MainText);

    painter->setFont(timeFont);
    QColor TextColor = option.palette.color(QPalette::PlaceholderText);
    painter->setPen(TextColor);
    painter->drawText(TimeRect, Qt::AlignLeft | Qt::AlignBottom, TimeText);

    painter->restore();
}