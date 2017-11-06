
#include <QThread>
#include <QMessageBox>
#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QCryptographicHash>
#include <QSettings>
#include <QImageReader>
#include <QXmlStreamWriter>
#include "xmppclient.h"
#include "QXmppMessage.h"
#include "QXmppRosterManager.h"
#include <qxmlstream.h>

#define CONFIGPATH  QCoreApplication::applicationDirPath()

#include "friendlist.h"   //添加群组
#include "../login.h"     //添加群组

XmppClient::XmppClient(QObject *parent) : QObject(parent)
{
    qDebug()<<QThread::currentThread()->objectName();
    qRegisterMetaType<QXmppTransferJob::State>("QXmppTransferJob::State");
    client = new QXmppClient;
    transferManager=nullptr;
    connect(client, SIGNAL(connected()), this, SLOT(connected()));  //成功登录服务器后，触发该信号
    connect(client, SIGNAL(stateChanged(QXmppClient::State)), this, SLOT(disconnectt(QXmppClient::State)));
}

XmppClient::~XmppClient()
{
    qDebug()<<"delete xmppClient"<<QThread::currentThread()->objectName();
    if(transferManager!=nullptr)
    {
        delete transferManager;
    }
    delete client;
    qDebug()<<"delete xmppClient success";
}
/********************************************************************
 * Function:    登录接口函数
 * Description: 登录时调用该函数完成登录参数传递
 * Cells:       该函数由UI端调用
 * Input:       输入参数为用户名和密码
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::loginWin(QString username, QString password)
{
    connect(client,SIGNAL(error(QXmppClient::Error)),this,SLOT(loginError(QXmppClient::Error)));
    config.setJid(username+ServerAddress);
    config.setPassword(password);
    client->connectToServer(config);
    client->logger()->setLoggingType(QXmppLogger::StdoutLogging); //需保留,log信息，注释后将不在打印xmpp协议内容
}

void XmppClient::loginError(QXmppClient::Error e)
{
    Error m_e;

    if(e==QXmppClient::SocketError)
        m_e=SocketError;
    else if(e==QXmppClient::KeepAliveError)
        m_e=KeepAliveError;
    else if(e==QXmppClient::XmppStreamError)
        m_e=XmppStreamError;
    emit loginErrortoUI(m_e);
    cancel_login();
}

void XmppClient::cancel_login()
{
    disconnect(client,SIGNAL(error(QXmppClient::Error)),this,SLOT(loginError(QXmppClient::Error)));
    client->disconnectFromServer();
}

void XmppClient::connected()
{
    qDebug()<<"登录成功!";
    static bool test=false;
    init();
    disconnect(client,SIGNAL(error(QXmppClient::Error)),this,SLOT(loginError(QXmppClient::Error)));
    if(!test)
    {
        emit logined();
        test=true;
        qDebug()<<"emit logined();";
    }
}

void XmppClient::error(QXmppClient::Error e)
{
    Error m_e;

    if(e==QXmppClient::SocketError)
    {
        m_e=SocketError;
    }
    else if(e==QXmppClient::KeepAliveError)
    {
        m_e=KeepAliveError;
    }
    else if(e==QXmppClient::XmppStreamError)
    {
        m_e=XmppStreamError;
    }
    emit ConnectErrortoUI(m_e);
    retrunLoginwin();
}

void XmppClient::retrunLoginwin()
{
    disconnect(client,SIGNAL(error(QXmppClient::Error)),this,SLOT(error(QXmppClient::Error)));
    client->disconnectFromServer();

}

/********************************************************************
 * Function:    初始化函数
 * Description: 完成收发消息、文件传输、好友列表、在线状态、vCard等信息信号与槽函数的连接
 * Cells:       登录成功后调用该函数
 * Input:
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::init()
{
    bool check;
    Q_UNUSED(check);
    check = connect(client,SIGNAL(error(QXmppClient::Error)),this,SLOT(error(QXmppClient::Error)));             //连接错误信号
    Q_ASSERT(check);
    check = connect(client, SIGNAL(messageReceived(QXmppMessage)),this,SLOT(messageReceived(QXmppMessage)));    //接收消息信号
    Q_ASSERT(check);

    transferManager = new QXmppTransferManager;
    //xmppTransferManager.setProxy("proxy."+ServerAddress);             //Set the JID of the SOCKS5 bytestream proxy to use for outgoing transfers.
    transferManager->setSupportedMethods(QXmppTransferJob::AnyMethod);  //Set the supported stream methods.
    client->addExtension(transferManager);                              //provides the support for file transmission
    connect(transferManager,SIGNAL(fileReceived(QXmppTransferJob*)),this,SLOT(slotfileReceived(QXmppTransferJob*))); //文件传输

    xmppMucManager = new QXmppMucManager;
    xmppDiscoveryManager = new QXmppDiscoveryManager;
    client->addExtension(xmppMucManager);                               //支持群组聊天
    client->addExtension(xmppDiscoveryManager);



    check = connect(&client->rosterManager(), SIGNAL(rosterReceived()),this,SLOT(rosterReceived()));//好友列表20160830
    Q_ASSERT(check);
    check = connect(&client->rosterManager(), SIGNAL(presenceChanged(QString,QString)),this,SLOT(presenceChanged(QString,QString)));//在线状态20160830
    Q_ASSERT(check);
    check = connect(&client->vCardManager(), SIGNAL(vCardReceived(QXmppVCardIq)),this,SLOT(vCardReceived(QXmppVCardIq)));//vCard20160830
    Q_ASSERT(check);
    check = connect(xmppMucManager,SIGNAL(invitationReceived(QString,QString,QString)),this,SLOT(GroupinvitationReceived(QString,QString,QString)));//群组约请
    Q_ASSERT(check);
    check = connect(xmppMucManager,SIGNAL(roomAdded(QXmppMucRoom*)),this,SLOT(GroupRoomAddedReceived(QXmppMucRoom*)));//成功加入或创建一个群组，在该槽中连接关于room的信号
    Q_ASSERT(check);


    QString ownBareJid = client->configuration().jidBare();
    qDebug()<<"oWnBareJid = "<<ownBareJid;          //测试是否成功登录，打印登录用户的ID

}

/********************************************************************
 * Function:    文件发送接口函数
 * Description: 调用该函数完成文件发送的任务
 * Cells:
 * Input:       filepath为文件完整路径包含文件名称(如：F:/workspace/ui_chatwindow.h),BareJid为发送对象的ID(如：wang@192.168.20.4)
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
bool  XmppClient::sendfile(QString filepath, QString BareJid, bool isPicture)
{
    bareJid = BareJid;
    QXmppTransferJob *sendFileJob;
    if(!filepath.isEmpty())
    {
/*************计算要发送文件的MD5值**********以下为添加部分*****************************************/
        QStringList ne = filepath.split("/");
        QString name = ne.at(ne.size()-1);
        QFile nfile;
        nfile.setFileName(name);
        if(!nfile.open(QIODevice::ReadOnly))
             qDebug() << "cannet open task file!" ;
        QByteArray ba = QCryptographicHash::hash(nfile.readAll(),QCryptographicHash::Md5);
        nfile.close();

        qint64 bp;
        bp = checkconfigfile(filepath,ba,BareJid);
        if(bp > 0)
        {
            qDebug()<<"this is a uncomplete task,break point is:"<<bp;
        }
        else
        {
            qDebug()<<"send file! get recveived file size and check again!";
        }
