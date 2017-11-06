/*
    Copyright © 2014-2015 by The qTox Project

    This file is part of qTox, a Qt-based graphical interface for Tox.

    qTox is libre software: you can redistribute it and/or modify
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

#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include "chatline.h"
#include <QDateTime>
#include <QImage>
class QLabel;
class Text;
class QXmppTransferJob;
class ChatMessage : public ChatLine
{
public:
    using Ptr = std::shared_ptr<ChatMessage>;

    enum SystemMessageType
    {
        INFO,
        ERROR,
        TYPING,
    };

    enum MessageType
    {
        NORMAL,
        ACTION,
        ALERT,
    };

    ChatMessage();

    static ChatMessage::Ptr createSenDataMessage(const QString& sender, const QDateTime &date = QDateTime()); //创建发送头  
    static ChatMessage::Ptr createHistoryMessage(Text *text); //创建发送头
    static ChatMessage::Ptr createChatMessage(const QString& rawMessage, QString richtext);//创建文本消息
    static ChatMessage::Ptr createChatInfoMessage(const QString& rawMessage, SystemMessageType type, const QDateTime& date);
    static ChatMessage::Ptr createFileTransferMessage(const QString& sender, QString file, bool isMe, QXmppTransferJob *job,const QDateTime& date); //创建文见传输

    static ChatMessage::Ptr createTypingNotification();
    static ChatMessage::Ptr createBusyNotification();
    static ChatMessage::Ptr createImage(QSize &size, QImage &image);

    void markAsSent(const QDateTime& time);
    QString toString() const;
    bool isAction() const;
    void setAsAction();
    void hideSender();
    void hideDate();

protected:
    static QString detectMarkdown(const QString& str);
    static QString detectAnchors(const QString& str);
    static QString detectQuotes(const QString& str, MessageType type);
    static QString wrapDiv(const QString& str, const QString& div);
private:
    bool action = false;
};

#endif // CHATMESSAGE_H
