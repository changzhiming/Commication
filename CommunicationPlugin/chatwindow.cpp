#include "chatwindow.h"
#include "ui_chatwindow.h"

#include "keyboard.h"
#include "listFriend/my_listmodel.h"
#include "friendlist.h"
#include "mytextedit.h"
#include "chatlog/chatlog.h"
#include "tool/croppinglabel.h"
#include "savesetting.h"
#include "chatlog/content/text.h"
#include "core/core.h"
#include "style/style.h"
#include "tool/facewidget.h"
#include "sip/qsip.h"
#include "QXmppMessage.h"
#include "tool/screenshotpart.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QFileInfo>
#include <QKeyEvent>
#include <QDebug>
#include <QTextBrowser>
#include <QTableView>
chatWindow::chatWindow(QWidget *parent, ListItemData *frienddata) :
    QWidget(parent),ui(new Ui::chat_win),friendData(frienddata)
{
    init();   //init setting
    if(frienddata->vcard.VoIP.isEmpty())
    {
        ui->toolButtonSip->setDisabled(true);
        ui->toolButtonSip->setToolTip(tr("对方没有sip账号"));
        ui->toolButtonSip->setIcon(QIcon(tr(":/statu/image/status/禁用.png")));
    }

}
chatWindow::chatWindow(QWidget *parent):QWidget(parent),ui(new Ui::chat_win)//, QRoom *room
{
    init();
    friendData=nullptr;

}
void chatWindow::init()
{
    ui->setupUi(this);
    textEdit=new MyTextEdit(ui->frame);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
    textEdit->setSizePolicy(sizePolicy);
    textEdit->setMinimumHeight(100);
    textEdit->setMaximumHeight(100);

    textEdit->setFrameShape(QFrame::NoFrame);
    textEdit->setFrameShadow(QFrame::Raised);
    textEdit->installEventFilter(this);

    QFrame *frame = new QFrame(this);
    frame->setObjectName(QStringLiteral("frame2"));
    frame->setStyleSheet(
                tr("QFrame{background-color: rgb(255, 255, 255);} QToolButton {border-style: outset;border-width: 2px;border-radius: 10px;\
                      height: 10px;\
                      width: 50px;\
                      font: bold \"Times New Roman\" 10px;\
                       color: white;\
                        padding: 6px;\
                        border-image: url(:/icon/image/发送t.png);\
                         }\
                         QToolButton:pressed {\
                           border-image: url(:/icon/a.png);\
                           background-color: red;\
                            }"));

    frame->setFrameShape(QFrame::NoFrame);
    frame->setFrameShadow(QFrame::Raised);
    QHBoxLayout *horizontalLayout_6 = new QHBoxLayout(frame);
    horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
    QSpacerItem *horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_6->addItem(horizontalSpacer_2);

    QToolButton *toolButton_2 = new QToolButton(frame);
    toolButton_2->setObjectName(QStringLiteral("toolButton_2"));

    toolButton_2->setAutoRaise(true);
    toolButton_2->setText(tr("发送"));
    QToolButton *toolButton_3 = new QToolButton(frame);
    toolButton_3->setObjectName(QStringLiteral("toolButton_3"));
    toolButton_3->setAutoRaise(true);
    toolButton_3->setText(tr("关闭"));

    horizontalLayout_6->addWidget(toolButton_3);
    horizontalLayout_6->addWidget(toolButton_2);


    //ui->sendFileToolbutton->setIcon(this->style()->standardIcon(QStyle::StandardPixmap(42)));
    ui->horizontalWidget->hide();             //隐藏字体设置

    /**********************初始化设置**********************************/
    //初始化定时器
    timer=new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    /***************************************************/

    ui->labelCallTime->hide();                                   //隐藏时间
    if(Core::getInstance()->sipPhone==nullptr)
    {
            ui->toolButtonSip->setDisabled(true);
            ui->toolButtonSip->setToolTip(tr("sip 初始化失败"));
            ui->toolButtonSip->setIcon(QIcon(tr(":/statu/image/status/禁用.png")));
    }

    ui->verticalLayout->addWidget(textEdit);  //初始化编辑窗体
    ui->verticalLayout->addWidget(frame);  //发送按钮
    chatwindow=new ChatLog(this);            //初始化消息显示船体
    chatwindow->setBackgroundBrush(QColor(0xf6, 0xfc, 0xff));

    chatwindow->setFrameShape(QFrame::NoFrame);
    ui->verticalLayout_3->addWidget(chatwindow);  //初始化Graphicview窗体
//************字体初始化******************************/////
    on_fontComboBox_currentFontChanged(QFont(SaveSetting::getInstance().getFontFamily()));
    on_fontSizeComboBox_currentIndexChanged(QString::number(SaveSetting::getInstance().getFontSize()));
    on_fontWeightToolbutton_clicked(SaveSetting::getInstance().getFontWidget());
    on_fontItalicToolbutton_clicked(SaveSetting::getInstance().getFontItalic());
    on_fontUnderlineToolButton_clicked(SaveSetting::getInstance().getFontUnderline());
    //添加查看历史文字
    /*label->setStyleSheet("background-color: rgb(255, 255, 255);color: blue;font: "\
                         "bold italic \"Times New Roman\";text-align: center;text-decoration: underline;");*/
    text=new Text("查看历史消息", Style::getFont(Style::Big), true, "查看历史消息",  Qt::blue);
    chatwindow->insertChatlineOnTop(ChatMessage::createHistoryMessage(text));

    historyDataTime=QDateTime::currentDateTime();
    faceWidget=new FaceWidget(this);                 //初始化表情窗体

    connect(text, SIGNAL(showHistoryMessage()), this, SLOT(showHistoryMessage())); //显示历史消息
    connect(textEdit, SIGNAL(return_sendMessage()),this, SLOT(onsendMessage()));  //回车发送消息
    connect(this, SIGNAL(sendMessage(FriendDataMessage)), Core::getInstance(), SLOT(sendMessage(FriendDataMessage)));  //核心发送消息
    connect(this, SIGNAL(sengFile(QString,QString, bool)),Core::getInstance(), SLOT(sendFile(QString,QString, bool)));  //发送文件
    connect(this, SIGNAL(insertMessageToDb(historyMessage *)), Core::getInstance(), SLOT(insertMessageToDb(historyMessage *)));  //存储聊天记录到sqlite

    connect(faceWidget, SIGNAL(sendFacePath(QString)), this, SLOT(reciveFacePath(QString)));

    connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
    connect(toolButton_2, SIGNAL(clicked()), this, SLOT(sendButton()));
    connect(toolButton_3, SIGNAL(clicked()), this, SLOT(closeButton()));
}

