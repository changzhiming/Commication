#include "roomlistview.h"
#include <QContextMenuEvent>
#include <QMenu>
#include "xmpp/xmppclient.h"
#include "friendlist.h"
#include "myitemdelegate.h"
#include "core/core.h"
RoomListView::RoomListView(QWidget *parent):QListView(parent)
{
    m_pModel=new RoomListModel(this);

    this->setModel(m_pModel);

    connect(this, SIGNAL(clicked(QModelIndex)), FriendList::friendlist, SLOT(showRoomChatWindow(QModelIndex)));  //绑定MyListView双击和chat窗口显示
    //键盘上下选择
    this->moveCursor(QAbstractItemView::MoveNext, Qt::KeypadModifier);
    //增加代理重新绘制文字图片
    MyItemDelegate *myitemdelegate=new MyItemDelegate(this);
    this->setItemDelegate(myitemdelegate);
    //设置选择时的颜色

    QPalette palette=this->palette();
    palette.setColor(QPalette::All, QPalette::Highlight, Qt::white);
    this->setPalette(palette);

    //设置没有边框
    this->setFrameShape(QFrame::NoFrame);
}

RoomListView::~RoomListView()
{
    qDebug()<<"delete RoomListView";
}

void RoomListView::addItem(RoomListData *pItem)
{
    if(m_pModel->m_ItemDataVec.contains(pItem))
    {
        m_pModel->m_ItemDataVec.removeOne(pItem);
    }
    m_pModel->addItem(pItem);
}
void RoomListView::contextMenuEvent(QContextMenuEvent *event)  //鼠标右击事件
{
    QListView::contextMenuEvent(event);
    int index = this->indexAt(event->pos()).row();
    if(index >-1){
     QMenu pMenu;
     pMenu.addAction(tr("删除群组"), this, SLOT(deleteItemSlot(bool)));
     pMenu.addAction("群组信息", this, SLOT(showRoomInformation()));
     pMenu.exec(event->globalPos());
    }
}

void RoomListView::deleteItemSlot(bool bDelete)
{
    int index=this->currentIndex().row();

    if(index >-1)
    {      
        RoomListData *roomData=m_pModel->getItem(index);
        Core::getInstance()->xmppClient->remove(roomData->room);
        m_pModel->deleteLItem(index);
    }
}

void RoomListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QListView::mouseDoubleClickEvent(event);
    if(event->button()==Qt::LeftButton)
    {

        int index=this->currentIndex().row();
        if(index >-1){
         QMenu pMenu;
         pMenu.addAction(tr("删除群组"), this, SLOT(deleteItemSlot(bool)));
         pMenu.addAction("群组信息", this, SLOT(showRoomInformation()));
         pMenu.exec(event->globalPos());
        }
    }
}
void RoomListView::showRoomInformation()
{

}

