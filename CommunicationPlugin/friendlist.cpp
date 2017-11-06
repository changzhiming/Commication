#include "friendlist.h"
#include "ui_friendlist.h"
#include "login.h"
#include "listFriend/qqpanel.h"
#include "listFriend/mylistview.h"
#include "chatwindow.h"
#include "addfriend.h"
#include "addgroup.h"
#include "settingui.h"
#include "listFriend/searchlistview.h"
#include "tool/screenshotpart.h"
#include "chatlog/chatlog.h"
#include "core/core.h"
#include "tool/friendinformation.h"
#include "savesetting.h"
#include "platform/timer.h"
#include "tool/facewidget.h"
#include "keyboard.h"
#include "sip/qsip.h"
#include "mainFunction/functionchoicewidget.h"
#include "core/myapplication.h"
#include "groupChat/mybasedialog.h"
#include "titlebarwidget/titlebarwidget.h"
#include "titlebarwidget/myview.h"
#include "tool/newestmessageframe.h"
#include "groupChat/groupchat.h"
#include <QVector>
#include <QVectorIterator>
#include <QThread>
#include <QSystemTrayIcon>
#include <QUrl>
#include <QMutexLocker>
#include <QtAlgorithms>
#include <QMessageBox>
#include "listFriend/roomlistview.h"
#include "chatlog/content/filetransferwidget.h"
#include "wcontainwidget.h"
#include <QDir>


FriendList* FriendList::friendlist=nullptr;

FriendList::FriendList(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent),
    ui(new Ui::FriendList)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->start(1000);                  //定时检测离开

    currentShowChatWindowData=nullptr;
    /*
     * 用户状态按钮下拉式菜单
     * */
    statusOnline = new QAction(this);
    statusOnline->setText(tr("在线"));
    statusOnline->setIcon(QIcon(getStatusIconPath(XmppClient::Online)));
    connect(statusOnline, &QAction::triggered, this, &FriendList::setStatusOnline);

    statusAway = new QAction(this);
    statusAway->setText(tr("离开"));
    statusAway->setIcon(QIcon(getStatusIconPath(XmppClient::Away)));
    connect(statusAway, &QAction::triggered, this, &FriendList::setStatusAway);

    statusBusy = new QAction(this);
    statusBusy->setText(tr("忙碌"));
    statusBusy->setIcon(QIcon(getStatusIconPath(XmppClient::DND)));
    connect(statusBusy, &QAction::triggered, this, &FriendList::setStatusBusy);


    statusoffline = new QAction(this);
    statusoffline->setText(tr("离线"));
    statusoffline->setIcon(QIcon(getStatusIconPath(XmppClient::Unavailable)));
    connect(statusoffline, &QAction::triggered, this, &FriendList::setStatusOffine);

    statusButtonMenu = new QMenu(ui->statusButton);
    statusButtonMenu->addAction(statusOnline);
    statusButtonMenu->addAction(statusAway);
    statusButtonMenu->addAction(statusBusy);
    statusButtonMenu->addAction(statusoffline);
    ui->statusButton->setMenu(statusButtonMenu);   //设置状态按钮下拉式菜单

/**************************初始化窗体***************************************************/
    ui->widgetSearch->hide();//隐藏搜索窗体
    callPhone=new keyboard(this);       //出事化键盘界面

    /*********************群发窗体初始化******************************/
    functionSelectWidget =new FunctionChoiceWidget();
    QWidget *mainwidget=new QWidget(this);             //好友列表右边主界面
    mainwidget->setObjectName(QStringLiteral("mainwidget"));
    mainwidget->setStyleSheet(QLatin1String("QWidget#mainwidget{background-color:white}"));

    titleBarwidget=new TitleBarWidget(this);

    QVBoxLayout *vboxlayout=new QVBoxLayout(mainwidget);
    vboxlayout->setContentsMargins(0, 12, 0, 0);
    vboxlayout->setSpacing(0);

    seperatorwidget=new QWidget(this);
    //seperatorwidget->setMinimumWidth(500);         //聊天窗口最小宽度
    vboxlayout->addWidget(titleBarwidget);

    vboxlayout->addWidget(seperatorwidget);

    seperatorLayout=new QHBoxLayout(seperatorwidget);

    seperatorLayout->setObjectName(tr("seperatorLayout"));
    seperatorLayout->setContentsMargins(0, 0, 0, 0);
    ui->splitter->addWidget(mainwidget);

    panel=new QQPanel(ui->stackedWidgetFriendList);      //好友列表窗体
    init_window(ui->verticalLayout_4, panel, tr("panel"));

    {
        QList<QToolButton *> buttonlist=ui->widgetTitleBar->findChildren<QToolButton *>();
        buttonlist[ui->stackedWidget->currentIndex()]->setStyleSheet("background:#ecf0f1");
    }

    addfriend=new AddFriend(seperatorwidget);
    init_window(seperatorLayout, addfriend, tr("addfriend"),false);
    addgroup=new AddGroup(seperatorwidget);
    init_window(seperatorLayout, addgroup, tr("addgroup"),false);
    settingui=new SettingUi(seperatorwidget);
    init_window(seperatorLayout, settingui, tr("settingui"), false);

    //mainMenuWidget=new MainMenuStackedWidget(seperatorwidget);
    //init_window(seperatorLayout, mainMenuWidget, tr("mainMenuWidget"), true);

