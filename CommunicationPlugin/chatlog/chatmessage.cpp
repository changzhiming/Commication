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

#include "chatmessage.h"
#include "chatlinecontentproxy.h"
#include "content/text.h"
#include "content/timestamp.h"
#include "content/spinner.h"
#include "content/filetransferwidget.h"
#include "content/image.h"
#include "content/notificationicon.h"
#include <QTextBrowser>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include "savesetting.h"
#include "style/smileypack.h"
#include "core/core.h"
#include "style/style.h"
#include <QList>

#define NAME_COL_WIDTH 90.0
#define TIME_COL_WIDTH 90.0

ChatMessage::ChatMessage()
{

}
ChatMessage::Ptr ChatMessage::createSenDataMessage(const QString &sender, const QDateTime &date)      //创建消息头
{
    ChatMessage::Ptr msg = ChatMessage::Ptr(new ChatMessage);
    QString senderdate=sender +"  "+date.toString(SaveSetting::getInstance().getTimestampFormat());
    msg->addColumn(new Text(senderdate, Style::getFont(Style::BigBold), false, senderdate,  Qt::blue), ColumnFormat(NAME_COL_WIDTH, ColumnFormat::VariableSize, ColumnFormat::Left));
    msg->addColumn(new Text("", Style::getFont(Style::Big), true, "",  Qt::black), ColumnFormat(30, ColumnFormat::FixedSize, ColumnFormat::Left));
    msg->addColumn(new Text("", Style::getFont(Style::Big), true, "",  Qt::black), ColumnFormat(30, ColumnFormat::FixedSize, ColumnFormat::Left));

    //msg->addColumn(new Timestamp(date, SaveSetting::getInstance().getTimestampFormat(), Style::getFont(Style::ExtraBig)), ColumnFormat(TIME_COL_WIDTH, ColumnFormat::FixedSize, ColumnFormat::Left));
    return msg;
}

ChatMessage::Ptr ChatMessage::createHistoryMessage(Text *text)
{
    ChatMessage::Ptr msg = ChatMessage::Ptr(new ChatMessage);

    msg->addColumn(new Text("", Style::getFont(Style::BigBold), false, "",  Qt::black), ColumnFormat(NAME_COL_WIDTH, ColumnFormat::FixedSize, ColumnFormat::Left));
    msg->addColumn(text, ColumnFormat(1.0, ColumnFormat::VariableSize, ColumnFormat::Center));
    msg->addColumn(new Text("", Style::getFont(Style::Big), true, "",  Qt::black), ColumnFormat(NAME_COL_WIDTH, ColumnFormat::FixedSize, ColumnFormat::Left));

    return msg;
}

ChatMessage::Ptr ChatMessage::createChatMessage(const QString& rawMessage, QString richtext)
{
    ChatMessage::Ptr msg = ChatMessage::Ptr(new ChatMessage);

    //text = detectMarkdown(rawMessage);  //发现隐含的连接
    bool showhis=false;
    QString message=rawMessage;
    if((showhis=message.contains("////log"))==true)
    {
        message.replace("////log", "");
    }
    // Note: Eliding cannot be enabled for RichText items. (QTBUG-17207)
    msg->addColumn(new Text(".", Style::getFont(Style::ExtraBig), true, "",  Qt::black), ColumnFormat(35, ColumnFormat::FixedSize, ColumnFormat::Right));
    msg->addColumn(new Text(richtext , Style::getFont(Style::Big), false, message), ColumnFormat(1.0, ColumnFormat::VariableSize));
    msg->addColumn(new Spinner(":/icon/css/Icon/spinner.svg", QSize(15, 15), 360.0/1.6), ColumnFormat(30, ColumnFormat::FixedSize, ColumnFormat::Left));

    if (Core::getInstance()->xmppClient->currentState==QXmppClient::ConnectedState||showhis)  //看是否联网发送成功
        msg->markAsSent(QDateTime());               //替换图标

    return msg;
}

