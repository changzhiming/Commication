#include "noticeitemdelegate.h"
#include <QPainter>
#include <QDebug>
NoticeItemDelegate::NoticeItemDelegate(QObject *parent ):QItemDelegate(parent)
{

}

NoticeItemDelegate::~NoticeItemDelegate()
{

}
void NoticeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    //显示标题
    QRect rect = option.rect;
    painter->setPen(QPen(Qt::gray));
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());

    if(!index.data(Qt::UserRole).toString().isEmpty()){
        painter->setPen(Qt::black);
        rect.setHeight(20);
        QFont font(QStringLiteral("宋体"), 14, QFont::Bold, false);
        painter->setFont(font);
        painter->drawText(rect, Qt::AlignCenter, index.data(Qt::UserRole).toString());
    }
    //显示 Icon
    rect = option.rect;
    QVariant value = index.data(Qt::UserRole+2);
    QPixmap pixmap;
    if(value.type() == QVariant::Icon) {
        QIcon icon = qvariant_cast<QIcon>(value);
        pixmap = icon.pixmap(QSize(40, 40), QIcon::Active, QIcon::On);
    }

    if(!pixmap.isNull()) {
        rect.setSize(QSize(40, 40));
        rect.moveTop(rect.y()+30);
        rect.moveLeft(rect.x() +20);
        painter->drawPixmap(rect, pixmap);
    }
    /*  show datetime */
    rect = option.rect;
    if(!index.data(Qt::UserRole+1).toString().isEmpty()){
        rect.setWidth(rect.width()-60);
        rect.setHeight(20);
        rect.moveLeft(rect.left() +60);
        rect.moveTop(rect.top() +30);
        painter->setPen(Qt::black);
        QFont font(QStringLiteral("宋体"), 9);
        painter->setFont(font);
        painter->drawText(rect, Qt::AlignRight, index.data(Qt::UserRole+1).toString());
        //qDebug()<<index.row()<<index.data(Qt::UserRole+1).toString()<<rect<<"unit ";
    }
    /*  show datetime */
    rect = option.rect;
    if(!index.data(Qt::UserRole+3).toString().isEmpty()){
        rect.setWidth(rect.width()-60);
        rect.setHeight(20);
        rect.moveLeft(rect.left()+ 55);
        rect.moveTop(rect.top()+60);
        painter->setPen(Qt::black);

        QFont font(QStringLiteral("宋体"), 12, QFont::Bold);
        painter->setFont(font);
        painter->drawText(rect, Qt::AlignRight, index.data(Qt::UserRole+3).toString());

    }
    painter->restore();
}