/**********************************************/
    qRegisterMetaType<FriendDataMessage>("FriendDataMessage");  //注册结构体类型供其它用户使用
    qRegisterMetaType<QList<XmppClient::FriendItem>>("QList<XmppClient::FriendItem>");
    qRegisterMetaType<XmppClient::StatusType>("XmppClient::StatusType");
    qRegisterMetaType<XmppClient::FriendVcard>("XmppClient::FriendVcard");
    qRegisterMetaType<QXmppTransferJob::State>("QXmppTransferJob::State");
    qRegisterMetaType<XmppClient::FriendVcard>("XmppClient::FriendVcard");
    qRegisterMetaType<QXmppClient::State>("QXmppClient::State");

    connect(timer, &QTimer::timeout, this, &FriendList::onUserAwayCheck);                  //检测自动离开
    connect(ui->addfriend, SIGNAL(clicked()), this, SLOT(showSettingWindow()));
    connect(ui->addgroup, SIGNAL(clicked()), this, SLOT(showSettingWindow()));
    connect(ui->settings, SIGNAL(clicked()), this, SLOT(showSettingWindow()));
    connect(ui->mainMenuToolbutton, SIGNAL(clicked()), this, SLOT(showSettingWindow()));
    connect(ui->callPhone, SIGNAL(clicked()), this, SLOT(showSettingWindow()));
    connect(ui->groupSend, SIGNAL(clicked()), this, SLOT(showSettingWindow()));


    //连接标题栏
    connect(ui->toolButton_2, SIGNAL(clicked()), this, SLOT(connectTitleBat()));
    connect(ui->toolButton_3, SIGNAL(clicked()), this, SLOT(connectTitleBat()));
    connect(ui->toolButton_4, SIGNAL(clicked()), this, SLOT(connectTitleBat()));
    connect(ui->toolButton_5, SIGNAL(clicked()), this, SLOT(connectTitleBat()));

    connect(Core::getInstance()->xmppClient, SIGNAL(receMesg(FriendDataMessage)), this, SLOT(receiveMessage(FriendDataMessage)));
    connect(Core::getInstance()->xmppClient, SIGNAL(signalfileReceived(QXmppTransferJob*)), this, SLOT(receiveFile(QXmppTransferJob*)));
    connect(Core::getInstance()->xmppClient, SIGNAL(signalSendFileJob(QXmppTransferJob*)), this, SLOT(sendFileJob(QXmppTransferJob*)));
    connect(Core::getInstance()->xmppClient, SIGNAL(friendlistReceived(QList<XmppClient::FriendItem>)), this, SLOT(friendlistReceived(QList<XmppClient::FriendItem>)));
    connect(Core::getInstance()->xmppClient, SIGNAL(vcardReceived(QString, XmppClient::FriendVcard)), this, SLOT(vcardReceived(QString, XmppClient::FriendVcard)));
    connect(Core::getInstance()->xmppClient, SIGNAL(presenstate(QString, XmppClient::StatusType)), this, SLOT(presenstate(QString, XmppClient::StatusType)));
    connect(Core::getInstance()->sipPhone, SIGNAL(Incoming(QString)), SLOT(incomingTelegram(QString)));
    connect(this, SIGNAL(insertMessageToDb(historyMessage *)), Core::getInstance(), SLOT(insertMessageToDb(historyMessage *)));  //存储聊天记录到sqlite



    /* 连接titlebar的单击标签和双击标签 */
    connect(titleBarwidget->view, SIGNAL(showFriend(ListItemData*)), this, SLOT(showChatWindow(ListItemData*)));
    connect(titleBarwidget->view, SIGNAL(hideFriend(ListItemData*)), this, SLOT(hideChatWindow(ListItemData*)));
    connect(titleBarwidget->view, SIGNAL(showRoom(RoomListData*)), this, SLOT(showRoomWindow(RoomListData*)));
    connect(titleBarwidget->view, SIGNAL(hideRoom(RoomListData*)), this, SLOT(hideRoomWindow(RoomListData*)));

    /* 群组信号连接 */
    connect(Core::getInstance()->xmppClient, &XmppClient::GroupRoomAdded, this, &FriendList::onRoomReciver);
    connect(Core::getInstance()->xmppClient, &XmppClient::Groupinvitation, this, &FriendList::Groupinvitation);
    connect(this, SIGNAL(addRoom(QString,QString,QString)), Core::getInstance(), SLOT(coreaddRoom(QString,QString,QString)));

/*****************************************************************************/
    friendlist=this;               //指向自己的指针
    ui->lineEdit->installEventFilter(this);  //安装事件过滤器
    /* 初始化最新消息窗体 */

    messagetFrame=new NewestMessageFrame(this);
    ui->splitter->addWidget(messagetFrame);
    NewesItemScrollArea *wd=new NewesItemScrollArea(MainFunctionButton::TEXT);
    messagetFrame->insertnewesItem(wd);


    /*  //初始化个人信息   */
        myData=new ListItemData();
        while(Core::getInstance()->xmppClient==nullptr)            //避免xmppclient没有初始化
        {
            qApp->processEvents();
        }
        myData->Id=Core::getInstance()->xmppClient->client->configuration().jidBare();
        QRegExp regexp("(.*)(@)");
        int pos=regexp.indexIn(myData->Id);

        myData->Name=regexp.cap(1);
        ui->useranme->setText(myData->Name);                  //设置自己的用户名
        ui->toolButton->setIcon(QIcon(myData->iconPath));
    /**********************************************/

    SaveSetting::getInstance().loadPersonal(myData->Name);     //加载好友列表

    loadLocalFriend(this->friendList);                        //初始化好友列表窗体

    searchListView=new SearchListView(ui->widgetSearch);      //初始化searchlistview
    searchListView->setViewMode(QListView::ListMode);
    searchListView->setStyleSheet("QListView{icon-size:40px}");
    searchListView->setObjectName(tr("searchListView"));
    searchListView->setListType(SearchListView::Search);
    connect(searchListView, SIGNAL(clicked(QModelIndex)), FriendList::friendlist, SLOT(showChatWindow(QModelIndex)));  //绑定MyListView双击和chat窗口显示

    ui->verticalLayout_8->addWidget(searchListView);         //添加初始化searchlistview到界面

    recentContactsView=new SearchListView(ui->stackedWidgetRecentContact);      //初始化searchlistview
    recentContactsView->setViewMode(QListView::ListMode);
    recentContactsView->setStyleSheet("QListView{icon-size:40px}");
    recentContactsView->setObjectName(tr("recentContactsView"));
    recentContactsView->setListType(SearchListView::CurrentContact);
    connect(recentContactsView, SIGNAL(clicked(QModelIndex)), FriendList::friendlist, SLOT(showChatWindow(QModelIndex)));  //绑定MyListView双击和chat窗口显示
    ui->verticalLayout_5->addWidget(recentContactsView);         //添加初始化searchlistview到界面

    loadLocalRecentContacts(this->currentContactList);             //加载最近联系人列表

    roomListView=new RoomListView(ui->stackedWidgetRoomList);
    roomListView->setViewMode(QListView::ListMode);
    roomListView->setStyleSheet("QListView{icon-size:40px}");
    roomListView->setObjectName(tr("roomListView"));
    ui->verticalLayout_7->addWidget(roomListView);

    //restoreGeometry(SaveSetting::getInstance().getWindowGeometry());       //重新加载窗体位置
    ui->frame->resize(SaveSetting::getInstance().getFriendWindowWidth(), this->height());

    Core::getInstance()->openDb(myData->Name+".db");                      //打开聊天记录数据库没有则创建
    ui->splitter->setStretchFactor(1,1);

