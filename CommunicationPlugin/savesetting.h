#ifndef SAVESETTING_H
#define SAVESETTING_H

#include <QObject>
#include <QSettings>
#include <QThread>
#include <QMutex>
#include <QHash>
#include <QObject>
#include <QPixmap>
#include <QMutex>
#include <QDate>
#include <QNetworkProxy>

class SaveSetting : public QObject
{
    Q_OBJECT
public:
    explicit SaveSetting(QObject *parent = 0);
    static SaveSetting& getInstance();
    ~SaveSetting();

    struct Login {
        QString userName;
        QString password;
    };
    QList<Login> loginList;                  //登陆用户列表

    /***********************设置*************************/
    //windews窗体外观大小
    QByteArray getWindowGeometry() const;
    void setWindowGeometry(const QByteArray &value);

    //windews窗体工具栏状态
    QByteArray getWindowState() const;
    void setWindowState(const QByteArray &value);

    //好友列表的宽度
    int getFriendWindowWidth();
    void setFriendWindowWinth(int width);
    //是否保存聊天记录
    bool getChatLog();
    void setChatLog(bool chatlog);

    //设置翻译
    QString getTranslation() const;
    void setTranslation(QString newValue);

     //开机自动启动
    bool getAutorun() const;
    void setAutorun(bool newValue);

    //启动自动检测更新
    bool getCheckUpdates() const;
    void setCheckUpdates(bool newValue);

    //显示图标在系统托盘
    bool getShowSystemTray() const;
    void setShowSystemTray(const bool& newValue);
    //在系统盘启动
    bool getAutostartInTray() const;
    void setAutostartInTray(bool newValue);
    //关闭到系统盘
    bool getCloseToTray() const;
    void setCloseToTray(bool newValue);
    //最小到系统盘
    bool getMinimizeToTray() const;
    void setMinimizeToTray(bool newValue);
    //多久时间后自动离开
    int getAutoAwayTime() const;
    void setAutoAwayTime(int newValue);
    //全局自动接收目录
    QString getGlobalAutoAcceptDir() const;
    void setGlobalAutoAcceptDir(const QString& dir);
    //个人自动接收目录
    QString getAutoAcceptDir(const QString& id) const;
    void setAutoAcceptDir(const QString& id, const QString& dir);
    //开启自动保存
    void setAutoSaveEnabled(bool newValue);
    bool getAutoSaveEnabled() const;
    //消息通知声音
    bool getNotifySound() const;
    void setNotifySound(bool newValue);
    //忙声音
    bool getBusySound() const;
    void setBusySound(bool newValue);
    //有消息自动显示窗口
    bool getShowWindow() const;
    void setShowWindow(bool newValue);
    //设为活动窗口
    bool getShowInFront() const;
    void setShowInFront(bool newValue);


    //状太改变通知
    bool getStatusChangeNotificationEnabled() const;
    void setStatusChangeNotificationEnabled(bool newValue);
    //群组消息提醒
    bool getGroupAlwaysNotify() const;
    void setGroupAlwaysNotify(bool newValue);
    //多窗口模式
    bool getSeparateWindow() const;
    void setSeparateWindow(bool value);
    //使用表情
    bool getUseEmoticons() const;
    void setUseEmoticons(bool newValue);
    //主题风格
    QString getStyle() const;
    void setStyle(const QString& newValue);
    //主题颜色
    int getThemeColor() const;
    void setThemeColor(const int& value);
    //时间格式
    const QString& getTimestampFormat() const;
    void setTimestampFormat(const QString& format);
    //日期格式
    const QString& getDateFormat() const;
    void setDateFormat(const QString& format);
    //xmpp服务器ip
    const QString& getXmppServerIp() const;
    void setXmppServerIp(const QString& xmppIp);


    //自动登陆
    bool getEnableLogging() const;
    void setEnableLogging(bool newValue);
    //保存登陆密码
    bool getPassword();
    void setPassword(bool password);

    //获取表情颜色和图标的大小
    int getEmojiFontPointSize() const;
    void setEmojiFontPointSize(int value);

    //获取表情包
    QString getSmileyPack() const;
    void setSmileyPack(const QString &value);
    //聊天字体和大小
    QString getFontFamily(); //类型
    void setFontFamily(QString fontFamily);
    int getFontSize();     //大小
    void setFontSize(int fontSize);
    bool getFontWidget();     //加粗
    void setFontWidget(bool fontWidget);
    bool getFontItalic();         //斜体
    void setFontItalic(bool fontItalic);
    bool getFontUnderline();   //下划线
    void setFontUnderline(bool fontUnderline);