/***********************************************************************************************/
        setResourcesList();

        if(!resourcesList.isEmpty())
        {
            //transferManager = client->findExtension<QXmppTransferManager>();
            //此处需修改sendFile函数的第三个参数，作为断点值传递。并将文件路径和hash值保存在job类中
            QString fn = filepath + "#" + QString::number(bp);//文件路径加断点值
            if(isPicture)
            {
                sendFileJob = transferManager->sendFile(BareJid+"/"+resourcesList[0],fn, "Picture");
            }
            else
            {
                sendFileJob = transferManager->sendFile(BareJid+"/"+resourcesList[0],fn, "_");
            }
            /*****************************add*****************************/
            sendFileJob->filePath = filepath;
            sendFileJob->hash = ba;
            /*************************************************************/
			
			
			
			
			if(sendFileJob!=NULL)
            {
                connect(sendFileJob, SIGNAL(error(QXmppTransferJob::Error)),this, SLOT(slotsendError(QXmppTransferJob::Error)));
                /*********************************add*************************************************************/
                connect(sendFileJob, SIGNAL(finished()),this, SLOT(slotsendFinished()));
                /**************************************************************************************************/
                if(!isPicture)
                    emit signalSendFileJob(sendFileJob);    //发送job类的地址
            }

        }
        else
        {
            qDebug()<<"Please confirm the user is online, you can only send the online user files!";
            return false;
        }
    }
    return true;
}
/********************************************************************
 * Function:    该函数为获取FullID的描述部分，如/spark
 * Description:
 * Cells:
 * Input:
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::setResourcesList()
{
    qDebug()<<getBareJid();
    resourcesList =client->rosterManager().getResources(getBareJid());
}



void XmppClient::slotreceiveError(QXmppTransferJob::Error error)
{
    qDebug() << "receive Transmission failed:" << error;
}

void XmppClient::slotsendError(QXmppTransferJob::Error error)
{
    /**************************************add**************************************/
    QXmppTransferJob *f_job = qobject_cast<QXmppTransferJob *>(sender());
    addconfigfile(f_job->filePath, f_job->hash, f_job->jid(), f_job->bytesreceived);
    /*******************************************************************************/
    qDebug() << "sender Transmission failed:" << error;
	
}
/********************************************************************
 * Function:    接收文件槽函数，接收文件后，执行该函数
 * Description: 调用set
 * 函数连接传输过程中的相干信号和槽；向UI端发送参数为JOB的信号
 * Cells:
 * Input:
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::slotfileReceived(QXmppTransferJob *r_job)
{
    setJob(r_job);
    emit signalfileReceived(r_job);
}

void XmppClient::setJob(QXmppTransferJob *Job)
{
    connect(Job, SIGNAL(error(QXmppTransferJob::Error)),this, SLOT(slotreceiveError(QXmppTransferJob::Error)));
    connect(Job, SIGNAL(finished()),this, SLOT(slotreceiveFinished()));
    //connect(Job, SIGNAL(progress(qint64,qint64)),this, SLOT(slotProgress(qint64,qint64)));//该信息修改由UI端直接接收并处理，顾注释掉
    qDebug()<<"~~~~~~~~~~~~~~~~~~fileReceived~~~~~~~~~~~~~~~~~~~~~~~";
}

/********************************************************************
 * Function:    文件接收完成处理函数，关闭文件句柄
 * Description:
 * Cells:
 * Input:
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::slotreceiveFinished()
{


    QXmppTransferJob *f_job = qobject_cast<QXmppTransferJob *>(sender());                     
    if(f_job->bytesend==f_job->fileSize())
        deletereceiveconfigdata(f_job);                     //删除接收端文件配置信息
    foreach(QFile *file ,filelist)
    {
        if(file->objectName() == f_job->sid())
        {
            file->close();
            filelist.removeOne(file);
        }
    }
    qDebug() << "Transmission finished";
}


/********************************************************************
 * Function:    查询配置信息文件接口函数
 * Description:
 * Cells:
 * Input:
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
QString XmppClient::checkreceiveconfigdata(QXmppTransferJob *c_job)
{
    QByteArray ba = c_job->fileHash().toBase64();
    QString data = c_job->fileName() + c_job->jid() + QString(ba);
    //检查配置文件中数据记录
    QSettings setting(CONFIGPATH,QSettings::IniFormat);
    setting.beginGroup("client");
    QString filepath = setting.value(data).toString();
    setting.endGroup();
    if(filepath == NULL)
    {
        return NULL;
    }
    else
    {
        QFile f;
        f.setFileName(filepath);
        if(f.exists())  //存在文件的情况,直接返回文件大小作为断点值。
        {

            if(c_job->fileOffset()!=f.size())
            {
                c_job->setReturnFileSize(f.size());
            }
            return filepath;

        }
        else    //不存在文件，设置断点值为空。并删除此条无用记录，返回空（让用户直接选择文件保存路径）
        {
            deletereceiveconfigdata(c_job);  //删除接收端保存的配置信息
            return NULL;
        }
    }
}
/********************************************************************
 * Function:    保存文件接口函数
 * Description: 当UI端选择完成路径后，调用该函数
 * Cells:       由UI端调用
 * Input:       输入参数文件路径和相关传输任务JOB
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::saveReceiveFile(QString filepath,QXmppTransferJob *m_job)
{
/****************************************add*****************************************************/
    QString fileName = filepath;
    if (!fileName.isNull())
    {
        //首先保存文件配置信息（防止接收端突然终止时，不能保存文件路径）
        addreceiveconfigdata(m_job->fileName(),m_job->jid(),m_job->fileHash(),filepath);
        /*****************add将接收文件全路径传入到库中***********************/
        m_job->transferSetStoryPath(filepath);
        /****************************************************************/
        QFile *file = new QFile(fileName);
        file->setObjectName(m_job->sid());
        file->open(QIODevice::WriteOnly|QIODevice::Append);
        m_job->accept(file);
        filelist.append(file);
    }
    else
    {
        m_job->abort();
    }
