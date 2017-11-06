#ifndef XMPPCLIENT_H
#define XMPPCLIENT_H

#include <QObject>
#include <QImage>
#include <QHash>
#include <QFile>
#include "QXmppClient.h"
#include "QXmppLogger.h"
#include "QXmppPresence.h"
#include "QXmppUtils.h"
#include "QXmppClient.h"
#include "QXmppTransferManager.h"
#include "QXmppRosterManager.h"
#include "QXmppVCardIq.h"
#include "QXmppVCardManager.h"
#include "QXmppMucManager.h"
#include "QXmppDiscoveryManager.h"
#include "QXmppMucIq.h"
#include "QXmppMessage.h"
struct FriendDataMessage{
    QString fromID;
    QString message;
    QString toID;
};
class login;
class XmppClient : public QObject
{
    Q_OBJECT
    Q_ENUMS(Error)
    Q_ENUMS(StatusType)

public:

    enum StatusType
    {
        Online = 0,      ///< The entity or resource is online.
        Away,           ///< The entity or resource is temporarily away.
        XA,             ///< The entity or resource is away for an extended period.
        DND,            ///< The entity or resource is busy ("Do Not Disturb").
        Chat,           ///< The entity or resource is actively interested in chatting.
        Invisible,       ///< obsolete XEP-0018: Invisible Presence.

        Available,      ///< Signals that the sender is online and available for communication.
        Unavailable,    ///< Signals that the sender is no longer available for communication.

    };
    enum Error              //登录及连接错误
    {
        NoError,            ///< No error.
        SocketError,        ///< Error due to TCP socket.
        KeepAliveError,     ///< Error due to no response to a keep alive.
        XmppStreamError     ///< Error due to XML stream.
    };

    struct FriendItem       //好友列表结构体
    {
        QString FriendID;   //好友ID
        QString FriendName; //好友昵称
        QString Group;      //好友所属群组
    };

    struct FriendVcard      //Vcard
    {
        QString username;   //姓名
        QString nickname;   //昵称
        QString email;      //Email 地址
        QString VoIP;       //SIP账号或IP电话
        QString title;      //职务
        QString unit;       //部门
        QString phone;      //电话
        QString mobile;     //移动电话
        QString fax;        //传真
        QImage photo;       //照片
        QString photoType;  //照片类型
    };
    struct NoticeBase
    {
        QStringList toID; //接收人
        enum style
        {
            Instructions,    //指示
            RequestInstructions //请示
        } NoticeStyle;
        bool Urgency;        //是否紧急
        bool Important;      //是否重要
        bool Secret;         //是否密送
        QString Subject;  //主题
        QString Appellation;//称呼
        QString Reason;      //事由
        QString Thing;       //事项
        QString Peroration;  //结束语
        QString Organization;//单位
        QString date;
     };

    QString ServerAddress;

public:
    explicit XmppClient(QObject *parent = 0);
    ~XmppClient();

    void loginWin(QString username, QString password);  //登录调用接口
    void cancel_login();                                //取消登录处理函数

    bool sendfile(QString filepath, QString BareJid, bool isPicture);   //发送文件，需传输文件路径
    void setJid( QString BareJid);                                      //获取需要发送文件对端的ID,调用sendfile前应首先设置
    void saveReceiveFile(QString filepath,QXmppTransferJob *m_job);     //保存接收文件
    QString checkreceiveconfigdata(QXmppTransferJob *c_job);            //接收文件前判断配置文件
    void sendmsg(const FriendDataMessage currentSendMsg);               //发送消息调用接口，传入消息结构体

    void setpresence(XmppClient::StatusType Status);    //调用该函数设置在线状态
    static QString swapStatustoString(XmppClient::StatusType status);

    QXmppMucRoom *addRoom(const QString &roomJid, const QString &Subject, const QString &password); //创建聊天室
    QXmppMucRoom *addRoom(const QString &roomJid);                                                  //加入聊天室
    bool setAffiliation(QXmppMucRoom *room, const QString &Bjid, const QString &affiliation);       //设置群组岗位
    bool setRole(QXmppMucRoom *room, const QString &Bjid, const QString &Role);                     //设置群组权限
    bool remove(QXmppMucRoom *room);                                                                //解除群组永久权限
    bool setRoomName(QXmppMucRoom *room,QString name);                                              //设置聊天室名称

