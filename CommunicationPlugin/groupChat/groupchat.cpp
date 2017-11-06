 #include "groupchat.h"
#include "ui_groupchat.h"
#include "listFriend/my_listmodel.h"
#include "listFriend/roomlistmodel.h"
#include "listFriend/roomlistview.h"
#include "friendlist.h"
#include "invitefrienddialog.h"
#include "core/core.h"
#include <QMessageBox>

GroupChat::GroupChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupChat)
{
    ui->setupUi(this);
    chatlog=new chatWindow(this);
    ui->splitter->insertWidget(0, chatlog);
    ui->splitter->setStretchFactor(0, 1);         //设置第一个窗体可以伸缩
    ui->listView->setListType(SearchListView::GroupFriend);
    inviteDailog=new InviteFriendDialog();

    connect(inviteDailog, &InviteFriendDialog::sendJidList, this, &GroupChat::inviteFriend);

}

GroupChat::~GroupChat()
{
    delete ui;
    delete inviteDailog;
}
void GroupChat::insertGroupList(QList<ListItemData *> groupList)
{
    foreach(ListItemData *data, groupList)
    {
        ui->listView->addItemToTop(data);
    }
}
void GroupChat::insertGroupMem(ListItemData *member)
{
    ui->listView->addItemToTop(member);
}

void GroupChat::setRoom(QXmppMucRoom *room)
{
    this->room=room;
    qDebug()<<room;

}
QXmppMucRoom *GroupChat::getRoom()
{
    return this->room;
}
SearchListView *GroupChat::getSearchListView()
{
    return ui->listView;
}
chatWindow * GroupChat::getChatLog()
{
    return chatlog;
}

void GroupChat::onCloseWindow()
{
    qDebug()<<"onCloseWindow";
}
void GroupChat::onSendFile()
{
    qDebug()<<"onSendFile";
}
void GroupChat::onSendMessage()
{
   qDebug()<<"onSendMessage";
}
void GroupChat::onShowHistoryMessage()
{
   qDebug()<<"onShowHistoryMessage";
}
void GroupChat::permissionsReceived(QList<QXmppMucItem> list)
{
    this->permissList=list;
}
QList<QXmppMucItem> GroupChat::getPermissions()
{
    return this->permissList;
}

void GroupChat::friendAddRoom(const QString &jid)
{
    qDebug()<<"sssssssssssssss"<<room<<room->participantFullJid((jid));
    if(room==nullptr)
        return;
    QRegExp regexp("(.*)(/)");
    int pos=regexp.indexIn(room->participantFullJid((jid)));
    if(pos>-1)
    {
        qDebug()<<regexp.cap(1);
        if(FriendList::friendlist->myData->Id==regexp.cap(1))
        {
            ui->listView->addItemToTop(FriendList::friendlist->myData);
        }
        if(FriendList::friendlist->friendList.value(regexp.cap(1)))
        {
            ListItemData *data= FriendList::friendlist->friendList.value(regexp.cap(1));
            data->GroupId=jid;

            ui->listView->addItemToTop(data);
        }
    }

}
void GroupChat::friendRemoveRoom(const QString &jid)
{
    qDebug()<<"friendremove"<<room;
    if(room==nullptr)
        return;
    QString Id;
    foreach(ListItemData *data, FriendList::friendlist->friendList.values())
    {
        if(data->GroupId==jid)
        {
            Id=data->Id;
        }
    }
    if(FriendList::friendlist->friendList.value(Id))
        {
            ListItemData *data= FriendList::friendlist->friendList.value(Id);

            ui->listView->m_pModel->deleteLItem(data);
        }

}
void GroupChat::kicked(const QString &jid, const QString &reason)
{
    if(room==nullptr)
        return;
    QRegExp regexp("(.*)(/)");
    int pos=regexp.indexIn(room->participantFullJid((jid)));
    if(pos>-1)
    {
        qDebug()<<regexp.cap(1);
        //pItem->friendList.append(regexp.cap(1));
        if(FriendList::friendlist->friendList.value(regexp.cap(1)))
        {
            ListItemData *data= FriendList::friendlist->friendList.value(regexp.cap(1));

            ui->listView->m_pModel->deleteLItem(data);
        }
    }
}
void GroupChat::allowedActionsChanged(QXmppMucRoom::Actions actions) const
{


}
void GroupChat::left()
{
    foreach(RoomListData *data ,FriendList::friendlist->roomListData)
    {
        if(data->groupChat==this)
        {
            FriendList::friendlist->roomListView->m_pModel->deleteLItem(data);
            FriendList::friendlist->roomListData.removeOne(data);
            delete data;
            Core::getInstance()->xmppClient->remove(data->room);

            data->groupChat->deleteLater();
            return;
        }
    }
}
void GroupChat::inviteFriend(QStringList list)
{
    foreach(QString jid, list)
    {
        qDebug()<<jid;
        room->sendInvitation(jid, inviteDailog->getReason());
    }
    inviteDailog->close();
}

void GroupChat::on_pushButton_clicked()  //邀请好友
{
    inviteDailog->setRoomId(room->jid());
    inviteDailog->show();
}