/**************************************************************/

}

FriendList::~FriendList()
{
    qDebug()<<"delete FriendList";
    saveWindowGeometry();        //保存窗体位置和大小
    SaveSetting::getInstance().savePersonal(myData->Name); //保存个人信息

    foreach(RoomListData *data, roomListData)
    {
        if(data->room) {
            //qDebug()<<Core::getInstance()->xmppClient->remove(data->room);
        }
        delete data;
    }
    timer->stop();

    delete myData;
    delete functionSelectWidget;
    foreach (ListItemData *data, this->friendList.values()) {
        qDebug()<<"delete"<<data->Name;
        if(data)
            delete data;
    }
    delete ui;

}
void FriendList::connectTitleBat()
{
    QToolButton *button=qobject_cast<QToolButton *>(sender());
    QList<QToolButton *> buttonlist=ui->widgetTitleBar->findChildren<QToolButton *>();

    foreach(QToolButton *buttonchild, buttonlist)
    {
        buttonchild->setStyleSheet("background");
        //qDebug()<<buttonchild->objectName();
    }
    ui->stackedWidget->setCurrentIndex(ui->horizontalLayout_5->indexOf(button));
    button->setStyleSheet("background:#ecf0f1");
}

void FriendList::onRoomReciver(QXmppMucRoom *room)
{
    RoomListData *pItem=new RoomListData();
    pItem->room=room;
    QRegExp regexp("(.*)(@)");
    int pos=regexp.indexIn(room->jid());
    if(pos>-1)
    {
        Core::getInstance()->xmppClient->setRoomName(room, regexp.cap(1));
    }

    pItem->RoomName=regexp.cap(1);

    roomListData.append(pItem);
    roomListView->addItem(pItem);

    ui->toolButton_4->click();
    show_window("");                 //隐藏所有窗体
    if(pItem->groupChat!=nullptr)
    {
        pItem->groupChat->show();
    }
    else
    {
        pItem->groupChat=new GroupChat(seperatorwidget);//初始化聊天窗体
        init_window(seperatorLayout, pItem->groupChat, pItem->RoomName ,true);        //初始化聊天窗体并显示
    }

    MyItem *item=new MyItem(pItem);
    titleBarwidget->addItem(item);             //增加聊天标签
    titleBarwidget->focusToItem(pItem);               //光标到当前好友

    QStringList jidlist=room->participants();

    QList<ListItemData *> roomlistdata;
    qDebug()<<jidlist<<room;
    foreach(QString jid, jidlist)
    {
        QRegExp regexp("(.*)(/)");
        qDebug()<<room->participantFullJid((jid));
        int pos=regexp.indexIn(room->participantFullJid((jid)));
        if(pos>-1)
        {
            qDebug()<<regexp.cap(1);
            ListItemData * listData=friendList.value(regexp.cap(1));
            if(listData)
            {
                listData->GroupId=jid;
                roomlistdata.append(listData);
            }
            if(myData->Id==regexp.cap(1))
            {
                roomlistdata.append(myData);
            }
        }
    }
    pItem->groupChat->insertGroupList(roomlistdata);
    pItem->groupChat->setRoom(room);

    connect(pItem->room,SIGNAL(participantAdded(QString)), pItem->groupChat, SLOT(friendAddRoom(QString)));
    connect(pItem->room, SIGNAL(participantRemoved(QString)), pItem->groupChat, SLOT(friendRemoveRoom(QString)));
    connect(pItem->room, SIGNAL(kicked(QString,QString)), pItem->groupChat, SLOT(kicked(QString,QString)));
    connect(pItem->room, SIGNAL(left()), pItem->groupChat, SLOT(left()));
    connect(pItem->room, SIGNAL(allowedActionsChanged(QXmppMucRoom::Actions)), pItem->groupChat, SLOT(allowedActionsChanged(QXmppMucRoom::Actions)));
    connect(pItem->room, SIGNAL(messageReceived(QXmppMessage)), pItem->groupChat->getChatLog(), SLOT(onReciveRoomMessage(QXmppMessage)));
    connect(pItem->room,SIGNAL(permissionsReceived(QList<QXmppMucItem>)), pItem->groupChat, SLOT(permissionsReceived(QList<QXmppMucItem>)), Qt::DirectConnection);

}

