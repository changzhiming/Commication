#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include "myitem.h"
/*
 * 聊天标签视图显示类
 *
 */
class MyView : public QGraphicsView
{
    Q_OBJECT
public:
    MyView(QWidget *paren=0);
    ~MyView();
    /*
     * 增加项目到视图
     * return no
     * @param1 标签项类
     */
    void addMyItem(MyItem *item);
    /*
     * 使项目在可见区域整体左移一个标签的宽度
     * return no
     */
    void moveToleftOne();
    /*
     * 使项目在可见区域整体右一个标签的宽度
     * return no
     */
    void moveToRightOne();
    /*
     * 根据名字设置一个项目获取标签
     * return no
     * @param1 好友名字
     */
    void setFocusItem(ListItemData *data);
     void setFocusItem(RoomListData *data);
    /*
     * 重新计算场景矩形和项目位置
     */
    void againstCaculateScensRect();
protected:
    void resizeEvent(QResizeEvent *event);

    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
signals:
    /* 显示data对应的好友 */
    void showFriend(ListItemData *data);
    void showRoom(RoomListData *data);

    /* 隐藏data对应的好友 */
    void hideFriend(ListItemData *data);
    void hideRoom(RoomListData *data);

public:
    /* 存储添加到视图的MyItem
     */
    QList<MyItem *> itemList;
    /*
     * 每次移动的项目间隔宽度
     */
    int widthItem;
private:
    QGraphicsScene *Scene;


};

#endif // MYVIEW_H
