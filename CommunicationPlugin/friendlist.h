#ifndef FRIENDLIST_H
#define FRIENDLIST_H
#include <QMenu>
#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QLineEdit>
#include <QLayout>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QModelIndex>
#include <QMoveEvent>
#include "xmpp/xmppclient.h"
#include "mainmenustackedwidget.h"
#include "listFriend/roomlistmodel.h"
#include <QHash>
#include <QPointer>

class Core;
class MyListView;
struct ListItemData;
class chatWindow;
class AddFriend;
class AddGroup;
class SettingUi;
class SearchListView;
class Message;
class Transfer;
class FaceWidget;
class FunctionChoiceWidget;
class TitleBarWidget;
class RoomListView;
class FileTransferWidget;
class QQPanel;
class SaveSetting;
class keyboard;
class NewestMessageFrame;

namespace Ui {
class FriendList;
}

class FriendList : public QWidget
{
    Q_OBJECT

public:
    explicit FriendList(QWidget *parent = 0, Qt::WindowFlags f=0);
    ~FriendList();
    QString getStatusIconPath(XmppClient::StatusType status);    //获取相应状态的图标
    void init_window(QLayout *layout, QWidget *win, QString s, bool show_hide=true); //把win加入layout里win名字是s默认显示
    inline void saveWindowGeometry();                    //保存主窗体大小和位置
    void loadLocalFriend(QHash<QString, ListItemData *> friendList);               //加载本地存储的好友 信息
    void loadLocalRecentContacts(QList<QString> recentContacts);           //加载本地存储的recent contacts好友 信息

signals:
    void insertMessageToDb(historyMessage *message);
    /* invite into room */
    void addRoom(const QString &roomJid,const QString &Subject,const QString &password);
private slots:

    void receiveFile(QXmppTransferJob *job);                  //接收文件
    void sendFileJob(QXmppTransferJob *job);                  //发送文件job接收
    void receiveMessage(const FriendDataMessage message);     //接收消息

    void on_lineEdit_returnPressed();   //输入框按回车
    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);      //光标位置改变
    void on_toolButton_clicked();
    void friendlistReceived(QList<XmppClient::FriendItem> list);                 //好友列表接收
    void vcardReceived(QString bareJid, XmppClient::FriendVcard Vcard);    //vcard获取完毕信号，此时可以执行getfriendItem()方法获取vcard信息
    void presenstate(QString bareJid, XmppClient::StatusType state);         //好友状态更新状态

    void onUserAwayCheck();                   //用户离开检测
    void connectTitleBat();                   //关联好友列表上方标题栏
    /* 群组 */
    void onRoomReciver(QXmppMucRoom *room);
    void Groupinvitation(const QString &roomJid, const QString &inviter, const QString &reason);

    void on_toolButtonClose_clicked();

    void on_toolButtonMin_clicked();

public slots:
    void show_window(QString objname);               //显示seperatorwidget的子窗体中的一个
    void onSeparateWindowChanged(bool status);  //分离窗口
    void addItemToRecentFriend(ListItemData *data);

    void showSettingWindow();                      //显示设置窗体
    void showChatWindow(QModelIndex modelIdex);    //点击好友列表聊天窗口显示
    void showChatWindow(ListItemData *data);
    void showRoomWindow(RoomListData*data);
    void updateAllWidgetMessageNum(ListItemData *data);
    void showRoomChatWindow(QModelIndex modelIndex);  //显示群组窗体

    void hideChatWindow(ListItemData *data);  /* 隐藏窗体 */
    void hideRoomWindow(RoomListData*data);

    void setStatusOnline();            //设置在线
    void setStatusOffine();
    void setStatusAway();
    void setStatusBusy();

    void incomingTelegram(QString name);             //sip电话来电

protected:
   bool eventFilter(QObject *obj, QEvent *event);   //事件过滤器
   void mousePressEvent(QMouseEvent *event);

public:
   static FriendList *friendlist;             //指向静态this的指针

   QQPanel *panel;                             //好友列表窗体
   ListItemData *currentShowChatWindowData;   //当前的好友的索引
   ListItemData *myData;                       //指向自己的信息
   Ui::FriendList *ui;
   QHash<QString, ListItemData *> friendList;   //好友列表数据
   QList<QString > currentContactList;     //Recently the contact list;

   keyboard *callPhone; //键盘界面
   bool setAway=false;
   bool friendListReciverComplete=false;
   SettingUi *settingui;                //设置窗体
   RoomListView *roomListView;                     //群组聊天窗体
   QList<RoomListData *> roomListData;             //群组列表数据

   QList<FileTransferWidget *> pauseFileTransfer;  //暂停的文件传输

private:
    //用户状态
    QAction *statusOnline;
    QAction *statusAway;
    QAction *statusBusy;
    QAction *statusoffline;
    SearchListView *searchListView;      //搜索窗体
    SearchListView *recentContactsView;  //the recent contacts view;
    QPointer<QWidget >seperatorwidget;            //窗体显示的位置

    QHBoxLayout *seperatorLayout;        //分离窗体的布局
    AddFriend *addfriend;                 //增加好友窗体
    AddGroup *addgroup;                   //增加组窗体

    //MainMenuStackedWidget *mainMenuWidget;   //主界面窗体
    bool separateWindow;                    //是否分离窗体

    QTimer *timer;                        //定时检测离开
    QMenu *statusButtonMenu;               //状态下拉菜单
    FunctionChoiceWidget *functionSelectWidget;    //群发
    TitleBarWidget *titleBarwidget;              //chat bar title
    NewestMessageFrame *messagetFrame;         //好友最新消息窗体
    friend class Core;
};

#endif // FRIENDLIST_H