    //临时图片个数
    int getTempPictureNum();
    void setTempPictureNum(int num);

    //sip
    QString getSipServer();
    void setSipServer(QString sipserver);
    QString getSipAgency();
    void setSipAgency(QString sipagency);
    QString getSipHostName();
    void setSipHostName(QString siphostname);
    QString getSipUserName();
    void setSipUserName(QString sipusername);
    QString getSipLoginName();
    void setSipLoginName(QString siploginname);
    QString getSipPassword();
    void setSipPassword(QString sippassword);

    bool getSipEncryption();
    void setSipEncrypTion(bool  sipencryption);

    //voice
    QString getOutDevName();
    void setOutDevName(QString outdevname);
    QString getInDevName();
    void setInDevName(QString indevname);
    int getOutDevVoice();
    void setOutDevVoice(int outdevvoice);
    int getInDevVoice();
    void setInDevVoice(int indevvoice);



    QString getSettingsDirPath(); ///< The returned path ends with a directory separator
    QString getAppDataDirPath(); ///< The returned path ends with a directory separator
    QString getAppCacheDirPath(); ///< The returned path ends with a directory separator

    void createSettingsDir(); ///< Creates a path to the settings dir, if it doesn't already exist
    void createPersonal(QString basename); ///< Write a default personal .ini settings file for a profile


    void loadPersonal(QString profileName);               //加载个人配置文件
    void loadGlobal();                //加载全局配置文件

public slots:
    void saveGlobal(); ///< Asynchronous
    void savePersonal(QString profileName); //异步的保存当前配置
    void sync(); ///< Waits for all asynchronous operations to complete

private:

    static QMutex bigLock;             //互斥锁
    static SaveSetting* savesetting;         //指向自己的实例
    static const QString globalSettingsFile;         //全局配置文件
    static QThread* settingsThread;                  //执行线程

    //聊天字体和大小
    QString fontFamily;        //字体
    int fontSize;              //大小
    bool fontWidget;           //加粗
    bool fontItalic;           //斜体
    bool fontUnderline;        //下划线
    bool chatLog;              //聊天记录
    bool loaded;                 //是否被加载过
    bool autoLogin;              //自动登陆
    bool separateWindow;
    bool savePassWord;           //登陆密码
    int tempPicture;             //临时文件个数

    QString translation;         //翻译

    bool useEmoticons;           //是否使用表情包
    bool checkUpdates;
    bool showWindow;
    bool showInFront;
    bool notifySound;
    bool busySound;
    bool groupAlwaysNotify;

    bool enableLogging;

    int autoAwayTime;
    bool autoSaveEnabled;
    QString globalAutoAcceptDir;
    QString autoAcceptDir;
    QString xmppServerIp;          //xmpp服务器Ip


    // GUI
    QString smileyPack;   //表情包
    int emojiFontPointSize;
    bool minimizeOnClose;
    QByteArray windowGeometry;               //windows的位置大小
    QByteArray windowState;                  //windows状态
    QByteArray splitterState;                //分离状态
    QByteArray dialogGeometry;               //分离位置大小
    QByteArray dialogSplitterState;
    QByteArray dialogSettingsGeometry;
    QString style;                          //应用风格
    int friendListWidth;                    //好友列表宽度
    bool showSystemTray;                    //最小化到系统托盘



    // ChatView

    QString timestampFormat;
    QString dateFormat;
    bool statusChangeNotificationEnabled;     //好友状态改变通知

    // Privacy
    bool typingNotification;

    // Audio
    QString inDev;
    QString outDev;
    qreal audioInGainDecibel;
    int outVolume;

    // Video
    QString outDevName;
    QString inDevName;
    int outDevVoice;
    int inDevVoice;

    QSize camVideoRes;
    unsigned short camVideoFPS;

    int themeColor;                 //主题颜色

    //Sip
    QString sipServer;            //sip服务器
    QString sipAgency;            //sip代理
    QString sipHostName;           //主机名
    QString sipLoginName;         //登陆名
    QString sipUserName;          //用户名
    QString sipPassword;           //密码
    bool sipEncryption;           //sip加密


};

#endif // SAVESETTING_H
