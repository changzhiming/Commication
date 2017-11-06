#include "myitemdelegate.h"
#include "my_listmodel.h"
#include <QPainter>
#include <QDebug>
MyItemDelegate::MyItemDelegate(QObject *parent):QItemDelegate(parent)
{

}

MyItemDelegate::~MyItemDelegate()
{

}


void MyItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    //显示装太
    XmppClient::StatusType Userstatus = (XmppClient::StatusType)index.data(Qt::UserRole+1).toInt();

    painter->setPen(Qt::gray);
    QRect rect = option.rect;
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    painter->restore();
    painter->save();

    //显示图片
    QSize painsize=QSize(option.rect.height(), option.rect.height())-QSize(10, 10);

    QVariant value = index.data(Qt::UserRole+2);
    QPixmap pixmap;
    if(value.type() == QVariant::Icon)
    {
        QIcon icon = qvariant_cast<QIcon>(value);
        if(Userstatus!=XmppClient::Unavailable)
        {
            pixmap = icon.pixmap(painsize, QIcon::Active, QIcon::On);
        }
        else
        {
            pixmap = icon.pixmap(painsize, QIcon::Disabled, QIcon::Off);
        }
    }
    rect = option.rect;

    rect.setWidth(pixmap.width());
    rect.setHeight(pixmap.height());
    rect.moveTop(rect.y() + (option.rect.height() - pixmap.height())/2);
    rect.moveLeft(rect.left() +10);
    painter->drawPixmap(rect, pixmap);

    QRect saverect=rect;
    //显示装太
    if(Userstatus>=0)
    {
    QPixmap pixmap1(My_ListModel::getIconPath(Userstatus));
    pixmap1=pixmap1.scaled(painsize.width()/3, painsize.height()/3);
    //rect = option.rect;

    rect.setWidth(pixmap1.width());
    rect.setHeight(pixmap1.height());
    rect.moveTop(rect.y() + (pixmap.height() - pixmap1.height()));
    rect.moveLeft(rect.left() +pixmap.width()-pixmap1.width());
    painter->drawPixmap(rect, pixmap1);
    }

    //显示聊天个数
    rect = saverect;
    if(!index.data(Qt::UserRole+3).toString().isEmpty())
    {
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        rect.setWidth(10);
        rect.setHeight(10);
        rect.moveLeft(rect.left()+pixmap.width()-10);
        painter->drawEllipse(rect);
        painter->setPen(Qt::white);
        QFont font;
        font.setPixelSize(9);
        font.setBold(true);
        painter->setFont(font);
        painter->drawText(rect, Qt::AlignCenter, index.data(Qt::UserRole+3).toString());

    }

    painter->restore();
    painter->save();
    //显示姓名
    rect = option.rect;
    rect.setLeft(rect.x()+pixmap.width()+20);
    //rect.moveTop(rect.y()+5);
    painter->setPen(Qt::black);

    if(!index.data(Qt::UserRole).toString().isEmpty())
    {
         painter->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, index.data(Qt::UserRole).toString());
    }

    painter->restore();

}