chatWindow::~chatWindow()
{
    qDebug()<<"delete chat-win";
    delete ui;
}
void chatWindow::setWindowType(chatWindow::WindowType windowype)
{
    this->windowtype = windowype;
}
bool chatWindow::onsendRoomMessage()  //ROOM send message
{
    foreach(RoomListData *data ,FriendList::friendlist->roomListData)
    {
        if(data->groupChat->getChatLog()==this)
        {
            QString message=textEdit->toHtml();

            addTextMessage(FriendList::friendlist->myData->Name, message, message, QDateTime::currentDateTime());
            QStringList jidlist=data->room->participants();
            foreach(QString jid, jidlist)
            {
                QRegExp regexp("(.*)(/)");
                qDebug()<<data->room->participantFullJid((jid));
                int pos=regexp.indexIn(data->room->participantFullJid((jid)));
                if(pos>-1)
                {
                    message=Core::getInstance()->analysisHtmlContainPicture(message, regexp.cap(1), FriendList::friendlist->myData->Id);

                }
            }
            //message=Core::getInstance()->analysisHtmlContainPicture(message, friendData->Id, FriendList::friendlist->myData->Id);

            data->room->sendMessage(message);
            break;
        }
    }
    textEdit->clear();
    textEdit->setFocus();
    return true;
}
bool chatWindow::onsendMessage()
{
    if(!friendData)
        return onsendRoomMessage();

    QString message=textEdit->toHtml();

    addTextMessage(FriendList::friendlist->myData->Name, message, message, QDateTime::currentDateTime());

    FriendList::friendlist-> addItemToRecentFriend(friendData);       //insert into data to recently friend
    historyMessage *hismessage=new historyMessage{
            friendData->Id,
            FriendList::friendlist->myData->Name,
            chatWindow::_Text,
            QDateTime::currentDateTime(),
            message
    };
    emit insertMessageToDb(hismessage);                    //聊天记录增加到数据库

    message=Core::getInstance()->analysisHtmlContainPicture(message, friendData->Id, FriendList::friendlist->myData->Id);
    FriendDataMessage Message = {
        FriendList::friendlist->myData->Id,
        message,
        friendData->Id,
    };
    emit sendMessage(Message);

    textEdit->clear();
    textEdit->setFocus();


    return true;
}
void chatWindow::onCallSip()          //ROOM call
{

}
void chatWindow::on_toolButton_4_clicked()            //拨打显示电话
{
    if(friendData==nullptr)
    {
        onCallSip();
        return;
    }
    static bool calling=false;

    QString server=friendData->vcard.VoIP+"@"+SaveSetting::getInstance().getSipServer();
    qDebug()<<server;
    if(calling==false)
    {
        if(!Core::getInstance()->sipPhone->status)
        {
            Core::getInstance()->sipPhone->MakeCall(server);  //拨打电话
        }

        ui->toolButtonSip->setToolTip(tr("通话中"));
        ui->toolButtonSip->setIcon(QIcon(tr(":/statu/image/status/通话中.png")));
        ui->labelCallTime->show();            //显示时间显示
        timer->start();
        calling=true;
    }
    else
    {
        if(Core::getInstance()->sipPhone!=nullptr)
        {
        if(Core::getInstance()->sipPhone->status)
        {
            Core::getInstance()->sipPhone->hangup();     //挂断电话
        }
        }
        ui->toolButtonSip->setToolTip(tr("拨号"));
        ui->toolButtonSip->setIcon(QIcon(tr(":/statu/image/status/拨号.png")));
        ui->labelCallTime->hide();                    //隐藏时间显示
        timer->stop();
        calling=false;
    }

}

