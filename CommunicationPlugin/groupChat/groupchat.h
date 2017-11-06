#ifndef GROUPCHAT_H
#define GROUPCHAT_H
#include "chatwindow.h"
#include "xmpp/xmppclient.h"
#include <QWidget>

namespace Ui {
class GroupChat;
}
class SearchListView;
struct ListItemData;
class InviteFriendDialog;
class GroupChat : public QWidget
{
    Q_OBJECT

public:
    explicit GroupChat(QWidget *parent = 0);
    ~GroupChat();

    void insertGroupList(QList<ListItemData *> groupList);
    void insertGroupMem(ListItemData * member);
    void setRoom(QXmppMucRoom *room);
    QXmppMucRoom *getRoom();
    SearchListView * getSearchListView();
    chatWindow *getChatLog();
public slots:

    void onCloseWindow();
    void onSendFile();
    void onSendMessage();
    void onShowHistoryMessage();
    /* 获取所有好友权限 */
    void permissionsReceived(QList<QXmppMucItem> list);
    QList<QXmppMucItem > getPermissions();
    /* 增加好友， 删除好友 */
    void friendAddRoom(const QString &jid);
    void friendRemoveRoom(const QString &jid);
    /* 自身权限发生变化 */
    void allowedActionsChanged(QXmppMucRoom::Actions actions) const;
    //被踢/// This signal is emitted if you get kicked from the room.
    void kicked(const QString &jid, const QString &reason);

   // 退出/// This signal is emitted once you have left the room.
    void left();
    /* 邀请好友*/
    void inviteFriend(QStringList list);

private slots:
    void on_pushButton_clicked();

private:
    Ui::GroupChat *ui;
    chatWindow *chatlog=nullptr;
    QXmppMucRoom *room=nullptr;
    QList<QXmppMucItem > permissList;//权限列表
    InviteFriendDialog *inviteDailog;
};

#endif // GROUPCHAT_H
