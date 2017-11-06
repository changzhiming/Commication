#include "myitem.h"
#include <QPainter>
#include <QStyle>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include "chatlog/chatlog.h"
#include "friendlist.h"
#include "listFriend/my_listmodel.h"
#include "tool/labelthumbnail.h"
#include <QGraphicsSceneHoverEvent>
#include <QCursor>
QSize MyItem::itemSize=QSize(170, 80);

MyItem::MyItem(ListItemData *data):QGraphicsItem()
{
    bindingData=data;
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    label=new LabelThumbnail();

}
MyItem::MyItem(RoomListData *data):QGraphicsItem()
{
    bindingRoomDta=data;
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    label=new LabelThumbnail();
}

MyItem::~MyItem()
{
    label->deleteLater();
}

QRectF MyItem::boundingRect() const
{
    qreal penWidth=1;
    //return QRectF(0-penWidth/2, 0-penWidth/2, itemSize.width()+penWidth, itemSize.height() +penWidth);
    return QRectF(0, 0, itemSize.width(), itemSize.height());
}
ListItemData *MyItem::getItemData()
{
    return bindingData;
}
RoomListData *MyItem::getRoomItemData()
{
    return this->bindingRoomDta;
}

void MyItem::setTextHeml(QString html)
{
    //if(doc.size())
    doc.setHtml(html);
    doc.setTextWidth(this->boundingRect().width());
    doc.documentLayout()->update();
}

void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QAbstractTextDocumentLayout::PaintContext ctx;

    if(option->state & QStyle::State_Selected)
    {

        painter->setBrush(QColor(0xf6, 0xfc, 0xff));
        painter->setPen(QColor(0xcc, 0xe5, 0xf5));
        painter->drawRect(0, 0, itemSize.width(), itemSize.height());
        painter->setPen(QColor(0xf6, 0xfc, 0xff));

        painter->drawLine(QPointF(this->boundingRect().bottomLeft())+=QPointF(0, 1), QPointF(this->boundingRect().bottomRight())+=QPointF(0, 1));
    }
    else
    {
        painter->setBrush(QColor(0xe9, 0xf6, 0xff));
        painter->setPen(QColor(0xd0, 0xe8, 0xf6));
        painter->drawRect(0, 0, itemSize.width(), itemSize.height());

    }

    painter->restore();
    painter->save();
    QImage pixmap(MyItem::itemSize, QImage::Format_ARGB32_Premultiplied);
    if(bindingData)
    {
        pixmap.load(bindingData->iconPath);
        pixmap=pixmap.scaled(QSize(40, 40), Qt::KeepAspectRatio);
    }else
    {
        pixmap.load(bindingRoomDta->iconPath);
        pixmap=pixmap.scaled(QSize(40, 40), Qt::KeepAspectRatio);
    }
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    QRectF rect=this->boundingRect();
    rect.setSize(pixmap.size());
    rect.moveLeft(rect.x()+10);
    rect.moveTop(rect.y()+(this->boundingRect().height()-pixmap.height())/2);
    painter->drawImage(rect, pixmap);


    QFontMetrics metrics = QFontMetrics(QFont("宋体", 9 , QFont::Bold));         //设置字体的宽度和大小
    //qDebug()<<this->boundingRect().bottomLeft();
    QString elidedText = metrics.elidedText(bindingData?(bindingData->Name):(bindingRoomDta->RoomName), Qt::ElideRight, this->boundingRect().width()-pixmap.width()-10); // 太长添加省略号
    painter->setFont(QFont("宋体", 9 , QFont::Bold));
    rect=this->boundingRect();
    rect.setLeft(rect.x()+20+pixmap.width());
    painter->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, elidedText);

    painter->restore();


    ctx.palette.setColor(QPalette::Text, QColor(0,0,0, 80));
    painter->setClipRect(this->boundingRect());
    doc.documentLayout()->draw(painter, ctx);

    Q_UNUSED(option)
    Q_UNUSED(widget)

}

void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}
void MyItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

    QPixmap pix;
    if(bindingData)
    {
    QPixmap pixmap(bindingData->chatwindow->getChatLog()->width()/2, bindingData->chatwindow->getChatLog()->height()/2);
    QPainter painter(&pixmap);

    bindingData->chatwindow->getChatLog()->render(&painter);
    painter.end();
    pix=pixmap;
    }
    if(bindingRoomDta)
    {

        QPixmap pixmap(bindingRoomDta->groupChat->getChatLog()->getChatLog()->width()/2, bindingRoomDta->groupChat->getChatLog()->getChatLog()->height()/2);
        QPainter painter(&pixmap);

        bindingRoomDta->groupChat->getChatLog()->getChatLog()->render(&painter);
        painter.end();
        pix=pixmap;
    }

   // if(!label->pixmap()->isNull()&&label->pixmap()->size()!=pixmap.size())
    {
       // pixmap=this->chatlog->grab();
    }

    //pixmap=pixmap.scaled(pixmap.width()/2, pixmap.height()/2, Qt::KeepAspectRatio);

    label->setPixmap(pix);
    label->move(QCursor::pos());
    label->show();

    QGraphicsItem::hoverEnterEvent(event);
}
void MyItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    label->hide();
    QGraphicsItem::hoverLeaveEvent(event);

}