ChatMessage::Ptr ChatMessage::createChatInfoMessage(const QString &rawMessage, SystemMessageType type, const QDateTime &date)
{
    ChatMessage::Ptr msg = ChatMessage::Ptr(new ChatMessage);
    QString text = rawMessage.toHtmlEscaped();

    QString img;
    switch(type)
    {
    case INFO:   img = ":/icon/css/Icon/info.svg";     break;
    case ERROR:  img = ":/icon/css/Icon/error.svg";    break;
    case TYPING: img = ":/icon/css/Icon/typing.svg";   break;
    }

   /* msg->addColumn(new Image(QSize(18, 18), img), ColumnFormat(NAME_COL_WIDTH, ColumnFormat::FixedSize, ColumnFormat::Right));
    msg->addColumn(new Text("<b>" + text + "</b>", Style::getFont(Style::Big), false, ""), ColumnFormat(1.0, ColumnFormat::VariableSize, ColumnFormat::Left));
    msg->addColumn(new Timestamp(date, SaveSetting::getInstance().getTimestampFormat(), Style::getFont(Style::Big)), ColumnFormat(TIME_COL_WIDTH, ColumnFormat::FixedSize, ColumnFormat::Right));
*/
    msg->addColumn(new Text("hello", Style::getFont(Style::Big), true, "",  Qt::black), ColumnFormat(60, ColumnFormat::FixedSize, ColumnFormat::Left));
    msg->addColumn(new Image(QSize(90, 90), img), ColumnFormat(300, ColumnFormat::FixedSize, ColumnFormat::Right));
    msg->addColumn(new Text("hello", Style::getFont(Style::Big), true, "",  Qt::black), ColumnFormat(60, ColumnFormat::FixedSize, ColumnFormat::Left));

    return msg;
}

ChatMessage::Ptr ChatMessage::createFileTransferMessage(const QString& sender, QString file, bool isMe, QXmppTransferJob *job, const QDateTime& date)
{
    ChatMessage::Ptr msg = ChatMessage::Ptr(new ChatMessage);
    msg->addColumn(new Text(sender, Style::getFont(Style::Big), true), ColumnFormat(30, ColumnFormat::FixedSize, ColumnFormat::Right));
    msg->addColumn(new ChatLineContentProxy(new FileTransferWidget(0, file, isMe, job), 300, 0.6f), ColumnFormat(1.0, ColumnFormat::VariableSize, ColumnFormat::Center));
    msg->addColumn(new Timestamp(date, SaveSetting::getInstance().getTimestampFormat(), Style::getFont(Style::Big)), ColumnFormat(30, ColumnFormat::FixedSize, ColumnFormat::Right));
    return msg;
}

ChatMessage::Ptr ChatMessage::createTypingNotification()
{
    ChatMessage::Ptr msg = ChatMessage::Ptr(new ChatMessage);

    // Note: "[user]..." is just a placeholder. The actual text is set in ChatForm::setFriendTyping()
    //
    // FIXME: Due to circumstances, placeholder is being used in a case where
    // user received typing notifications constantly since contact came online.
    // This causes "[user]..." to be displayed in place of user nick, as long
    // as user will keep typing. Issue #1280
    msg->addColumn(new NotificationIcon(QSize(18, 18)), ColumnFormat(NAME_COL_WIDTH, ColumnFormat::FixedSize, ColumnFormat::Right));
    msg->addColumn(new Text("[user]...", Style::getFont(Style::Big), false, ""), ColumnFormat(1.0, ColumnFormat::VariableSize, ColumnFormat::Left));

    return msg;
}
ChatMessage::Ptr ChatMessage::createImage(QSize &size, QImage &image)
{
    ChatMessage::Ptr msg = ChatMessage::Ptr(new ChatMessage);

    msg->addColumn(new Text("", Style::getFont(Style::Big), true, "",  Qt::black), ColumnFormat(30, ColumnFormat::FixedSize, ColumnFormat::Left));
    msg->addColumn(new Image(size, image), ColumnFormat(1.0, ColumnFormat::VariableSize));
    msg->addColumn(new Text("", Style::getFont(Style::Big), true, "",  Qt::black), ColumnFormat(30, ColumnFormat::FixedSize, ColumnFormat::Left));

    return msg;
}

