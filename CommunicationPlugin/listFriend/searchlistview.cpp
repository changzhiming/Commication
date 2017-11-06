#include "searchlistview.h"
#include "my_listmodel.h"
#include "friendlist.h"
#include <QPoint>
#include "chatwindow.h"

#include <QMouseEvent>
#include "myitemdelegate.h"
#include "groupChat/groupchat.h"
#include "xmpp/xmppclient.h"
using namespace std;
class FriendList;
SearchListView::SearchListView(QWidget *parent) : QListView(parent)
{
    m_pModel=new My_ListModel(this);
    this->setModel(m_pModel);
    //connect(this, SIGNAL(clicked(QModelIndex)), FriendList::friendlist, SLOT(showChatWindow(QModelIndex)));  //绑定MyListView双击和chat窗口显示
    this->moveCursor(QAbstractItemView::MoveNext, Qt::KeypadModifier);

    MyItemDelegate *myitemdelegate=new MyItemDelegate(this);
    this->setItemDelegate(myitemdelegate);
    //设置选择时的颜色
    QPalette palette=this->palette();
    palette.setColor(QPalette::All, QPalette::Highlight, Qt::white);
    //this->setPalette(palette);
    //设置没有边框
    this->setFrameShape(QFrame::NoFrame);

}

SearchListView::~SearchListView()
{
    qDebug()<<"delete searchListView";
    delete m_pModel;
}

void SearchListView::addItem(ListItemData *pItem)
{

    if(m_pModel->m_ItemDataVec.contains(pItem))
    {
        m_pModel->m_ItemDataVec.removeOne(pItem);
    }
    m_pModel->addItem(pItem);

}
void SearchListView::addItemToTop(ListItemData *pItem)
{
    if(m_pModel->m_ItemDataVec.contains(pItem))
    {
        m_pModel->m_ItemDataVec.removeOne(pItem);
    }

    m_pModel->addItem(pItem);

    for(int i=1; i<m_pModel->m_ItemDataVec.count(); i++)
    {
           for(int j=i; j>0; j--)
           {
               ListItemData *data;

               if(m_pModel->m_ItemDataVec.at(j-1)->status>m_pModel->m_ItemDataVec.at(j)->status)
               {
                    data=m_pModel->m_ItemDataVec.at(j);
                    m_pModel->m_ItemDataVec[j]=m_pModel->m_ItemDataVec[j-1];
                    m_pModel->m_ItemDataVec[j-1]=data;
               }
           }
     }
     for(int i=1; i<m_pModel->m_ItemDataVec.count(); i++)
     {
        for(int j=i; j>0; j--)
        {
            ListItemData *data;

            if((m_pModel->m_ItemDataVec.at(j-1)->status==m_pModel->m_ItemDataVec.at(j)->status)&&(m_pModel->m_ItemDataVec[j-1]->Id>m_pModel->m_ItemDataVec[j]->Id))
            {
                    data=m_pModel->m_ItemDataVec.at(j);
                    m_pModel->m_ItemDataVec[j]=m_pModel->m_ItemDataVec[j-1];
                    m_pModel->m_ItemDataVec[j-1]=data;
            }
        }
     }
     this->update();
}
void SearchListView::setListType(SearchListView::ListType type)
{
    this->type=type;
}

bool SearchListView::ban()
{
    QXmppMucRoom *room;
    int index=this->currentIndex().row();
    if(index >-1)
    {
        ListItemData *listData=m_pModel->getItem(index);
        foreach(RoomListData *data, FriendList::friendlist->roomListData)
        {
            if(data->groupChat->getSearchListView()==this)
            {
                room=data->room;
                break;
            }
        }
        if(room!=nullptr)
        {
            QRegExp regexp("(.*)(/)");
            int pos=regexp.indexIn(room->participantFullJid(listData->GroupId));
            if(pos>-1)
            {
                return room->ban(regexp.cap(1), " ");
            }
        }
        else
        {
            return false;
        }
    }
    return false;
}
bool SearchListView::kick()
{
    qDebug()<<"11111111111";
    QXmppMucRoom *room;
    int index=this->currentIndex().row();
    if(index >-1)
    {
        ListItemData *listData=m_pModel->getItem(index);

        foreach(RoomListData *data, FriendList::friendlist->roomListData)
        {
            if(data->groupChat->getSearchListView()==this)
            {
                room=data->room;
                break;
            }
        }
        if(room!=nullptr)
        {
            return room->kick(listData->GroupId, " ");
        }else
        {
            return false;
        }
    }
    return false;
}
bool SearchListView::leave()
{
    QXmppMucRoom *room;
    int index=this->currentIndex().row();
    if(index >-1)
    {
        foreach(RoomListData *data, FriendList::friendlist->roomListData)
        {
            if(data->groupChat->getSearchListView()==this)
            {
                room=data->room;
                break;
            }
        }
        if(room!=nullptr)
        {
            return room->leave(" ");
        }else
        {
            return false;
        }
    }
    return false;
}
void SearchListView::removeFromBan()
{
    QAction * action=qobject_cast<QAction *>(sender());

    if(action)
    {
        QXmppMucRoom *room;
        foreach(RoomListData *data, FriendList::friendlist->roomListData)
        {
            if(data->groupChat->getSearchListView()==this)
            {
                room=data->room;
                room->removeBan(action->text());
                break;
            }
        }
    }
}

void SearchListView::contextMenuEvent(QContextMenuEvent *event)  //鼠标右击事件
{

    QListView::contextMenuEvent(event);
    if(this->type==SearchListView::GroupFriend)
    {
        QXmppMucRoom *room;
        foreach(RoomListData *data, FriendList::friendlist->roomListData)
        {
            if(data->groupChat->getSearchListView()==this)
            {
                room=data->room;
                if(room!=nullptr)
                {
                    room->requestPermissions();
                }
                else
                {
                    return;
                }
                int index = this->indexAt(event->pos()).row();

                if(index >-1)
                {
                    ListItemData *listData=m_pModel->getItem(index);
                    QMenu pMenu;
                    if(listData!=FriendList::friendlist->myData)
                    {
                         pMenu.addAction(tr("踢出"), this, SLOT(kick()));
                         pMenu.addAction(tr("加入黑名单"), this, SLOT(ban()));

                    }
                    QMenu *subMenu=pMenu.addMenu(tr("移出黑名单"));
                    foreach(QXmppMucItem item, data->groupChat->getPermissions())
                    {
                        if(item.affiliation()==QXmppMucItem::OutcastAffiliation)
                        {
                            //subMenu->addAction(item.affiliationToString(item.affiliation()));
                            subMenu->addAction(item.jid(), this, SLOT(removeFromBan()));
                        }

                    }
                    pMenu.addAction(tr("离开"), this, SLOT(leave()));
                    pMenu.exec(event->globalPos());
                    break;
                }
            }
        }

    }
}
