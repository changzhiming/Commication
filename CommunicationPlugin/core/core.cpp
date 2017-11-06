/*
    Copyright (C) 2013 by Maxim Biro <nurupo.contributions@gmail.com>
    Copyright © 2014-2015 by The qTox Project

    This file is part of qTox, a Qt-based graphical interface for Tox.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qTox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qTox.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "core.h"
#include "savesetting.h"
#include "chatlog/chatmessage.h"
#include "listFriend/qqpanel.h"
#include "friendlist.h"
#include "sip/qsip.h"
#include <ctime>
#include <cassert>
#include <limits>
#include <functional>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QStandardPaths>
#include <QThread>
#include <QTimer>
#include <QCoreApplication>
#include <QDateTime>
#include <QList>
#include <QBuffer>
#include <QMutexLocker>
#include <QSqlQuery>
#include <QFile>

QThread* Core::coreThread{nullptr};
Core *Core::core{nullptr};

Core::Core(QObject *parent)
{
    //创建核心线程
    coreThread=new QThread(this);
    coreThread->setObjectName(tr("core thread"));
    coreThread->start();
    qDebug()<<"corethread start";
    core=this;

    connect(this, SIGNAL(signalInitXmpp()), this, SLOT(initxmpp()), Qt::QueuedConnection);

    sipPhone=new simple_pjsua(this);
    if(sipPhone->SipInit()!=0)
    {
        qDebug()<<"sip init fail";
        delete sipPhone;
        sipPhone=nullptr;
    }
    else
    {
        qDebug()<<"sip init success";                 //2016.6.30 sip初始化问题
        int i=sipPhone->RegisterSIP(SaveSetting::getInstance().getSipUserName(), SaveSetting::getInstance().getSipPassword(), SaveSetting::getInstance().getSipServer());
    }
   // sipPhone=nullptr;

    mediaPlayer=new QMediaPlayer(this);   //初始化媒体播放
    playlist=new QMediaPlaylist(this);

    moveToThread(coreThread);
    emit signalInitXmpp();
}

Core* Core::getInstance()
{
    if(!core) {
        core=new Core();
    }
    return core;
}
Core::~Core()
{
    qDebug()<<"delete core"<<QThread::currentThread()->objectName();
    db.close();
    xmppClient->deleteLater();
    coreThread->exit(0);
    coreThread->wait();
    //coreThread->deleteLater();
}

 bool Core::sendMessage(FriendDataMessage message)
 {
     qDebug()<<QThread::currentThread()->objectName();
     xmppClient->sendmsg(message);
     return true;
 }

bool Core::sendFile(QString path, QString username, bool isPicture)
{
    if (QThread::currentThread() != coreThread){
          return QMetaObject::invokeMethod(this, "sendFile", Qt::QueuedConnection, Q_ARG(QString, path), Q_ARG(QString, username)
                                           , Q_ARG(bool, isPicture));
        }
    return  xmppClient->sendfile(path, username, isPicture);
}
void Core::coreaddRoom(const QString &roomJid,const QString &Subject,const QString &password)
{
    QXmppMucRoom *room=xmppClient->addRoom(roomJid,Subject,password);
    qDebug()<<room->participants()<<room;
}

void Core::initxmpp()
{
    qDebug()<<"initxmpp"<<QThread::currentThread()->objectName();
    xmppClient=new XmppClient();  //初始化xmpp
}
void Core::openDb(QString dbName)
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setConnectOptions("QSQLITE_ENABLE_SHARED_CACHE=1");
    db.setDatabaseName(SaveSetting::getInstance().getAppCacheDirPath()+dbName);
    if (!db.open())
    {
        qWarning() << QString("Can't open file: %1, history will not be saved!").arg(dbName);
        db.setDatabaseName(":memory:");
        db.setConnectOptions();
        db.open();
    }
    db.exec("create table IF NOT EXISTS history(id varchar, messagetype INTEGER, sendername varchar, message varchar, datetime INTEGER)");       //创建数据表
}
void Core::insertMessageToDb(historyMessage *message)
{
    qDebug()<<"insettMedageToDb"<<message->Id;
    message->message.replace("'", "\\\"");                  //单引号替换成\'；
    QSqlQuery query(db);
    if(query.exec(tr("insert into history values('%1', %2, '%3', '%4', %5)").\
            arg(message->Id).arg((int)message->messagetype).arg(message->senderName).arg(message->message).arg(message->datetime.toMSecsSinceEpoch())))
    {
        qDebug()<<"insert into success"<<message->datetime.toString();
    }

    delete message;
}

QList<historyMessage> Core::readMessageFromDb(QDateTime dateTime, QString Id)
{
    QList<historyMessage> history;
//1474167766898
    QSqlQuery query=db.exec(tr("select * from history where Id='%1' and datetime<%2").arg(Id).arg(dateTime.toMSecsSinceEpoch()));
    query.last();        //移送到最后
    query.seek(query.at()+1-20);
    while(query.next())
    {

        historyMessage message;
        message.Id=query.value(0).toInt();
        message.messagetype=(chatWindow::MessageType)query.value(1).toInt();
        message.senderName=query.value(2).toString();
        message.message=query.value(3).toString().replace("\\\"", "'"),
        message.datetime= QDateTime::fromMSecsSinceEpoch(query.value(4).toLongLong());
        history.append(message);

    }
    return history;
}
void Core::playSound()
{
    qDebug()<<"sssssssss";
    playlist->addMedia(QUrl("qrc:/voice/ringin.wav"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    mediaPlayer->setPlaylist(playlist);
    mediaPlayer->play();
}
void Core::playSound(QString soundPath)
{
    mediaPlayer->setMedia(QUrl(soundPath));
    mediaPlayer->play();
}

void Core::stopSound()
{
    mediaPlayer->stop();
}

QString Core::getStylesheet(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "Stylesheet " << filename << " not found";
        return QLatin1String(nullptr);
    }
    return file.readAll();
}
QString Core::analysisHtmlContainPicture(QString html, QString sendId, QString myId)
{
    QStringList list;
    QRegExp regexp("(temp_picture/mypic\\d{1,5}.png)");    //匹配图片名称
    int pos=0;
    while((pos=regexp.indexIn(html, pos))!=-1)
    {
        qDebug()<<regexp.cap(1);
        list<<regexp.cap(1);
        pos+=regexp.matchedLength();
    }
    foreach(QString path, list) {
        sendFile(path, sendId, true);  //发送文件
    }
    html.replace(tr("temp_picture"), "receive_picture/"+myId);
    QThread::msleep(100);
    return html;
}
