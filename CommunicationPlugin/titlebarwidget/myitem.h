#ifndef MYITEM_H
#define MYITEM_H
#include <QGraphicsItem>
#include <QSize>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QLabel>
#include "listFriend/my_listmodel.h"
#include "listFriend/roomlistmodel.h"
class ChatLog;
class LabelThumbnail;
/*
 * 聊天标签类
 * 用来显示最近消息和实现聊天窗体之间跳转
 * 显示好友最近聊天缩略图·
 */
class MyItem : public QGraphicsItem
{

public:
    MyItem(ListItemData *data);
    MyItem(RoomListData *data);
    ~MyItem();
    QRectF boundingRect() const;

public:

    void setTextHeml(QString html=QString());
    ListItemData * getItemData();
    RoomListData *getRoomItemData();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
private:
    QTextDocument doc;          /* 格式化绘制文本内容 */
public:
    static QSize itemSize;
    ListItemData *bindingData=nullptr;
    RoomListData *bindingRoomDta=nullptr;
    LabelThumbnail *label=nullptr;


};

#endif // MYITEM_H
