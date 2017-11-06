#include "mylistview.h"
#include "my_listmodel.h"
#include "friendlist.h"
#include "tool/friendinformation.h"
#include <QPoint>
#include <QLabel>
#include <QModelIndex>
#include "chatwindow.h"
#include "myitemdelegate.h"
using namespace std;
class FriendList;
MyListView::MyListView(QWidget *parent):QListView(parent)
{
    m_pModel=new My_ListModel(this);

    this->setModel(m_pModel);
    m_ActionMap=new QMap<QAction *, MyListView *>();

    connect(this, SIGNAL(clicked(QModelIndex)), FriendList::friendlist, SLOT(showChatWindow(QModelIndex)));  //绑定MyListView双击和chat窗口显示
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

MyListView::~MyListView()
{
    qDebug()<<"delete MyListView";
    delete m_ActionMap;
}

void MyListView::contextMenuEvent(QContextMenuEvent *event)  //鼠标右击事件
{
    int index = this->indexAt(event->pos()).row();
    if(index >-1){
     show_pmenu(event->pos());
    }
}

void MyListView::deleteItemSlot(bool bDelete)
{
    int index=this->currentIndex().row();
    if(index >-1){
        m_pModel->deleteLItem(index);
    }
}

void MyListView::setListMap(QMap<QString, MyListView *> *pListMap)
{
    m_pListMap=pListMap;
}

void MyListView::addItem(ListItemData *pItem)
{
    //m_pModel->addItem(pItem);
    this->addItemToTop(pItem);
}

void MyListView::addItemToTop(ListItemData *pItem)                //根据姓名插入到好友列表
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


}

void MyListView::moveSlot(bool bMove)
{
    QAction *pSender=qobject_cast<QAction *>(sender());
    if(pSender)
    {
        MyListView *pList=m_ActionMap->find(pSender).value();
        if(pList)
        {
            int index=this->currentIndex().row();
            ListItemData *pItem=m_pModel->getItem(index);
            pList->addItem(pItem);
            m_pModel->deleteLItem(index);
        }
    }
    m_ActionMap->clear();
}

void MyListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QListView::mouseDoubleClickEvent(event);
    if(event->button()==Qt::LeftButton)
    {
        int index = this->indexAt(event->pos()).row();

        if(index >-1){
        show_pmenu(event->pos());
        }
    }
}
void MyListView::show_pmenu(const QPoint &point)   //显示菜单
{
        QMenu pMenu;
        QAction *pDeleteAtc=new QAction(tr("删除"),&pMenu);
        pMenu.addAction(pDeleteAtc);

        connect(pDeleteAtc, SIGNAL(triggered(bool)), this, SLOT(deleteItemSlot(bool)));

        QMenu *pSubMenu=nullptr;

        QMap<QString,MyListView *>::iterator it=m_pListMap->begin();

        for(; it!=m_pListMap->end(); ++it)
        {
            if((!pSubMenu))
            {
                pSubMenu=new QMenu(tr("转移联系人至"),&pMenu);
                pMenu.addMenu(pSubMenu);
            }
            if(it.value()!=this)
            {
                QAction *pMoveAct=new QAction(it.key(),&pMenu);
                m_ActionMap->insert(pMoveAct, it.value());
                pSubMenu->addAction(pMoveAct);
                connect(pMoveAct, SIGNAL(triggered(bool)), this, SLOT(moveSlot(bool)));

            }
        }
        pMenu.addAction("好友信息", this, SLOT(showFriendInformation()));
        pMenu.exec(mapToGlobal(point));
}
void MyListView::showFriendInformation()
{
    ListItemData *friendMessage=m_pModel->getItem(currentIndex().row());
    FriendInformation *information=new FriendInformation(this);

    information->setFriendPhoto(QImage(friendMessage->iconPath));
    information->setFriendName(friendMessage->Name);
    information->setFriendStatus(XmppClient::swapStatustoString(friendMessage->status));
    XmppClient::FriendVcard vcad=friendMessage->vcard;

    {
    information->addFriendVcardInformation("姓名", vcad.username);
    information->addFriendVcardInformation("昵称", vcad.nickname);
    information->addFriendVcardInformation("Email 地址", vcad.email);
    information->addFriendVcardInformation("SIP账号或IP电话", vcad.VoIP);
    information->addFriendVcardInformation("职务", vcad.title);
    information->addFriendVcardInformation("部门", vcad.unit);
    information->addFriendVcardInformation("电话", vcad.phone);
    information->addFriendVcardInformation("移动电话", vcad.mobile);
    information->addFriendVcardInformation("传真", vcad.fax);
    }

    information->show();
}