void FriendList::Groupinvitation(const QString &roomJid, const QString &inviter, const QString &reason)
{
    qDebug()<<roomJid<<inviter;
    if(QMessageBox::Ok==QMessageBox::information(this, tr("群组邀请"), inviter+tr("邀请你加入")+roomJid, QMessageBox::Cancel, QMessageBox::Ok))
        emit addRoom(roomJid, reason, tr(""));
}

QString FriendList::getStatusIconPath(XmppClient::StatusType status)
{
    switch (status)
    {
    case XmppClient::Online:
        return QStringLiteral(":/statu/image/status/在线.png");
    case XmppClient::Away:
        return QStringLiteral(":/statu/image/status/离开.png");
    case XmppClient::DND:
        return QStringLiteral(":/statu/image/status/忙碌.png");
    case XmppClient::Unavailable:
    default:
        return QStringLiteral(":/statu/image/status/离线.png");
    }
}

void FriendList::on_lineEdit_returnPressed()              //搜索回车按下事件
{
    searchListView->m_pModel->m_ItemDataVec.clear();  //清楚所有的
    searchListView->hide();
    searchListView->show();
    qDebug()<<"enter ";
    foreach(ListItemData *data, this->friendList.values())
    {
        if(data->Name.startsWith(ui->lineEdit->text()))
            searchListView->addItem(data);
    }
    if(searchListView->m_pModel->rowCount())                     //搜多到的行数不是o就显示当前的好友
        showChatWindow(searchListView->currentIndex());

    searchListView->setCurrentIndex(searchListView->m_pModel->index(0)); //获取光标在第0行
}

void FriendList::on_lineEdit_cursorPositionChanged(int arg1, int arg2)     //光标位置改变槽
{

    searchListView->m_pModel->m_ItemDataVec.clear();
    qDebug()<<"cursorpositionchange ";
    foreach(ListItemData *data, this->friendList.values())
    {
        if(data->Name.startsWith(ui->lineEdit->text()))
            searchListView->addItem(data);
    }
       //刷新界面  不刷新界面就会出现延时显示（原因未知）
    {
        searchListView->hide();
        searchListView->show();
    }
    searchListView->setCurrentIndex(searchListView->m_pModel->index(0)); //获取光标在第0行
}
void FriendList::receiveFile(QXmppTransferJob *job)
{
    QRegExp regexp("(.*)(/)");
    qDebug()<<job->jid()<<"123214";
    int pos=regexp.indexIn(job->jid());

    if(job->fileInfo().description()=="Picture")
    {
        QDir dir(qApp->applicationDirPath()+"/receive_picture/"+regexp.cap(1));
        if(!dir.exists()) {
            dir.mkpath(dir.absolutePath());
        }
        qDebug()<<"receive_picture/"+regexp.cap(1)+"/"+job->fileName();
        Core::getInstance()->xmppClient->saveReceiveFile("receive_picture/"+regexp.cap(1)+"/"+job->fileName(),job);
        return;
    }
    if(pos>-1)
    {
        QString Id=regexp.cap(1);
        if(this->friendList.contains(Id))
        {
            ListItemData *data=this->friendList.value(Id);
            if(data->chatwindow!=nullptr)
            {
               if(!data->chatwindow->isVisible())
                   data->messageNumer=QString::number(data->messageNumer.toInt()+1);
               for(QMap<QString, MyListView *>::iterator it=panel->m_pListMap->begin(); it!=panel->m_pListMap->end();it++)
               {
                   MyListView *pList=it.value();
                   if(pList->m_pModel->m_ItemDataVec.contains(data))
                   {
                       pList->repaint();
                       break;
                   }
               }
               data->chatwindow->addFileTransferMessage(data->Name, "", false, job);
            }
            else
            {

               data->chatwindow=new chatWindow(seperatorwidget, data);//初始化聊天窗体
               init_window(seperatorLayout, data->chatwindow, tr(data->Id.toLocal8Bit().data()),false);        //初始化聊天窗体并隐藏
               if(!data->chatwindow->isVisible())
                   data->messageNumer=QString::number(data->messageNumer.toInt()+1);
               for(QMap<QString, MyListView *>::iterator it=panel->m_pListMap->begin(); it!=panel->m_pListMap->end();it++)
               {
                   MyListView *pList=it.value();
                   if(pList->m_pModel->m_ItemDataVec.contains(data))
                   {
                       pList->repaint();
                       break;
                   }
               }
               data->chatwindow->addFileTransferMessage(data->Name, job->localFileUrl().toLocalFile(), false, job);
            }
            addItemToRecentFriend(data);
        }

     }
}
void FriendList::sendFileJob(QXmppTransferJob *job)
{
    foreach(FileTransferWidget *filewidget, pauseFileTransfer)
    {
        if(filewidget->getFilePath()==job->filePath)
        {
            filewidget->setJob(job);
            return;
        }
    }
    QRegExp regexp("(.*)(/)");
    int pos=regexp.indexIn(job->jid());
    QString filepath=job->localFileUrl().toLocalFile();
    if(pos>-1)
    {
        QString Id=regexp.cap(1);
        if(this->friendList.contains(Id))
        {
            ListItemData *data=this->friendList.value(Id);
            if(data->chatwindow!=nullptr)
            {
                data->chatwindow->addFileTransferMessage(myData->Name, filepath, true, job);
            }
            else
            {

                data->chatwindow=new chatWindow(seperatorwidget, data);//初始化聊天窗体
                init_window(seperatorLayout, data->chatwindow, tr(data->Id.toLocal8Bit().data()),false);        //初始化聊天窗体并隐藏

                data->chatwindow->addFileTransferMessage(myData->Name, filepath, true, job);
            }

            addItemToRecentFriend(data);       //insert into data to recently friend
            historyMessage *hismessage=new historyMessage{
                data->Id,
                myData->Name,
                chatWindow::TransferFile,
                QDateTime::currentDateTime(),
                filepath};
            emit insertMessageToDb(hismessage);
        }
    }
}