/**************************************************************************************************/
}

/********************************************************************
 * Function:    设置接收文件的JID
 * Description:
 * Cells:       由UI端调用
 * Input:       输入参数为JID
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::setJid( QString  BareJid)
{
    bareJid = BareJid;
}

QString XmppClient::getBareJid()
{
    return bareJid;
}



/********************************************************************
 * Function:    消息接收槽函数
 * Description: 将接收到的消息进行处理，按照私有的格式通过信号的方式进行发送给UI 端
 * Cells:       当有需要接收的消息时，调用该函数进行接收
 * Input:       接收到的消息
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::messageReceived(const QXmppMessage& msg)
{

    if (msg.body().isEmpty())
        return;
    if(msg.type()==QXmppMessage::Chat)
    {
        FriendDataMessage currentReceMsg;
        currentReceMsg.fromID = QXmppUtils::jidToBareJid(msg.from());
        currentReceMsg.message=msg.body();
        emit receMesg(currentReceMsg);
		//NoticeHanding(msg.body());
    }
    if(msg.type()==QXmppMessage::GroupChat)
    {
          qDebug()<<"GroupChat";
    }
    if(msg.type()==QXmppMessage::Error)
    {
        qDebug()<<"message Error";
    }
    if(msg.type()==QXmppMessage::Normal)
    {
        qDebug()<<"message Normal";
    }
    if(msg.type()==QXmppMessage::Headline)
    {
        qDebug()<<"message Headlin";
        NoticeHanding(msg.body());
    }

}

/********************************************************************
 * Function:    消息发送接口函数
 * Description:
 * Cells:       有UI 端进行调用
 * Input:       需要发送的信息作为参数
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::sendmsg(FriendDataMessage currentSendMsg)
{
    qDebug()<<QThread::currentThread()->objectName();
    QString ownBareJid = client->configuration().jidBare();
    qDebug()<<"sendmsg oWnBareJid  = "<<ownBareJid;
    QString message = currentSendMsg.message;
    QXmppMessage msg("", currentSendMsg.toID, message);
    msg.setStamp(QDateTime::currentDateTime());

    client->sendPacket(msg);
}



/********************************************************************
 * Function:    好友列表接收槽函数
 * Description: 当列表接收完成后，按照私有的格式发送以信号参数的形式发送给UI 端
 * Cells:       当接收到好友列表后，由信号连接该槽函数
 * Input:
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::rosterReceived()
{
    FriendItem currentItem;                     //定义好友列表结构体
    QList<XmppClient::FriendItem> friendlist;   //定义好友列表
    foreach (const QString &bareJid, client->rosterManager().getRosterBareJids())
    {
        client->vCardManager().requestVCard(bareJid);                                   //获取好友Vcard
        currentItem.FriendName = client->rosterManager().getRosterEntry(bareJid).name();//获取备注名称
        foreach(const QString &group, client->rosterManager().getRosterEntry(bareJid).groups())
        {
            currentItem.Group = group;
        }

        currentItem.FriendID = bareJid;
        friendlist.append(currentItem);
    }
    client->vCardManager().requestVCard(client->configuration().jidBare());//20160929获取自己vcard未同步
    while(FriendList::friendlist==nullptr)      //等待初始化完成
    {
        QThread::usleep(1);
    }
    emit(friendlistReceived(friendlist));       //发射好友列表更新完毕信号
}

/********************************************************************
 * Function:    好友状态变化槽函数
 * Description: 当好友状态发生变化后，由连接的连接的信号调用该函数，并以信号的形式将好友状态进行发送
 * Cells:       当好友状态发生变化后，由连接的连接的信号调用该函数
 * Input:
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::presenceChanged(const QString& bareJid, const QString& resource)
{
    QXmppPresence presence = client->rosterManager().getPresence(bareJid,resource); //get the updated presence
    while(FriendList::friendlist==nullptr);  //等待界面

    if(presence.type()==QXmppPresence::Unavailable)
    {
        emit presenstate(bareJid, XmppClient::Unavailable);
        return;
    }
    else if(presence.type()==QXmppPresence::Available)
    {
        emit presenstate(bareJid, XmppClient::StatusType(presence.availableStatusType()));
        return;
    }
}

/********************************************************************
 * Function:    vCard接收槽函数
 * Description: 当vCard接收完成后，按照私有的格式发送以信号参数的形式发送给UI 端
 * Cells:       当接收到vCaard后，由信号连接该槽函数
 * Input:
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::vCardReceived(const QXmppVCardIq& vCard)
{
    FriendVcard currentVcard;                           //定义vCard 结构体
    QString bareJid = vCard.from();
    currentVcard.username = vCard.firstName();
    currentVcard.nickname = vCard.nickName();
    currentVcard.email  =   vCard.email();
    currentVcard.VoIP   =   vCard.url();
    currentVcard.title  =   vCard.organization().title();
    currentVcard.unit   =   vCard.organization().unit();
    for(int i=0;i<vCard.phones().size(); ++i)
    {
        if(vCard.phones().at(i).type()==QXmppVCardPhone::Type(QXmppVCardPhone::Work|QXmppVCardPhone::Voice))
        {
            //qDebug()<<vCard.phones().at(i).number();    //商务：电话
            currentVcard.phone = vCard.phones().at(i).number();
        }
        if(vCard.phones().at(i).type()==QXmppVCardPhone::Type(QXmppVCardPhone::Work|QXmppVCardPhone::Fax))
        {
            //qDebug()<<vCard.phones().at(i).number();    //商务：传真
            currentVcard.fax = vCard.phones().at(i).number();
        }
        if(vCard.phones().at(i).type()==QXmppVCardPhone::Type(QXmppVCardPhone::Work|QXmppVCardPhone::Cell))
        {
            //qDebug()<<vCard.phones().at(i).number();    //商务：移动电话
            currentVcard.mobile = vCard.phones().at(i).number();
        }
    }
    currentVcard.photoType = vCard.photoType();
    QByteArray m_photo = vCard.photo();
    QBuffer buffer;
    buffer.setData(m_photo);
    buffer.open(QIODevice::ReadOnly);
    QImageReader imageReader(&buffer);
    QImage image = imageReader.read();
    currentVcard.photo = image;
    while(FriendList::friendlist==nullptr)      //等待初始化完成
    {
        qApp->processEvents();
    }
    emit vcardReceived(bareJid,currentVcard);  //发射好友列表更新完毕信号
}
void XmppClient::disconnectt(QXmppClient::State status)               //当前的连接状态改变
{
    qDebug()<<"current status"<<QXmppClient::DisconnectedState;
    currentState=status;
}

void XmppClient::setpresence(XmppClient::StatusType Status)
{
    QXmppPresence presence = client->clientPresence();
    if(Status == (XmppClient::StatusType)QXmppPresence::Online)
        presence.setStatusText("在线");
    if(Status == (XmppClient::StatusType)QXmppPresence::Away)
        presence.setStatusText("离开");
    if(Status == (XmppClient::StatusType)QXmppPresence::XA)
        presence.setStatusText("Not available");
    if(Status == (XmppClient::StatusType)QXmppPresence::DND)
        presence.setStatusText("正忙");
    if(Status == (XmppClient::StatusType)QXmppPresence::Chat)
        presence.setStatusText("空闲");
    presence.setFrom(client->configuration().jidBare());
    if(Status==XmppClient::Unavailable)
        presence.setType(QXmppPresence::Unavailable);
    else
        presence.setAvailableStatusType((QXmppPresence::AvailableStatusType)Status);
    client->setClientPresence(presence);
}
QString XmppClient::swapStatustoString(XmppClient::StatusType status)
{
    switch (status)
    {
    case XmppClient::Online:
        return QString("在线");
    case XmppClient::Away:
        return QString("离开");
    case XmppClient::XA:
        return QString("无效");
    case XmppClient::DND:
        return QString("忙碌");
    case XmppClient::Chat:
        return QString("聊天中");
    case XmppClient::Unavailable:
        return QString("离线");
    default:
        return QString("离线");
        ;
    }
    return QString("离线");
}



/********群组聊天*********/
QXmppMucRoom * XmppClient::addRoom(const QString &roomJid,const QString &Subject,const QString &password)
{
    QXmppMucRoom *room = xmppMucManager->addRoom(roomJid);   //创建群组
    if(!password.isEmpty())
    {
        room->setPassword(password);
    }
    if(!Subject.isEmpty())
    {
        room->setSubject(Subject);
    }
    room->setNickName(client->configuration().user()); //设置群内聊天昵称
    room->join();



    //设置为固定聊天室
    QXmppDataForm from;
    from.setType(QXmppDataForm::Submit);
    QXmppDataForm::Field field(QXmppDataForm::Field::BooleanField);
    field.setKey("muc#roomconfig_persistentroom");
    field.setDescription("Make room persisten");
    QVariant value;
    value.setValue(true);
    field.setValue(value);

    /*添加一个FIELD*/
//    QXmppDataForm::Field fieldWhoJid(QXmppDataForm::Field::TextSingleField) ;
//    fieldWhoJid.setKey("muc#roomconfig_whois");
//    fieldWhoJid.setDescription("Present real Jabber IDs to");
//    fieldWhoJid.setValue("anyone");
    QList<QXmppDataForm::Field> fields;
    fields.append(field);
 //   fields.append(fieldWhoJid);
    from.setFields(fields);
    room->setConfiguration(from);
    fields.removeLast();
    return room;
}