void chatWindow::addTextMessage(const QString &sender, const QString &rawMessage, const QString richtext, const QDateTime &date)
{

    if(messageForm!=sender)
    {
        chatwindow->insertChatlineAtBottom(ChatMessage::createSenDataMessage(sender, date));
    }
    chatwindow->insertChatlineAtBottom(ChatMessage::createChatMessage(rawMessage, richtext));
    messageForm=sender;
}

void chatWindow::addTextMessageInTop(const QString &sender, const QString &rawMessage, const QString richtext, const QDateTime &date)
{
    static QString messageForm=sender;
    QString message=rawMessage+"////log";
    if(messageForm==sender)
    {
        chatwindow->insertChatlineOnTop(ChatMessage::createChatMessage(message, richtext));
    }
    else
    {
        chatwindow->insertChatlineOnTop(ChatMessage::createSenDataMessage(messageForm, date));
        chatwindow->insertChatlineOnTop(ChatMessage::createChatMessage(message, richtext));
    }

    messageForm=sender;
}
void chatWindow::addFileTransferMessage(const QString &sender, QString file, bool isMe, QXmppTransferJob *job)
{
    if(messageForm!=sender)
    {
        chatwindow->insertChatlineAtBottom(ChatMessage::createSenDataMessage(sender, QDateTime::currentDateTime()));
    }

    chatwindow->insertChatlineAtBottom(ChatMessage::createFileTransferMessage("", file ,isMe, job, QDateTime()));
    messageForm=sender;

}
void chatWindow::addFileTransferMessageInTop(const QString &sender, QString file, bool isMe, QXmppTransferJob *job)
{

    static QString messageForm=sender;
    if(messageForm==sender)
    {
        chatwindow->insertChatlineOnTop(ChatMessage::createFileTransferMessage("", file ,isMe, job, QDateTime()));
    }
    else
    {
        chatwindow->insertChatlineOnTop(ChatMessage::createSenDataMessage(sender, QDateTime::currentDateTime()));
        chatwindow->insertChatlineOnTop(ChatMessage::createFileTransferMessage("", file ,isMe, job, QDateTime()));
    }

    messageForm=sender;
}
ChatLog * chatWindow::getChatLog()
{
    return chatwindow;
}