    bool sendNotice(NoticeBase &notice);


private:

    void init();
    void retrunLoginwin();
    void setResourcesList();
    QString getBareJid();
    void setJob(QXmppTransferJob *Job);         //接收到有需要接收的文件后连接相关信号
    void addreceiveconfigdata(QString filename,QString jid,QByteArray hash,QString filepath);   //断点续传，添加接收端配置信息
    QString checkreceiveconfigdata(QString filename,QString jid,QByteArray hash);               //断点续传，检查接收端配置文件中的文件保存路径
    qint64 checkconfigfile(QString filename,QByteArray ba,QString jid);                         //断点续传，比较配置文件信息，返回断点值
    void deleteconfigfile(QXmppTransferJob *job);                                               //断点续传，删除配置文件中的一条信息
    void deletereceiveconfigdata(QXmppTransferJob *job);                                        //断点续传，接收完成删除接收端保存的配置信息
    void addconfigfile(QString filename,QByteArray ba,QString jid,qint64 bp);                   //断点续传，添加保存配置信息

    QString getImageType(const QByteArray &contents); //公告传输中获取图片类型
    void NoticeHanding(QString NoticeBody); //公告接收处理函数

signals:
    void logined();                             //登陆成功信号
    void loginErrortoUI(XmppClient::Error e);   //若登录失败发送该信号
    void ConnectErrortoUI(XmppClient::Error e); //若连接失败向UI发送信号

    void signalSendFileJob(QXmppTransferJob *job);  //发送文件job类
    void signalfileReceived(QXmppTransferJob *job); //有文件需要接收,向UI端发送job类

    void receMesg(const FriendDataMessage msg);     //有消息需要接收

    void friendlistReceived(QList<XmppClient::FriendItem>);                 //发送好友列表更新完毕信号
    void vcardReceived(QString, XmppClient::FriendVcard);                   //vcard获取完毕信号
    void presenstate(QString bareJid, XmppClient::StatusType status);       //好友状态更新状态

    void Groupinvitation(const QString &roomJid, const QString &inviter, const QString &reason); //群组聊天 新邀请信号
    void GroupRoomAdded(QXmppMucRoom *room);                                                     //新加进群组信号

    void NoticeReceived(XmppClient::NoticeBase notice, QString noticeXml);

public slots:

    void loginError(QXmppClient::Error);    //登录错误处理函数
    void connected();                       //连接成功处理函数
    void rosterReceived();                                                  //好友列表接收函数
    void presenceChanged(const QString& bareJid, const QString& resource);  //好友状态发生变化接收函数
    void vCardReceived(const QXmppVCardIq& vCard);                          //vCard接收函数
    void disconnectt(QXmppClient::State status);

private slots:
    //初始化
    void error(QXmppClient::Error e);
    void slotreceiveFinished();
    void slotreceiveError(QXmppTransferJob::Error error);
    void slotsendError(QXmppTransferJob::Error error);
    void slotsendFinished();
    void slotfileReceived(QXmppTransferJob *r_job);
    void messageReceived(const QXmppMessage& msg);
    void GroupinvitationReceived(const QString &roomJid, const QString &inviter, const QString &reason);//群组约请处理函数
    void GroupRoomAddedReceived(QXmppMucRoom *room);

public:
        QXmppClient *client;
        QXmppClient::State currentState = QXmppClient::DisconnectedState;        //用户当前的连接状态

private:
    QString m_pw;
    QString m_user;
    QXmppConfiguration config;
    QStringList resourcesList;
    QString bareJid;
    QXmppTransferManager *transferManager;   
    QXmppMucManager *xmppMucManager;
    QXmppDiscoveryManager *xmppDiscoveryManager;
    QList<QFile *> filelist;                //为关闭文件句柄
    QFile *file;
};
Q_DECLARE_METATYPE(XmppClient::NoticeBase)
Q_DECLARE_METATYPE(XmppClient::Error)

#endif // XMPPCLIENT_H