QXmppMucRoom * XmppClient::addRoom(const QString &roomJid)
{
    QXmppMucRoom *room = xmppMucManager->addRoom(roomJid);   //创建群组
    room->setNickName(client->configuration().user());
    room->join();
    return room;
}

void XmppClient::GroupRoomAddedReceived(QXmppMucRoom *room)
{
    emit GroupRoomAdded(room);
}

void XmppClient::GroupinvitationReceived(const QString &roomJid, const QString &inviter, const QString &reason)//接收约请信号
{
    qDebug()<<"roomJid = "<<roomJid <<"   "<<"inviter = "<<inviter<<"   "<<"reason = "<<reason;
    emit Groupinvitation(roomJid, inviter, reason);
}

bool XmppClient::setAffiliation(QXmppMucRoom *room, const QString &Bjid, const QString &affiliation)
{
    qDebug()<<"setAffiliation";
    QXmppMucItem    mucitem;
    QList<QXmppMucItem> permissions;
//    QString NickName;
    QXmppMucItem::Affiliation CurrentAffiliation;
    bool re;
    mucitem.setJid(Bjid);
/*
    QRegExp regexp("(.*)(@)");
    int pos=regexp.indexIn(Bjid);
    if(pos!=-1)
    {
        NickName = regexp.cap(1);
    }
     mucitem.setNick(NickName);
*/

    CurrentAffiliation = mucitem.affiliationFromString(affiliation);
     mucitem.setAffiliation(CurrentAffiliation);
     permissions.append(mucitem);
     re = room->setPermissions(permissions);
     permissions.removeLast();
     return re;
}