void chatWindow::on_fontComboBox_currentFontChanged(const QFont &f)  //更换字体
{
    qDebug()<<f.family()<<"font";

    textEdit->setCurrentFont(f);
    ui->fontComboBox->setCurrentText(f.family());
    SaveSetting::getInstance().setFontFamily(f.family());
    textEdit->setFocus();
}

void chatWindow::on_fontItalicToolbutton_clicked(bool checked)            //字体斜体
{
    qDebug()<<"fontItalic id checked"<<checked;
    textEdit->setFontItalic(checked);
    ui->fontItalicToolbutton->setChecked(checked);
    SaveSetting::getInstance().setFontItalic(checked);
    textEdit->setFocus();

}

void chatWindow::on_fontUnderlineToolButton_clicked(bool checked)   //字体下划线
{
    qDebug()<<"fontUnderline id checked";

    textEdit->setFontUnderline(checked);
    ui->fontUnderlineToolButton->setChecked(checked);
    SaveSetting::getInstance().setFontUnderline(checked);
    textEdit->setFocus();

}

void chatWindow::on_fontSizeComboBox_currentIndexChanged(const QString &size)   //改变字体大小
{
    qDebug()<<"font size"<<size;
    textEdit->setFontPointSize(size.toDouble());
    ui->fontSizeComboBox->setCurrentText(size);
    SaveSetting::getInstance().setFontSize(size.toDouble());
    textEdit->setFocus();
}

void chatWindow::on_fontWeightToolbutton_clicked(bool checked)       //字体加粗
{
    qDebug()<<"fontWidget id checked"<<checked;

    textEdit->setFontWeight(checked?QFont::Bold:QFont::Normal);
    ui->fontWeightToolbutton->setChecked(checked);
    SaveSetting::getInstance().setFontWidget(checked);
    textEdit->setFocus();
}
void chatWindow::onSendRoomFile()
{

}
void chatWindow::on_sendFileToolbutton_clicked()
{
    if(friendData==nullptr){
        onSendRoomFile();
        return;
    }
    QStringList paths = QFileDialog::getOpenFileNames(this,
                                                      tr("发送文件"));
    if (paths.isEmpty())
        return;

    for (QString path : paths)
    {
        QFile file(path);
        if (!file.exists() || !file.open(QIODevice::ReadOnly)){
            QMessageBox::warning(this,
                                 tr("Unable to open"),
                                 tr("qTox wasn't able to open %1").arg(QFileInfo(path).fileName()));
            continue;
        }
        if (file.isSequential()){
            QMessageBox::critical(this,
                                  tr("Bad idea"),
                                  tr("You're trying to send a special (sequential) file, that's not going to work!"));
            file.close();
            continue;
        }
        Core::getInstance()->sendFile(path, friendData->Id, false);
        //emit sengFile(path, friendData->Id, false);  //COre 发送文件
        file.close();
    }
}


void chatWindow::on_sendFaceToolbutton_clicked(bool f)
{
    qDebug()<<"sssssssssssssssssssssssssssssssssssssssssss";
    QToolButton *button=qobject_cast<QToolButton *>(sender());
    QPoint point=ui->frame->mapToGlobal(button->pos());

    static bool showclose=true;

    if(showclose)
    {

        faceWidget->move(point.x()-faceWidget->width()/2, point.y()-faceWidget->height()-10);
        faceWidget->show();
        faceWidget->close();
        showclose=false;
    }

    if(!faceWidget->isVisible())
    {
        faceWidget->exec(QPoint(point.x()-faceWidget->width()/2, point.y()-faceWidget->height()-10));
    }
}

