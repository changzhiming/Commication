#ifndef ROOMLISTVIEW_H
#define ROOMLISTVIEW_H
#include "roomlistmodel.h"
#include "QListView"

class RoomListView : public QListView
{
    Q_OBJECT
public:
    RoomListView(QWidget *parent=nullptr);
    ~RoomListView();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    //void moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);

public slots:
    void deleteItemSlot(bool bDelete);
    void showRoomInformation();

    void addItem(RoomListData *pItem);

public:
     RoomListModel *m_pModel;
};

#endif // ROOMLISTVIEW_H