bool XmppClient::setRole(QXmppMucRoom *room, const QString &Bjid, const QString &Role)
{
    qDebug()<<"setRole";
    QXmppMucItem    mucitem;
    QList<QXmppMucItem> roles;
    QString NickName;
    QXmppMucItem::Role CurrentRole;
    bool re;
    mucitem.setJid(Bjid);

    QRegExp regexp("(.*)(@)");
    int pos=regexp.indexIn(Bjid);
    if(pos!=-1)
    {
        NickName = regexp.cap(1);
    }
     mucitem.setNick(NickName);
     CurrentRole = mucitem.roleFromString(Role);
     mucitem.setRole(CurrentRole);
     roles.append(mucitem);
     re = room->setPermissions(roles);
     roles.removeLast();
     return re;
}

bool XmppClient::remove(QXmppMucRoom *room)
{
    QXmppDataForm persistentroom;
    persistentroom.setType(QXmppDataForm::Submit);
    QXmppDataForm::Field field(QXmppDataForm::Field::BooleanField);
    field.setKey("muc#roomconfig_persistentroom");
    field.setDescription("Make room persisten");
    QVariant value;
    value.setValue(false);
    field.setValue(value);
    QList<QXmppDataForm::Field> fields;
    fields.append(field);
    persistentroom.setFields(fields);
    bool re = room->setConfiguration(persistentroom);
    fields.removeLast();
    return re;
}