ChatMessage::Ptr ChatMessage::createBusyNotification()
{
    ChatMessage::Ptr msg = ChatMessage::Ptr(new ChatMessage);

    // TODO: Bigger font
    msg->addColumn(new Text(QObject::tr("Resizing"), Style::getFont(Style::ExtraBig), false, ""), ColumnFormat(1.0, ColumnFormat::VariableSize, ColumnFormat::Center));

    return msg;
}

void ChatMessage::markAsSent(const QDateTime &time)
{
    // remove the spinner and replace it by $time
    replaceContent(2, new Timestamp(time, SaveSetting::getInstance().getTimestampFormat(), Style::getFont(Style::Big)));
}

QString ChatMessage::toString() const
{
    ChatLineContent* c = getContent(1);
    if (c)
        return c->getText();

    return QString();
}

bool ChatMessage::isAction() const
{
    return action;
}

void ChatMessage::setAsAction()
{
    action = true;
}

void ChatMessage::hideSender()
{
    ChatLineContent* c = getContent(0);
    if (c)
        c->hide();
}

void ChatMessage::hideDate()
{
    ChatLineContent* c = getContent(2);
    if (c)
        c->hide();
}

QString ChatMessage::detectMarkdown(const QString &str)
{
    QString out = str;

    // Create regex for certain markdown syntax
    QRegExp exp("(\\*\\*)([^\\*\\*]{2,})(\\*\\*)"   // Bold    **text**
                "|(\\*)([^\\*]{2,})(\\*)"           // Italics *text*
                "|(\\_)([^\\_]{2,})(\\_)"           // Italics _text_
                "|(\\_\\_)([^\\_\\_]{2,})(\\_\\_)"  // Bold    __text__
                "|(\\-)([^\\-]{2,})(\\-)"           // Underline  -text-
                "|(\\~)([^\\~]{2,})(\\~)"           // Strike  ~text~
                "|(\\~~)([^\\~\\~]{2,})(\\~~)"      // Strike  ~~text~~
                "|(\\`)([^\\`]{2,})(\\`)"           // Codeblock  `text`
                );

    int offset = 0;
    while ((offset = exp.indexIn(out, offset)) != -1)
    {
        QString snipCheck = out.mid(offset-1,exp.cap(0).length()+2);
        QString snippet = exp.cap(0).trimmed();
        QString htmledSnippet;

        // Only parse if surrounded by spaces, newline(s) and/or beginning/end of line
        if ((snipCheck.startsWith(' ') || snipCheck.startsWith('>') || offset == 0)
            && ((snipCheck.endsWith(' ') || snipCheck.endsWith('<')) || offset + snippet.length() == out.length()))
        {
            int mul = 0; // Determines how many characters to strip from markdown text
            // Set mul depending on markdownPreferenc

            // Match captured string to corresponding md format
            if (exp.cap(1) == "**") // Bold **text**
                htmledSnippet = QString("<b>%1</b>").arg(snippet.mid(mul,snippet.length()-2*mul));
            else if (exp.cap(4) == "*" && snippet.length() > 2) // Italics *text*
                htmledSnippet = QString("<i>%1</i>").arg(snippet.mid(mul/2,snippet.length()-mul));
            else if (exp.cap(7) == "_" && snippet.length() > 2) // Italics _text_
                htmledSnippet = QString("<i>%1</i>").arg(snippet.mid(mul/2,snippet.length()-mul));
            else if (exp.cap(10) == "__"&& snippet.length() > 4) // Bold __text__
                htmledSnippet = QString("<b>%1</b>").arg(snippet.mid(mul,snippet.length()-2*mul));
            else if (exp.cap(13) == "-" && snippet.length() > 2) // Underline -text-
                htmledSnippet = QString("<u>%1</u>").arg(snippet.mid(mul/2,snippet.length()-mul));
            else if (exp.cap(16) == "~" && snippet.length() > 2) // Strikethrough ~text~
                htmledSnippet = QString("<s>%1</s>").arg(snippet.mid(mul/2,snippet.length()-mul));
            else if (exp.cap(19) == "~~" && snippet.length() > 4) // Strikethrough ~~text~~
                htmledSnippet = QString("<s>%1</s>").arg(snippet.mid(mul,snippet.length()-2*mul));
            else if (exp.cap(22) == "`" && snippet.length() > 2) // Codeblock `text`
                htmledSnippet = QString("<font color=#595959><code>%1</code></font>").arg(snippet.mid(mul/2,snippet.length()-mul));
            else
                htmledSnippet = snippet;
            out.replace(offset, exp.cap().length(), htmledSnippet);
            offset += htmledSnippet.length();
        } else
            offset += snippet.length();
    }

    return out;
}