void FriendList::receiveMessage(const FriendDataMessage message)     //接收消息
{
        QString Id=message.fromID;
        qDebug()<<Id;
        if(this->friendList.contains(Id))
        {
            ListItemData *data=this->friendList.value(Id);

            if(data->chatwindow!=nullptr)
            {
                   if(!data->chatwindow->isVisible())
                       data->messageNumer=QString::number(data->messageNumer.toInt()+1);
                   for(QMap<QString, MyListView *>::iterator it=panel->m_pListMap->begin(); it!=panel->m_pListMap->end();it++)
                   {
                       MyListView *pList=it.value();
                       if(pList->m_pModel->m_ItemDataVec.contains(data))
                       {
                           pList->repaint();
                           break;
                       }
                   }
                   foreach(NewesItemScrollArea *wd , messagetFrame->itemList)
                   {
                       if(wd->messagetype==MainFunctionButton::TEXT)
                       {
                           wd->insertListItem(data, message.message);
                           wd->update();
                           break;
                       }
                   }

                   data->chatwindow->addTextMessage(data->Name, message.message, message.message, QDateTime::currentDateTime());
            }
            else
            {

                   data->chatwindow=new chatWindow(seperatorwidget, data);//初始化聊天窗体
                   init_window(seperatorLayout, data->chatwindow, tr(data->Id.toLocal8Bit().data()),false);        //初始化聊天窗体并隐藏
                   if(!data->chatwindow->isVisible())
                       data->messageNumer=QString::number(data->messageNumer.toInt()+1);

                   for(QMap<QString, MyListView *>::iterator it=panel->m_pListMap->begin(); it!=panel->m_pListMap->end();it++)
                   {
                       MyListView *pList=it.value();
                       if(pList->m_pModel->m_ItemDataVec.contains(data))
                       {
                           pList->repaint();
                           break;
                       }
                   }
                   foreach( NewesItemScrollArea *wd , messagetFrame->itemList)
                   {
                       if(wd->messagetype==MainFunctionButton::TEXT)
                       {
                           wd->insertListItem(data, message.message);
                           wd->update();
                           break;
                       }
                   }

                   data->chatwindow->addTextMessage(data->Name, message.message, message.message, QDateTime::currentDateTime());
            }
            addItemToRecentFriend(data);

            historyMessage *hismessage=new historyMessage{
                data->Id,
                data->Name,
                chatWindow::_Text,
                QDateTime::currentDateTime(),
                message.message};
            emit insertMessageToDb(hismessage);
         }
}

void FriendList::onSeparateWindowChanged(bool status)   //分离窗体
{

    separateWindow=status;            //设置分离窗体属性
    QSize fram_size= ui->frame->size();
    QSize splitter1_size;
    QPoint split_pos= mapToGlobal(ui->splitter->widget(1)->pos());   //分离窗体位置

    seperatorwidget->hide();
    splitter1_size = ui->splitter->widget(1)->size();
    qDebug()<<seperatorwidget->parentWidget();
    if(seperatorwidget->parentWidget()!=nullptr)
    {
       seperatorwidget->setParent(0);
    }
    //seperatorwidget->deleteLater();
    resize(0,0);  //重新设置好友列表的大小

    this->show();

    resize(fram_size);

    setWindowTitle(QString());

    seperatorwidget->move(split_pos.x(), split_pos.ry()-31);
    seperatorwidget->show();
    this->setWindowTitle(tr("聊天工具"));


}

void FriendList::addItemToRecentFriend(ListItemData *data)
{
    if(!currentContactList.contains(data->Id))
    {
        currentContactList.push_front(data->Id);               //inserted into the recent contacts
        recentContactsView->addItem(data);
    }
    else
    {
        recentContactsView->addItem(data);
    }
}

void FriendList::on_toolButton_clicked()           //查看自己的信息
{
    FriendInformation *information=new FriendInformation(this);

    information->setFriendPhoto(QImage(myData->iconPath));
    information->setFriendName(myData->Name);
    information->setFriendStatus(XmppClient::swapStatustoString(myData->status));
    XmppClient::FriendVcard vcad=myData->vcard;
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
    information->move(QCursor::pos());
    information->show();
}
void FriendList::loadLocalFriend(QHash<QString, ListItemData *> friendList)
{
    qDebug()<<this->friendList.size();
    foreach(ListItemData* pItem, friendList.values())
    {
        pItem->status=XmppClient::Unavailable;
        panel->addFriend(pItem, pItem->Group);
    }
}
void FriendList::loadLocalRecentContacts(QList<QString> recentContacts)
{
    recentContactsView->m_pModel->m_ItemDataVec.clear();
    foreach(QString Id, recentContacts)
    {
        if(this->friendList.contains(Id))
        {
            recentContactsView->addItem(this->friendList.value(Id));
        }
    }
}

void FriendList::friendlistReceived(QList<XmppClient::FriendItem> friendList)         //接收好友列表并初始化好友列表
{
    qDebug()<<"friendlistReceived";
    QHash<QString, ListItemData *> MyfriendList=this->friendList;

    this->friendList.clear();
    for(QMap<QString, MyListView *>::iterator it=panel->m_pListMap->begin(); it!=panel->m_pListMap->end();it++)
    {
        MyListView *pList=it.value();
        pList->m_pModel->m_ItemDataVec.clear();
    }
   foreach(XmppClient::FriendItem i, friendList)
   {
            ListItemData *pItem=new ListItemData();
            pItem->Group=i.Group;
            pItem->Id=i.FriendID;

            pItem->Name=i.FriendName;
            if(i.FriendName.isEmpty())
            {
                pItem->Name=i.FriendID;
            }
            QRegExp regexp("(.*)(@)");
            int pos=regexp.indexIn(pItem->Name);
            if(pos!=-1)
            {
                pItem->Name=regexp.cap(1);
            }

             this->friendList.insert(pItem->Id, pItem);
            panel->addFriend(pItem, pItem->Group);
   }
   loadLocalRecentContacts(this->currentContactList);             //加载最近联系人列表
   friendListReciverComplete=true;
   foreach (ListItemData *data, MyfriendList.values()) {
       delete data;
   }
   for(QMap<QString, MyListView *>::iterator it=panel->m_pListMap->begin(); it!=panel->m_pListMap->end();it++)
   {
       MyListView *pList=it.value();
       pList->update();
   }
}