bool XmppClient::setRoomName(QXmppMucRoom *room,QString name)
{
    QXmppDataForm from;
    from.setType(QXmppDataForm::Submit);
    QXmppDataForm::Field field(QXmppDataForm::Field::TextSingleField);
    field.setKey("muc#roomconfig_roomname");
    QVariant value(name);
    field.setValue(value);
    QList<QXmppDataForm::Field> fields;
    fields.append(field);
    from.setFields(fields);
    bool re = room->setConfiguration(from);
    fields.removeLast();
    qDebug()<<room->name();
    return re;

}


/********************************************************************
 * Function:    checkconfigfile检查发送端配置文件信息
 * Description:
 * Cells:
 * Input:
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
qint64 XmppClient::checkconfigfile(QString filename, QByteArray ba, QString jid)
{
    //判断配置文件中是否存在当前发送文件配置，如果有则设置对端保存路经和JID
    QByteArray md5 = ba.toBase64();
    QString record = filename + QString(md5) + jid;
    QSettings settings(CONFIGPATH,QSettings::IniFormat);
    //判断配置文件中有没有这条记录
    settings.beginGroup("config");
    qint64 setval = settings.value(record).toInt();
    settings.endGroup();
    if(setval > 0)
    {
        return setval;
    }
    else
    {
        return 0;
    }

}

/********************************************************************
 * Function:    slotsendFinished发送完成槽函数
 * Description:
 * Cells:
 * Input:
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::slotsendFinished()
{
    qDebug() << "Transmission finished";
    QXmppTransferJob *f_job = qobject_cast<QXmppTransferJob *>(sender());
    deleteconfigfile(f_job);
}

/********************************************************************
 * Function:    deleteconfigfile
 * Description:
 * Cells:
 * Input:
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::deleteconfigfile(QXmppTransferJob *job)
{
    QByteArray md5 = job->hash.toBase64();
    QString str = job->filePath + QString(md5) + job->jid();
    if(job->bytesreceived == job->fileSize())
    {
        QSettings setting(CONFIGPATH,QSettings::IniFormat);
        setting.beginGroup("config");
        if(!setting.value(str).toString().isEmpty())
            setting.remove(str);
        setting.endGroup();
        qDebug()<<"transfer completed! remove breakpoint!";
    }
    else
        qDebug()<<"transfer uncompleted! save breakpoint!";
		
}

void XmppClient::addconfigfile(QString filename, QByteArray ba, QString jid, qint64 bp)
{
    QByteArray md5 = ba.toBase64();
    QString data = filename + QString(md5) + jid;
    QSettings settings(CONFIGPATH,QSettings::IniFormat);
    settings.beginGroup("config");
    //添加一条记录
    settings.setValue(data,bp);
    settings.endGroup();
    return;
}

void XmppClient::addreceiveconfigdata(QString filename,QString jid,QByteArray hash,QString filepath)
{
    //return job->readStoryPath();
    QByteArray ba = hash.toBase64();
    QString data = filename + jid + QString(ba);
    //添加配置文件中数据记录
    QSettings setting(CONFIGPATH,QSettings::IniFormat);
    setting.beginGroup("client");
    setting.setValue(data,QVariant(filepath));
    setting.endGroup();
}

void XmppClient::deletereceiveconfigdata(QXmppTransferJob *job)
{
    QByteArray ba = job->fileHash().toBase64();
    QString data = job->fileName() + job->jid() + QString(ba);
    //检查配置文件中数据记录
    QSettings setting(CONFIGPATH,QSettings::IniFormat);
    setting.beginGroup("client");
    setting.remove(data);
    setting.endGroup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
 * Function:    公告发送接口函数
 * Description:
 * Cells:       有UI 端进行调用
 * Input:       需要发送的公告内容作为参数
 * Output:
 * Return:
 * Other:
 * ******************************************************************/
