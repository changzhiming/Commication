#ifndef chatWindow_H
#define chatWindow_H

#include <QWidget>
#include <QTimer>
#include "chatlog/chatmessage.h"
#include "xmpp/xmppclient.h"

class keyboard;
class ListItemData;
class MyTextEdit;
class QTextBrowser;
class ChatMessage;
class QDateTime;


namespace Ui {
class chat_win;
}
struct historyMessage;

class SaveChatLog;
class ChatLog;
class FaceWidget;
class chatWindow : public QWidget
{
    Q_OBJECT
public:

    enum MessageType
    {
        _Text,
        TransferFile,
        Voice,
        Video,
    };
    enum WindowType
    {
        Signal,
        Room,
    };

    explicit chatWindow(QWidget *parent, ListItemData *frienddata);
    chatWindow(QWidget *parent); //QRoom * room);

    ~chatWindow();
    void init();
    void addTextMessage(const QString &sender, const QString &rawMessage, const QString richtext, const QDateTime &date);
    void addTextMessageInTop(const QString &sender, const QString &rawMessage, const QString richtext, const QDateTime &date);
    void addFileTransferMessage(const QString &sender, QString file, bool isMe, QXmppTransferJob *job);
    void addFileTransferMessageInTop(const QString &sender, QString file, bool isMe, QXmppTransferJob *job);

    void setWindowType(chatWindow::WindowType windowype);
    ChatLog * getChatLog();

public slots:
    bool onsendMessage();  //send data to textbower
    bool onsendRoomMessage(); // send Room message
    void cursorPositionChanged();
    void sipStatusChanged(QString status);                   //sip状太改变
    void sendButton();
    void closeButton();
    void updateTime();
    void onReciveRoomMessage(const QXmppMessage &message);

    void on_toolButton_4_clicked();
    void onCallSip();

protected:
    void showEvent(QShowEvent *ev);
    bool eventFilter(QObject *obj, QEvent *ev);
    chatWindow::WindowType windowtype;

private slots:

    void on_fontComboBox_currentFontChanged(const QFont &f);  //字体类型
    void on_fontSizeComboBox_currentIndexChanged(const QString &arg1); //字体大小
    void on_fontWeightToolbutton_clicked(bool checked); //字体加粗
    void on_fontItalicToolbutton_clicked(bool checked); //字体斜体
    void on_fontUnderlineToolButton_clicked(bool checked);//字体下划线
    void on_sendFaceToolbutton_clicked(bool f);
    void on_sendFileToolbutton_clicked();                  //发送文件

    void onSendRoomFile();                       //ROOM send file
    void showHistoryMessage();
    void showROOMhistoryMessage();                      //ROOM show history message;
    void reciveFacePath(QString facePath);

    void on_toolButtonFont_clicked(bool checked);

    void on_toolButtonSip_clicked();

    void on_toolButtonCut_clicked();


signals:

    void sendMessage(FriendDataMessage message);
    void sengFile(QString path, QString usernmae, bool isPicture);
    void insertMessageToDb(struct historyMessage *message);

public:
    QString messageForm;  //消息的显示
    ListItemData *friendData=nullptr;

private:
    Ui::chat_win *ui;  //聊天界面

    SaveChatLog *chat_Log=nullptr;             //聊天记录
    MyTextEdit *textEdit=nullptr;
    ChatLog *chatwindow=nullptr;       //聊天消息显示窗体

    bool insertSenderName=true;  //是否插入发送和接收者的名字
    Text *text=nullptr;                  //查看历史消息
    QDateTime historyDataTime;         //历史记录时间
    FaceWidget *faceWidget=nullptr;
    QTimer *timer=nullptr;

};

#endif // chatWindow_H