void FriendList::vcardReceived(QString bareJid, XmppClient::FriendVcard friendVcard)        //获取vcard一个人
{
    qDebug()<<bareJid<<"vcardReceived";
    QDir dir(qApp->applicationDirPath()+"/temp_picture");
    if(!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    if(!myData->Id.compare(bareJid))
    {
        myData->vcard=friendVcard;
        if(!friendVcard.photo.isNull())
        {

            QString savePixmapPath=qApp->applicationDirPath()+"/temp_picture/"+myData->Name+".png";
            QImage image=friendVcard.photo;
            image.save(savePixmapPath);
            myData->iconPath=savePixmapPath;
            ui->toolButton->setIcon(QIcon(myData->iconPath));
        }
        ui->useranme->setText(myData->Name);
        return;
    }
    if(this->friendList.contains(bareJid))
    {
        this->friendList.value(bareJid)->vcard=friendVcard;
        if(!friendVcard.photo.isNull())
        {
            QString savePixmapPath=qApp->applicationDirPath()+"/temp_picture/"+friendList.value(bareJid)->Name+".png";
            QImage image=friendVcard.photo;
            image.save(savePixmapPath);
            this->friendList.value(bareJid)->iconPath=savePixmapPath;
            qDebug()<<this->friendList.value(bareJid)->iconPath;
        }
    }
    for(QMap<QString, MyListView *>::iterator it=panel->m_pListMap->begin(); it!=panel->m_pListMap->end();it++)
    {
        MyListView *pList=it.value();
        pList->update();
    }
}

void FriendList::presenstate(QString bareJid, XmppClient::StatusType state)        //获取一个人的状态
{
    qDebug()<<bareJid<<XmppClient::swapStatustoString(state)<<myData->Id;
    if(!myData->Id.compare(bareJid))
    {
        myData->status=state;
        ui->status->setText(XmppClient::swapStatustoString(state));
        return;
    }
    if(this->friendList.contains(bareJid))
    {
        this->friendList.value(bareJid)->status=state;
        for(QMap<QString, MyListView *>::iterator it=panel->m_pListMap->begin(); it!=panel->m_pListMap->end();it++)
        {
            MyListView *pList=it.value();
            if(pList->m_pModel->m_ItemDataVec.contains(this->friendList.value(bareJid)))
            {
                pList->addItemToTop(this->friendList.value(bareJid));
            }
        }

    }
}

void FriendList::showChatWindow(QModelIndex modelIndex)            //点击好友列表聊天窗口显示
{
    if(QObject::sender()->objectName()==tr("searchListView")||QObject::sender()->objectName()=="lineEdit"||QObject::sender()->objectName()==tr("recentContactsView")
            ||QObject::sender()->objectName()==tr("listView"))
    {
        SearchListView* sender;
        if(!(QObject::sender()->objectName()=="lineEdit"))
            sender = qobject_cast<SearchListView*>(QObject::sender());
        else
            sender=this->searchListView;

         //如果当前显示的好友聊天窗口和现在点击的是同一个就不再显示直接返回
         if(currentShowChatWindowData!=nullptr&&currentShowChatWindowData->chatwindow==sender->m_pModel->getItem(modelIndex.row())->chatwindow&&
                 currentShowChatWindowData->chatwindow->isVisible())
            return;
         if(myData==sender->m_pModel->getItem(modelIndex.row()))
             return;
         currentShowChatWindowData=sender->m_pModel->getItem(modelIndex.row());
         if(myData==sender->m_pModel->getItem(modelIndex.row()))   //If the Item be clicked is myData we ignore all thing;
             return;
    }
    else
    {

        MyListView* sender = qobject_cast<MyListView*>(QObject::sender());
        if(currentShowChatWindowData!=nullptr&&currentShowChatWindowData->chatwindow==sender->m_pModel->getItem(modelIndex.row())->chatwindow&&
                currentShowChatWindowData->chatwindow->isVisible())
           return;

        currentShowChatWindowData=sender->m_pModel->getItem(modelIndex.row());
        if(myData==sender->m_pModel->getItem(modelIndex.row()))   //If the Item be clicked is myData we ignore all thing;
            return;
    }

    show_window("");                 //隐藏所有窗体
    if(currentShowChatWindowData->chatwindow!=nullptr)
    {
        currentShowChatWindowData->chatwindow->show();
    }
    else
    {
        currentShowChatWindowData->chatwindow=new chatWindow(seperatorwidget, currentShowChatWindowData);//初始化聊天窗体
        qDebug()<<currentShowChatWindowData->Name<<"friend name";
        init_window(seperatorLayout, currentShowChatWindowData->chatwindow, tr(currentShowChatWindowData->Name.toLocal8Bit().data()),true);        //初始化聊天窗体并显示
    }
    currentShowChatWindowData->messageNumer.clear();

    bool itemornotexits=false;                    //判断标签是否已经存在
    foreach(MyItem *item,titleBarwidget->view->itemList)
    {
        if(item->getItemData()==currentShowChatWindowData)
        {
            itemornotexits=true;

            break;
        }
    }
    if(!itemornotexits)
    {
        MyItem *item=new MyItem(currentShowChatWindowData);
        titleBarwidget->addItem(item);             //增加聊天标签
    }
    updateAllWidgetMessageNum(currentShowChatWindowData);        //更新所有何data相关联的窗体
    //隐藏搜索界面
    if(QObject::sender()->objectName()==tr("searchListView")||QObject::sender()->objectName()=="lineEdit")
    {
      qDebug()<<"sender is searchListView";
      const QString s=tr("搜索联系人/群组");
      ui->stackedWidget->show();
      ui->widgetSearch->hide();
      ui->lineEdit->setText(s);
    }

}
void FriendList::showChatWindow(ListItemData *data)
{
    show_window("");                 //隐藏所有窗体
    data->messageNumer.clear();
    currentShowChatWindowData=data;
    currentShowChatWindowData->chatwindow->show();
    updateAllWidgetMessageNum(data);
   // panel->setCurrentFriendCursor(currentShowChatWindowData->Name);       //光标位置移动到当前的联系人
   // titleBarwidget->focusToItem(currentShowChatWindowData);               //光标到当前好友
}
void FriendList::showRoomWindow(RoomListData *data)
{
    show_window("");                 //隐藏所有窗体
    if(!data->groupChat->isVisible())
    {
        data->groupChat->show();
    }
}
void FriendList::updateAllWidgetMessageNum(ListItemData *data)
{
    foreach(NewesItemScrollArea *area, messagetFrame->itemList)
    {
        if(area->widgetMaplistItem.values().contains(data))
        {
            area->removeListItemWidget(area->widgetMaplistItem.key(data));
            break;
        }
    }
    for(QMap<QString, MyListView *>::iterator it=panel->m_pListMap->begin(); it!=panel->m_pListMap->end();it++)
    {
        MyListView *pList=it.value();
        if(pList->m_pModel->m_ItemDataVec.contains(data))
        {
            pList->repaint();
            break;
        }
    }
    if(data==currentShowChatWindowData)
    {
        titleBarwidget->focusToItem(currentShowChatWindowData);               //光标到当前好友
        panel->setCurrentFriendCursor(currentShowChatWindowData->Name);       //光标位置移动到当前的联系人
    }

}

void FriendList::showRoomChatWindow(QModelIndex modelIndex)
{
    RoomListView *sender = qobject_cast<RoomListView*>(QObject::sender());
    show_window("");                 //隐藏所有窗体

    RoomListData *pItem =sender->m_pModel->getItem(modelIndex.row());
    if(pItem->groupChat!=nullptr)
        pItem->groupChat->show();
    else{
        pItem->groupChat=new GroupChat(seperatorwidget);//初始化聊天窗体
        init_window(seperatorLayout, pItem->groupChat, pItem->RoomName ,true);        //初始化聊天窗体并显示
    }
    pItem->messageNumer.clear();
    bool itemornotexits=false;                    //判断标签是否已经存在
    foreach(MyItem *item, titleBarwidget->view->itemList)
    {
        if(item->getRoomItemData()==pItem){
            itemornotexits=true;
            break;
        }
    }
    if(!itemornotexits){
        MyItem *item=new MyItem(pItem);
        titleBarwidget->addItem(item);             //增加聊天标签
    }
    titleBarwidget->focusToItem(pItem);               //光标到当前好友

}

void FriendList::hideChatWindow(ListItemData *data)
{
    if(data==currentShowChatWindowData)
        show_window(tr("mainMenuWidget"));
}
void FriendList::hideRoomWindow(RoomListData *data)
{
    if(data->groupChat->isVisible())
        show_window(tr("mainMenuWidget"));
}

void FriendList::showSettingWindow()  //显示设置窗体
{
    qDebug()<<sender()->objectName();

    if(QObject::sender()->objectName()==tr("addfriend"))
         show_window(tr("addfriend"));
    if(QObject::sender()->objectName()==tr("addgroup"))
           show_window(tr("addgroup"));
    if(QObject::sender()->objectName()==tr("settings"))
        show_window(tr("settingui"));
    if(QObject::sender()->objectName()==tr("callPhone")){
        if(callPhone->isVisible())
            callPhone->hide();
        else
            callPhone->show();
    }
    if(QObject::sender()->objectName()==tr("mainMenuToolbutton"))
        show_window(tr("mainMenuWidget"));
    if(QObject::sender()->objectName()==tr("groupSend"))
    {
        /*show_window("");                 //隐藏所有窗体
        GroupChat *groupchat=new GroupChat(seperatorwidget);
        groupchat->insertGroupList(this->friendList.values());
        init_window(seperatorLayout, groupchat, "groupchat",true);

        //groupchat->show();

        functionSelectWidget->clearSelectFriendList();
        functionSelectWidget->initSelectFriendList(this->friendList);
        functionSelectWidget->show();
        functionSelectWidget->activateWindow();
        */
    }


}
void FriendList::init_window(QLayout *layout, QWidget *win, QString s, bool show_hide)
{
    layout->addWidget(win);
    win->setObjectName(s);
    if(!show_hide)
        win->hide();
}

void FriendList::show_window(QString objname)             //根据名字显示seperatorwidget子类窗体
{
    seperatorwidget->showNormal();

    QList<QWidget *> widgets = seperatorwidget->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly);
    foreach(QWidget *child, widgets)
    {
        if(child->objectName()==objname)
            child->show();
        else
            child->hide();
    }
}

