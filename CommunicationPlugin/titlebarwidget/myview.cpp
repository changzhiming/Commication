#include "myview.h"
#include "myitem.h"
#include <QDebug>
#include <QScrollBar>
#include <QDateTime>
MyView::MyView(QWidget *parent): QGraphicsView(parent)
{
    Scene=new QGraphicsScene(this);
    widthItem=MyItem::itemSize.width()+32;
    qDebug()<<widthItem;

    setScene(Scene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFrameShape(QFrame::NoFrame);

}

MyView::~MyView()
{
    foreach (MyItem *item, itemList) {
        qDebug()<<"delete item";
        delete item;
    }
}
void MyView::addMyItem(MyItem *item)
{
    item->setPos(widthItem*itemList.size(), 0);
    Scene->addItem(item);
    itemList.append(item);
}

void MyView::resizeEvent(QResizeEvent *event)
{
    //qDebug()<<"viewport"<<viewport()->rect().bottomLeft();

    QGraphicsView::resizeEvent(event);
    setSceneRect(sceneRect().x(), sceneRect().y(), viewport()->rect().width(), MyItem::itemSize.height());
}

void MyView::moveToleftOne()
{
    if(this->sceneRect().x()>0)
    {
        this->setSceneRect(this->sceneRect().x()-widthItem, this->sceneRect().y(), this->sceneRect().width(), this->sceneRect().height()-2);
    }
}
void MyView::moveToRightOne()
{
    if(this->sceneRect().x()+this->sceneRect().width()<this->scene()->items().size()*widthItem-32)
    {
        qDebug()<<this->sceneRect().x()+this->sceneRect().width()<<this->scene()->items().size()*widthItem;
        this->setSceneRect(this ->sceneRect().x()+ widthItem, this->sceneRect().y(), this->sceneRect().width(), this->sceneRect().height());
    }
}
void MyView::setFocusItem(ListItemData *data)
{

    foreach (MyItem *item, itemList) {
        if(item->bindingData==data){
            this->scene()->clearSelection();
            item->setSelected(true);

            QPointF pointf=item->mapToScene(0, 0);
            qDebug()<<sceneRect()<<item->mapToScene(item->boundingRect().toRect()).boundingRect();
            if(sceneRect().contains(item->mapToScene(item->boundingRect().toRect()).boundingRect()))
            {
                return;
            }

            if(pointf.x()+this->sceneRect().width()>this->scene()->items().size()*widthItem-32)
            {
                pointf.setX(this->scene()->items().size()*widthItem-this->sceneRect().width());
            }
            this->setSceneRect(pointf.x(), this->sceneRect().y(), this->sceneRect().width(), this->sceneRect().height());
            break;
        }

    }
}
void MyView::setFocusItem(RoomListData *data)
{
    foreach (MyItem *item, itemList) {
        if(item->bindingRoomDta==data){
            this->scene()->clearSelection();
            item->setSelected(true);

            QPointF pointf=item->mapToScene(0, 0);
            qDebug()<<sceneRect()<<item->mapToScene(item->boundingRect().toRect()).boundingRect();
            if(sceneRect().contains(item->mapToScene(item->boundingRect().toRect()).boundingRect()))
            {
                return;
            }

            if(pointf.x()+this->sceneRect().width()>this->scene()->items().size()*widthItem-32)
            {
                pointf.setX(this->scene()->items().size()*widthItem-this->sceneRect().width());
            }
            this->setSceneRect(pointf.x(), this->sceneRect().y(), this->sceneRect().width(), this->sceneRect().height());
            break;
        }

    }
}

void MyView::againstCaculateScensRect()
{
    for(int i=0; i<itemList.count(); i++)
    {
        itemList[i]->setPos(widthItem*i, 0);
    }
}

void MyView::paintEvent(QPaintEvent *event)
{

    QPainter painter(this->viewport());

    //painter.drawLine(0, 0, 1, 1);

    QGraphicsView::paintEvent(event);
}
/*
 * 鼠标双击发送隐藏相应聊天窗口信号
 */
void MyView::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
    MyItem *item;
    if((item=static_cast<MyItem *>(Scene->itemAt(mapToScene(ev->pos()), QTransform()))))
    {
        if(item->bindingData)
            emit hideFriend(item->bindingData);
        if(item->bindingRoomDta)
            emit hideRoom(item->bindingRoomDta);
    }
    if(scene())
    {
        scene()->removeItem(item);
        int index=itemList.indexOf(item);
        itemList.removeOne(item);
        if(index>0)
        {
            setFocusItem(itemList[index-1]->getItemData());
        }
        else
        {
            if(itemList.size())
            {
                setFocusItem(itemList[index]->getItemData());
            }
        }
        againstCaculateScensRect(); /* 重新排列场景 */
        delete item;
    }
    }
    QGraphicsView::mouseDoubleClickEvent(ev);
}
/*
 * 鼠标按压发送显示相应聊天窗口信号
 */
void MyView::mousePressEvent(QMouseEvent *ev)
{
    static QDateTime datetime=QDateTime::currentDateTime();
    if(QDateTime::currentDateTime().toMSecsSinceEpoch()-datetime.toMSecsSinceEpoch()>800)
    {

    if(ev->button()==Qt::LeftButton)
    {
        MyItem *item;
        if((item=static_cast<MyItem *>(Scene->itemAt(mapToScene(ev->pos()), QTransform()))))
        {
            if(item->bindingData)
                emit showFriend(item->bindingData);
            if(item->bindingRoomDta)
                emit showRoom(item->bindingRoomDta);
        }
    }
    datetime=QDateTime::currentDateTime();
    QGraphicsView::mousePressEvent(ev);
    }
}