void chatWindow::showEvent(QShowEvent *ev)
{
    textEdit->setFocus();
    QWidget::showEvent(ev);
}
bool chatWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj==textEdit&&ev->type()==QEvent::FocusIn)
    {
        ui->frame->setStyleSheet(" QFrame#frame{border: 1px  solid blue;}");
    }
    if(obj==textEdit&&ev->type()==QEvent::FocusOut)
    {
        ui->frame->setStyleSheet(" QFrame#frame{border:solid}");
    }
    return QWidget::eventFilter(obj, ev);
}
void chatWindow::showROOMhistoryMessage()
{

}
void chatWindow::showHistoryMessage()
{
    if(friendData==nullptr)
    {
        showROOMhistoryMessage();
        return;
    }
    qDebug()<<"showhistorymessage";
    QList<historyMessage> history=Core::getInstance()->readMessageFromDb(historyDataTime, friendData->Id);
    chatwindow->removeFirst();
    text=new Text("查看历史消息", Style::getFont(Style::Big), true, "查看历史消息",  Qt::blue);

    connect(text, SIGNAL(showHistoryMessage()), this, SLOT(showHistoryMessage()));                //显示历史消息
    for(int i=history.size()-1; i>=0; i--)
    {
        historyMessage hist=history.at(i);
        //qDebug()<<hist.datetime.toString()<<"type"<<hist.messagetype;
        if(hist.messagetype==chatWindow::_Text)
        {
            addTextMessageInTop(hist.senderName, hist.message, hist.message, hist.datetime);
            if(historyDataTime>hist.datetime)
            {
                historyDataTime=hist.datetime;
            }

        }
        if(hist.messagetype==chatWindow::TransferFile)
        {
            addFileTransferMessageInTop(hist.senderName, hist.message, false, nullptr);
        }

    }
    chatwindow->insertChatlineOnTop(ChatMessage::createHistoryMessage(text));
}
void chatWindow::cursorPositionChanged()   //保持字体的设置
{
    QTextCharFormat charFormat;
    charFormat.setFont(QFont(SaveSetting::getInstance().getFontFamily(), SaveSetting::getInstance().getFontSize(),
                             SaveSetting::getInstance().getFontWidget()?QFont::Bold:QFont::Normal, SaveSetting::getInstance().getFontItalic()));
    charFormat.setFontUnderline(SaveSetting::getInstance().getFontUnderline());
    textEdit->setCurrentCharFormat(charFormat);
}

void chatWindow::reciveFacePath(QString facePath)
{
    //textEdit->append(QString("<img width=16 height=16 src=%1>").arg(facePath));
    QTextImageFormat format;
    format.setName(facePath);
    textEdit->textCursor().insertImage(format);

}

void chatWindow::sipStatusChanged(QString status)
{
    //ui->labelCurrentStatus->setText(status);
}
void chatWindow::sendButton()
{
    onsendMessage();
}
void chatWindow::closeButton()
{
    FriendList::friendlist->show_window(QStringLiteral("mainMenuWidget"));
}
void chatWindow::updateTime()
{
    long totalTime=0, currentTime=0;
    if(Core::getInstance()->sipPhone!=nullptr)
    {
        Core::getInstance()->sipPhone->getDuration(totalTime, currentTime);
    }
    if(totalTime>15&&currentTime<0)
    {
        on_toolButton_4_clicked();               //超时挂断电话
    }
    if(currentTime>0)
    {
        ui->toolButtonSip->setToolTip(tr("通话中"));
        ui->labelCallTime->setText(QString::number(currentTime));
    }
    else
    {
        ui->toolButtonSip->setToolTip(tr("拨号中"));
        ui->labelCallTime->setText(QString::number(totalTime));
    }
}
void chatWindow::onReciveRoomMessage(const QXmppMessage &message)
{
    QXmppMucRoom *room=qobject_cast<QXmppMucRoom *>(sender());
    QString Id;
    if(room)
    {
        QRegExp regexp("(.*)(/)");
        int pos=regexp.indexIn(room->participantFullJid((message.from())));
        if(pos>-1)
        {
            Id=regexp.cap(1);
        }
        else
        {
            Id=room->participantFullJid(message.from());
        }
        if(Id==FriendList::friendlist->myData->Id)
        {
            return;
        }
    }
    else
    {
        Id=message.from();
    }
    addTextMessage(Id, message.body(), message.body(), QDateTime::currentDateTime());
}
void chatWindow::on_toolButtonFont_clicked(bool checked)
{
    if(checked==true)
    {
        ui->horizontalWidget->show();
    }
    else
    {
        ui->horizontalWidget->hide();
    }
}

void chatWindow::on_toolButtonSip_clicked()
{
    on_toolButton_4_clicked();
}


void chatWindow::on_toolButtonCut_clicked()
{
    ScreenShotPart *screen=new ScreenShotPart(this);
    screen->show();
}