bool FriendList::eventFilter(QObject *obj, QEvent *event) //事件过滤器
{
    if(obj==ui->lineEdit) {
        if(QEvent::FocusIn==event->type()) {
            qDebug()<<"line edit mouseButton";
            ui->stackedWidget->hide();
            ui->widgetTitleBar->hide();
            ui->widgetSearch->show();
            ui->lineEdit->clear();
            searchListView->setCurrentIndex(searchListView->m_pModel->index(0)); //获取光标在第0行

        }
        if(QEvent::KeyPress==event->type()) {
            QKeyEvent *keyevent=static_cast<QKeyEvent *>(event);
            if(keyevent->key()==Qt::Key_Down) {
                if(searchListView->currentIndex().row()==searchListView->m_pModel->rowCount()-1)
                    searchListView->setCurrentIndex(searchListView->m_pModel->index(0));
                else
                    searchListView->setCurrentIndex(searchListView->m_pModel->index(searchListView->currentIndex().row()+1));
            }
            if(keyevent->key()==Qt::Key_Up) {
                if(searchListView->currentIndex().row()==0)
                    searchListView->setCurrentIndex(searchListView->m_pModel->index(searchListView->m_pModel->rowCount()-1));
                else
                    searchListView->setCurrentIndex(searchListView->m_pModel->index(searchListView->currentIndex().row()-1));
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void FriendList::mousePressEvent(QMouseEvent *event)          //鼠标按键主窗体动作使lineEdit失去焦点并隐藏搜索列表
{
    const QString s=tr("搜索联系人/群组");

    QWidget::mousePressEvent(event);
    if(event->button()==Qt::LeftButton){
        //qDebug()<<"clearFocus"<<"FriendList mousePressEvent";
        ui->lineEdit->clearFocus();
        ui->stackedWidget->show();
        ui->widgetTitleBar->show();
        ui->widgetSearch->hide();
        ui->lineEdit->setText(s);
    }
}

void FriendList::saveWindowGeometry()
{
    SaveSetting::getInstance().setFriendWindowWinth(ui->splitter->widget(0)->width());
}

void FriendList::setStatusOnline()
{
    QAction *status=qobject_cast<QAction *>(QObject::sender());
    ui->statusButton->setIcon(status->icon());
    Core::getInstance()->xmppClient->setpresence(XmppClient::Online);
}
void FriendList::setStatusOffine()
{
    QAction *status=qobject_cast<QAction *>(QObject::sender());
    ui->statusButton->setIcon(status->icon());
     //Core::getInstance()->xmppClient->setpresence(XmppClient::Unavailable);
}
void FriendList::setStatusBusy()
{
    QAction *status=qobject_cast<QAction *>(QObject::sender());
    ui->statusButton->setIcon(status->icon());
    Core::getInstance()->xmppClient->setpresence(XmppClient::DND);
}
void FriendList::setStatusAway()
{
    QAction *status=qobject_cast<QAction *>(QObject::sender());
    ui->statusButton->setIcon(status->icon());
    Core::getInstance()->xmppClient->setpresence(XmppClient::Away);
    setAway=true;          //用户设置离开
}
void FriendList::onUserAwayCheck()                   //用户离开检测
{
    if(setAway)
        return;

    uint32_t autoAwayTime = SaveSetting::getInstance().getAutoAwayTime() * 60 * 1000;
    /*if ((myData->status==XmppClient::Online)&&autoAwayTime && Platform::getIdleTime() >= autoAwayTime) {
       qDebug() << "auto away activated at" << QTime::currentTime().toString()<<QString::number(Platform::getIdleTime());
       statusAway->trigger();
    }
    if ((myData->status==XmppClient::Away)&&(!autoAwayTime || Platform::getIdleTime() < autoAwayTime)) {
          qDebug() << "auto away deactivated at" << QTime::currentTime().toString();
          statusOnline->trigger();
    }*/
}

void FriendList::incomingTelegram(QString name)
{
    QRegExp regexp("(:)(.*)(@)");
    int pos=regexp.indexIn(name);

    qDebug()<<regexp.cap(2);
    for(QMap<QString, MyListView *>::iterator it=panel->m_pListMap->begin(); it!=panel->m_pListMap->end();it++)
    {
        int i=0;
        MyListView *pList=it.value();
        ListItemData *data;
        while((data=pList->m_pModel->getItem(i++))!=nullptr)
        {
            if(data->vcard.VoIP==regexp.cap(2)){
                Core::getInstance()->playSound();
                //////////////////////////////////////////////
                     //如果当前显示的好友聊天窗口和现在点击的是同一个就不再显示直接返回
                currentShowChatWindowData=pList->m_pModel->getItem(i-1);

                show_window("");                 //隐藏所有窗体
                if(currentShowChatWindowData->chatwindow){
                    currentShowChatWindowData->chatwindow->show();
                }else{
                    currentShowChatWindowData->chatwindow=new chatWindow(seperatorwidget, currentShowChatWindowData);//初始化聊天窗体
                    qDebug()<<currentShowChatWindowData->Name<<"friend name";
                    init_window(seperatorLayout, currentShowChatWindowData->chatwindow, tr(currentShowChatWindowData->Name.toLocal8Bit().data()),true);        //初始化聊天窗体并显示
                }
                currentShowChatWindowData->messageNumer.clear();

                panel->setCurrentFriendCursor(currentShowChatWindowData->Name);            //光标位置移动到当前的联系人
                if(QMessageBox::information(qobject_cast<QWidget *>(this->parent()), QObject::tr("来电"), data->Name ,QMessageBox::Cancel, QMessageBox::Ok)==QMessageBox::Ok)
                {
                    if(Core::getInstance()->sipPhone){
                        Core::getInstance()->sipPhone->Answer();
                        currentShowChatWindowData->chatwindow->on_toolButton_4_clicked();                         //改便上方通话图标和装太
                    }
                }
                Core::getInstance()->stopSound();
                return;
            }
        }
        callPhone->incomingShow(regexp.cap(2));
    }
}



void FriendList::on_toolButtonClose_clicked()
{
     hide();
}

void FriendList::on_toolButtonMin_clicked()
{
    if(WContainWidget::MainWidget)
        WContainWidget::MainWidget->showMinimized();
}