QString ChatMessage::detectAnchors(const QString &str)
{
    QString out = str;

    // detect URIs
    QRegExp exp("("
                "(?:\\b)((www\\.)|(http[s]?|ftp)://)" // (protocol)://(printable - non-special character)
                // http://ONEORMOREALHPA-DIGIT
                "\\w+\\S+)" // any other character, lets domains and other
                // ↓ link to a file, or samba share
                //   https://en.wikipedia.org/wiki/File_URI_scheme
                "|(?:\\b)((file|smb)://)([\\S| ]*)"
                "|(?:\\b)(tox:[a-zA-Z\\d]{76})" //link with full user address
                "|(?:\\b)(mailto:\\S+@\\S+\\.\\S+)" //@mail link
                "|(?:\\b)(tox:\\S+@\\S+)"); // starts with `tox` then : and only alpha-digits till the end
                // also accepts tox:agilob@net as simplified TOX ID

    int offset = 0;
    while ((offset = exp.indexIn(out, offset)) != -1)
    {
        QString url = exp.cap();
        // If there's a trailing " it's a HTML attribute, e.g. a smiley img's title=":tox:"
        if (url == "tox:\"")
        {
            offset += url.length();
            continue;
        }
        QString htmledUrl;
        // add scheme if not specified
        if (exp.cap(2) == "www.")
            htmledUrl = QString("<a href=\"http://%1\">%1</a>").arg(url);
        else
            htmledUrl = QString("<a href=\"%1\">%1</a>").arg(url);
        out.replace(offset, exp.cap().length(), htmledUrl);
        offset += htmledUrl.length();
    }

    return out;
}

QString ChatMessage::detectQuotes(const QString& str, MessageType type)
{
    // detect text quotes
    QStringList messageLines = str.split("\n");
    QString quotedText;
    for (int i = 0; i < messageLines.size(); ++i)
    {
        // don't quote first line in action message. This makes co-existence of
        // quotes and action messages possible, since only first line can cause
        // problems in case where there is quote in it used.
        if (QRegExp("^(&gt;|＞).*").exactMatch(messageLines[i])) {
            if (i > 0 || type != ACTION)
                quotedText += "<span class=quote>" + messageLines[i] + "</span>";
            else
                quotedText += messageLines[i];
        } else {
            quotedText += messageLines[i];
        }

        if (i < messageLines.size() - 1)
            quotedText += "<br/>";
    }

    return quotedText;
}

QString ChatMessage::wrapDiv(const QString &str, const QString &div)
{
    return QString("<p class=%1>%2</p>").arg(div, /*QChar(0x200E) + */QString(str));
}