bool XmppClient::sendNotice(NoticeBase &notice)
{
    QString NoticeBody;

    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(&NoticeBody);
    xmlWriter->writeStartElement("notice");
    xmlWriter->writeTextElement("style",QString::number(notice.NoticeStyle,10));
    xmlWriter->writeTextElement( "urgency", QString::number(notice.Urgency));
    xmlWriter->writeTextElement("important", QString::number(notice.Important));
    xmlWriter->writeTextElement("secret", QString::number(notice.Secret));
    if (!notice.Subject.isEmpty())
        xmlWriter->writeTextElement("subject", notice.Subject);
    if (!notice.Appellation.isEmpty())
        xmlWriter->writeTextElement("appellation",notice.Appellation);
    if (!notice.Reason.isEmpty())
        xmlWriter->writeTextElement("reason", notice.Reason);
    if (!notice.Thing.isEmpty())
        xmlWriter->writeTextElement("thing",notice.Thing);
    if(!notice.Peroration.isEmpty())
        xmlWriter->writeTextElement("peroration",notice.Peroration);
    if(!notice.Organization.isEmpty())
        xmlWriter->writeTextElement("organization",notice.Organization);
    if(!notice.date.isEmpty())
        xmlWriter->writeTextElement("date",notice.date);

    xmlWriter->writeEndElement();

    qDebug()<<NoticeBody;
    foreach(QString Jid, notice.toID)
    {
       QXmppMessage NoticeToMsg("", Jid, NoticeBody);
       NoticeToMsg.setStamp(QDateTime::currentDateTime());
       NoticeToMsg.setType(QXmppMessage::Headline);
       bool re = client->sendPacket(NoticeToMsg);
       if(!re) {
           return re;
       }
    }
    return false;
}

