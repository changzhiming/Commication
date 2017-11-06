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

#ifndef CORE_HPP
#define CORE_HPP

#include <cstdint>
#include <QObject>
#include <QMutex>
#include <QDateTime>
#include "chatlog/chatlinecontent.h"
#include "chatlog/chatline.h"
#include "chatwindow.h"
#include "xmpp/xmppclient.h"
#include <QSqlDatabase>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class QTimer;
class Message;
class Transfer;
struct vpx_image;
class XmppClient;
class QXmppTransferJob;
class simple_pjsua;

struct historyMessage{
    QString Id;          //聊天窗口的Id
    QString senderName;        //发送者名字
    chatWindow::MessageType messagetype;    //消息类型
    QDateTime datetime;  //时间
    QString message;      //消息
};

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject* parent=0);
    ~Core();

    static Core* getInstance(); ///< Returns the global widget's Core instance
    static QString getStylesheet(const QString &filename);
    QString analysisHtmlContainPicture(QString html, QString sendId, QString myId);
    void playSound();
    void playSound(QString soundPath);
    void stopSound();             //停止播放声音

Q_SIGNALS:
    void signalInitXmpp();        //初始化xmpp

public Q_SLOTS:

    bool sendMessage(FriendDataMessage message);         //发送消息
    bool sendFile(QString path, QString username, bool isPicture);
    void initxmpp();
    void coreaddRoom(const QString &roomJid,const QString &Subject,const QString &password);
    void openDb(QString dbName);
    void insertMessageToDb(struct historyMessage *message);
    QList<struct historyMessage> readMessageFromDb(QDateTime dateTime, QString Id);
public:
    XmppClient *xmppClient;
    QSqlDatabase db;                    //数据库
    simple_pjsua *sipPhone=nullptr;     //sip电话
    QMediaPlayer * mediaPlayer;        //多媒体播放
    QMediaPlaylist *playlist;

private:
    QMutex messageMutex;                 //互斥锁
    static QThread *coreThread;          //执行线程
    static Core*core;                    //指向自己的指针
    QTimer *toxTimer;
    friend class chatWindow;
    friend class FriendList;
    friend class MyListView;
};

#endif // CORE_HPP