/********************************************************************
 * Function:    接收公告处理函数
 * Description:
 * Cells:       当接收到公告信息是调用该函数
 * Input:
 * Output:      其向UI端发送信息，参数为公告内容
 * Return:
 * Other:
 * ******************************************************************/
void XmppClient::NoticeHanding(QString NoticeBody)
{

    //qDebug()<<"NoticeHanding";
    NoticeBase notice;
    QVariant tempvalue;
    QXmlStreamReader xmlReader;
    xmlReader.addData(NoticeBody);

    xmlReader.readNext();
    xmlReader.readNext();
    if(xmlReader.isStartElement())
    {
        if(xmlReader.name()=="notice")
        {
            while(!xmlReader.atEnd() && !xmlReader.hasError())
            {

                xmlReader.readNext();
                if(xmlReader.isStartElement())
                {
                    if(xmlReader.name()=="notice")
                    {
                        //qDebug()<<"————notice————";
                    }else if (xmlReader.name()=="style") {
                        switch(xmlReader.readElementText().toInt()){
                            case 0:
                                notice.NoticeStyle = NoticeBase::Instructions;
                            case 1:
                                notice.NoticeStyle = NoticeBase::RequestInstructions;
                        }

                    }else if (xmlReader.name()=="urgency") {
                        tempvalue = xmlReader.readElementText();
                        notice.Urgency = tempvalue.toBool();
                        //qDebug()<<QString::number(notice.Urgency);

                    }else if (xmlReader.name()=="important"){
                        tempvalue = xmlReader.readElementText();
                        notice.Important = tempvalue.toBool();
                        //qDebug()<<QString::number(notice.Important);

                    }else if (xmlReader.name()=="secret") {
                        tempvalue = xmlReader.readElementText();
                        notice.Secret = tempvalue.toBool();
                        //qDebug()<<QString::number(notice.Secret);

                    }else if (xmlReader.name()=="subject") {
                        notice.Subject = xmlReader.readElementText();
                        //qDebug()<<notice.Subject;

                    }else if (xmlReader.name()=="appellation"){
                        notice.Appellation =xmlReader.readElementText();
                        //qDebug()<<notice.Appellation;

                    }else if (xmlReader.name()=="reason"){
                        notice.Reason = xmlReader.readElementText();
                        //qDebug()<<xmlReader.readElementText();

                    }else if (xmlReader.name()=="thing") {
                        notice.Thing = xmlReader.readElementText();
                        //qDebug()<<notice.Thing;

                    }else if (xmlReader.name()=="peroration") {
                        notice.Peroration = xmlReader.readElementText();
                        //qDebug()<<notice.Peroration;

                    }else if (xmlReader.name()=="organization"){
                        notice.Organization = xmlReader.readElementText();
                        //qDebug()<<notice.Organization;

                    }else if (xmlReader.name()=="date") {
                        notice.date = xmlReader.readElementText();
                        //qDebug()<<notice.date;
                    }
                }
            }//end while

            emit NoticeReceived(notice, NoticeBody);
        }//end if(xmlReader.name()=="notice")
        else
        {
            return ;
        }
    }//end if(xmlReader.isStartElement())
}


QString XmppClient::getImageType(const QByteArray &contents)
{
    if (contents.startsWith("\x89PNG\x0d\x0a\x1a\x0a"))
        return "image/png";
    else if (contents.startsWith("\x8aMNG"))
        return "video/x-mng";
    else if (contents.startsWith("GIF8"))
        return "image/gif";
    else if (contents.startsWith("BM"))
        return "image/bmp";
    else if (contents.contains("/* XPM */"))
        return "image/x-xpm";
    else if (contents.contains("<?xml") && contents.contains("<svg"))
        return "image/svg+xml";
    else if (contents.startsWith("\xFF\xD8\xFF\xE0"))
        return "image/jpeg";
    return "image/unknown";
}
