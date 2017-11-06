#include "savesetting.h"
#include "platform/autorun.h"
#include "friendlist.h"
#include "listFriend/my_listmodel.h"
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QCryptographicHash>
#include <QtCore/qshareddata.h>
#include <QFont>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QDebug>
#include <QList>
#include <QStyleFactory>
#include <QCryptographicHash>
#include <QMutexLocker>
#include <QThread>
#include <QBuffer>
#include <QImageReader>


#include <QStandardPaths>

const QString SaveSetting::globalSettingsFile = "qtox.ini";//全局配置路径
SaveSetting* SaveSetting::savesetting{nullptr};                //指向自己的实例
QMutex SaveSetting::bigLock{QMutex::Recursive};            //互斥锁
QThread* SaveSetting::settingsThread{nullptr};             //执行线程

SaveSetting::SaveSetting(QObject *parent) : QObject(parent)
{
    settingsThread = new QThread(this);
    settingsThread->setObjectName(tr("savesetting thread"));
    settingsThread->start(QThread::LowPriority);
    moveToThread(settingsThread);
    savesetting=this;
}

SaveSetting::~SaveSetting()
{
    sync();
    settingsThread->exit(0);
    settingsThread->wait();
}

SaveSetting& SaveSetting::getInstance()
{

    if(savesetting==nullptr)
    {
        savesetting=new SaveSetting();
    }
    return *savesetting;
}

void SaveSetting::sync()
{

    if (QThread::currentThread() != settingsThread){
        QMetaObject::invokeMethod(&getInstance(), "sync", Qt::BlockingQueuedConnection);
        return;
    }
    QMutexLocker locker{&bigLock};
    qApp->processEvents();
}
QByteArray SaveSetting::getWindowGeometry() const
{
    QMutexLocker locker{&bigLock};
    return windowGeometry;
}

void SaveSetting::setWindowGeometry(const QByteArray &value)
{
    QMutexLocker locker{&bigLock};
    windowGeometry = value;
}

QByteArray SaveSetting::getWindowState() const
{
    QMutexLocker locker{&bigLock};
    return windowState;
}

void SaveSetting::setWindowState(const QByteArray &value)
{
    QMutexLocker locker{&bigLock};
    windowState = value;
}
int SaveSetting::getFriendWindowWidth()
{
    QMutexLocker locker{&bigLock};
    return friendListWidth;
}

void SaveSetting::setFriendWindowWinth(int width)
{
    QMutexLocker locker{&bigLock};
    friendListWidth=width;
}

bool SaveSetting::getCheckUpdates() const
{
    QMutexLocker locker{&bigLock};
    return checkUpdates;
}
void SaveSetting::setCheckUpdates(bool newValue)
{
    QMutexLocker locker{&bigLock};
    checkUpdates = newValue;
}
bool SaveSetting::getAutorun() const
{
    QMutexLocker locker{&bigLock};
    return Platform::getAutorun();
}

void SaveSetting::setAutorun(bool newValue)
{
    QMutexLocker locker{&bigLock};
    Platform::setAutorun(newValue);
}



QString SaveSetting::getTranslation() const
{
    QMutexLocker locker{&bigLock};
    return translation;
}

void SaveSetting::setTranslation(QString newValue)
{
    QMutexLocker locker{&bigLock};
    translation = newValue;
}


QString SaveSetting::getStyle() const
{
    QMutexLocker locker{&bigLock};
    return style;
}

void SaveSetting::setStyle(const QString& newStyle)
{
    QMutexLocker locker{&bigLock};
    style = newStyle;
}

bool SaveSetting::getShowSystemTray() const
{
    QMutexLocker locker{&bigLock};
    return showSystemTray;
}

void SaveSetting::setShowSystemTray(const bool& newValue)
{
    QMutexLocker locker{&bigLock};
    showSystemTray = newValue;
}


bool SaveSetting::getStatusChangeNotificationEnabled() const
{
    QMutexLocker locker{&bigLock};
    return statusChangeNotificationEnabled;
}

void SaveSetting::setStatusChangeNotificationEnabled(bool newValue)
{
    QMutexLocker locker{&bigLock};
    statusChangeNotificationEnabled = newValue;
}

bool SaveSetting::getShowInFront() const
{
    QMutexLocker locker{&bigLock};
    return showInFront;
}

void SaveSetting::setShowInFront(bool newValue)
{
    QMutexLocker locker{&bigLock};
    showInFront = newValue;
}

bool SaveSetting::getNotifySound() const
{
    QMutexLocker locker{&bigLock};
    return notifySound;
}

void SaveSetting::setNotifySound(bool newValue)
{
    QMutexLocker locker{&bigLock};
    notifySound = newValue;
}

bool SaveSetting::getBusySound() const
{
    QMutexLocker locker{&bigLock};
    return busySound;
}

void SaveSetting::setBusySound(bool newValue)
{
    QMutexLocker locker{&bigLock};
    busySound = newValue;
}

bool SaveSetting::getGroupAlwaysNotify() const
{
    QMutexLocker locker{&bigLock};
    return groupAlwaysNotify;
}

void SaveSetting::setGroupAlwaysNotify(bool newValue)
{
    QMutexLocker locker{&bigLock};
    groupAlwaysNotify = newValue;
}
int SaveSetting::getAutoAwayTime() const
{
    QMutexLocker locker{&bigLock};
    return autoAwayTime;
}

void SaveSetting::setAutoAwayTime(int newValue)
{
    QMutexLocker locker{&bigLock};
    if (newValue < 0)
        newValue = 10;

    autoAwayTime = newValue;
}

void SaveSetting::setAutoSaveEnabled(bool newValue)
{
    QMutexLocker locker{&bigLock};
    autoSaveEnabled = newValue;
}

bool SaveSetting::getAutoSaveEnabled() const
{
    QMutexLocker locker{&bigLock};
    return autoSaveEnabled;
}


bool SaveSetting::getSeparateWindow() const
{
    QMutexLocker locker{&bigLock};
    return separateWindow;
}

void SaveSetting::setSeparateWindow(bool value)
{
    QMutexLocker locker{&bigLock};
    separateWindow = value;
}
void SaveSetting::setUseEmoticons(bool newValue)
{
    QMutexLocker locker{&bigLock};
    useEmoticons = newValue;
}

bool SaveSetting::getUseEmoticons() const
{
    QMutexLocker locker{&bigLock};
    return useEmoticons;
}
int SaveSetting::getThemeColor() const
{
    QMutexLocker locker{&bigLock};
    return themeColor;
}

void SaveSetting::setThemeColor(const int &value)
{
    QMutexLocker locker{&bigLock};
    themeColor = value;
}
const QString& SaveSetting::getTimestampFormat() const
{
    QMutexLocker locker{&bigLock};
    return timestampFormat;
}

void SaveSetting::setTimestampFormat(const QString &format)
{
    QMutexLocker locker{&bigLock};
    timestampFormat = format;
}

const QString& SaveSetting::getDateFormat() const
{
    QMutexLocker locker{&bigLock};
    return dateFormat;
}

void SaveSetting::setDateFormat(const QString &format)
{
    QMutexLocker locker{&bigLock};
    dateFormat = format;
}

const QString& SaveSetting::getXmppServerIp() const
{
    QMutexLocker locker{&bigLock};
    return xmppServerIp;
}

void SaveSetting::setXmppServerIp(const QString &xmppIp)
{
    QMutexLocker locker{&bigLock};
    xmppServerIp = xmppIp;
}

void SaveSetting::saveGlobal()
{
    if (QThread::currentThread() != settingsThread)
        return (void) QMetaObject::invokeMethod(&getInstance(), "saveGlobal");

    QMutexLocker locker{&bigLock};
    QString path = getSettingsDirPath() + globalSettingsFile;
    qDebug() << "Saving global SaveSetting at " + path;

    QSettings s(path, QSettings::IniFormat);
    s.setIniCodec("UTF-8");  //设置编码
    s.clear();               //清理
    s.beginGroup("Login");
        s.setValue("autoLogin", autoLogin);
    s.endGroup();

    s.beginGroup("TempPicture");
        s.setValue("tempPicture", tempPicture);
    s.endGroup();
    
    s.beginGroup("userPassWord");
        s.beginWriteArray("loginList");
        for(int i=0; i<loginList.size(); ++i)
        {
            s.setArrayIndex(i);
            s.setValue("userName", loginList.at(i).userName);
            s.setValue("password", loginList.at(i).password);
        }
        s.endArray();
        s.setValue("xmppServerIp", xmppServerIp);
    s.endGroup();
    
    s.beginGroup("General");
        s.setValue("translation",translation);
        s.setValue("showSystemTray", showSystemTray);
        s.setValue("autoAwayTime", autoAwayTime);
        s.setValue("checkUpdates", checkUpdates);
        s.setValue("showWindow", showWindow);
        s.setValue("showInFront", showInFront);
        s.setValue("notifySound", notifySound);
        s.setValue("busySound", busySound);
        s.setValue("groupAlwaysNotify", groupAlwaysNotify);
        s.setValue("separateWindow", separateWindow);
        s.setValue("autoSaveEnabled", autoSaveEnabled);
        s.setValue("globalAutoAcceptDir", globalAutoAcceptDir);
        s.setValue("chatLog", chatLog);
        s.setValue("savePassord", savePassWord);
    s.endGroup();

    s.beginGroup("GUI");
        s.setValue("smileyPack", smileyPack);
        s.setValue("emojiFontPointSize", emojiFontPointSize);
        s.setValue("timestampFormat", timestampFormat);
        s.setValue("dateFormat", dateFormat);
        s.setValue("useEmoticons", useEmoticons);
        s.setValue("themeColor", themeColor);
        s.setValue("style", style);
        s.setValue("statusChangeNotificationEnabled", statusChangeNotificationEnabled);
    s.endGroup();

    s.beginGroup("State");
        s.setValue("windowGeometry", windowGeometry);
        s.setValue("friendListWidth", friendListWidth);
        s.setValue("splitterState", splitterState);
        s.setValue("dialogGeometry", dialogGeometry);
        s.setValue("dialogSplitterState", dialogSplitterState);
        s.setValue("dialogSaveSettingGeometry", dialogSettingsGeometry);
    s.endGroup();

    s.beginGroup("Andio");
        s.setValue("outDevName", outDevName);
        s.setValue("inDevName", inDevName);
        s.setValue("outDevVoice",outDevVoice);
        s.setValue("inDevVoice", inDevVoice);
    s.endGroup();

    s.beginGroup("Video");
        s.setValue("camVideoRes",camVideoRes);
        s.setValue("camVideoFPS",camVideoFPS);
    s.endGroup();

    s.beginGroup("Font");        //保存字体格式和大小
        s.setValue("fontFamily", fontFamily);
        s.setValue("fontSize", fontSize);
        s.setValue("fontWidget", fontWidget);
        s.setValue("fontItalic", fontItalic);
        s.setValue("fontUnderline", fontUnderline);
    s.endGroup();


    s.beginGroup("SIP");

        s.setValue("sipServer", sipServer);
        s.setValue("sipUserName", sipUserName);
        s.setValue("sipAgency",sipAgency);
        s.setValue("sipHostName", sipHostName);
        s.setValue("sipLoginName", sipLoginName);
        s.setValue("sipPassword", sipPassword);
        s.setValue("sipEncryption", sipEncryption);
    s.endGroup();


}
void SaveSetting::savePersonal(QString profileName)
{
    /*if (QThread::currentThread() != settingsThread)
        return (void) QMetaObject::invokeMethod(&getInstance(), "savePersonal",
                                                Q_ARG(QString, profileName));*/
    QMutexLocker locker{&bigLock};

    QString path = getSettingsDirPath() + profileName + ".ini";

    qDebug() << "Saving personal settings at " << path;

    QSettings ps(path, QSettings::IniFormat);

    ps.setIniCodec("UTF-8");  //设置编码
    ps.clear();               //清理
    ps.beginGroup("Friends");
        ps.beginWriteArray("Friend", FriendList::friendlist->friendList.size());     //好友 列表大小
        int index = 0;
        for (auto& frnd : FriendList::friendlist->friendList.values())
        {
            ps.setArrayIndex(index);
            ps.setValue("iconPath", frnd->iconPath);
            ps.setValue("Name", frnd->Name);
            ps.setValue("Id", frnd->Id);
            ps.setValue("Group", frnd->Group);
            ps.setValue("status", frnd->status);
            //ps.setValue("Vcard", frnd->vcard)

            ps.beginGroup("Vcard");
            XmppClient::FriendVcard vcard=frnd->vcard;
            {
                ps.setValue("username", vcard.username);
                ps.setValue("nickname", vcard.nickname);
                ps.setValue("email", vcard.email);
                ps.setValue("VoIP", vcard.VoIP);
                ps.setValue("title", vcard.title);
                ps.setValue("unit", vcard.unit);
                ps.setValue("phone", vcard.phone);
                ps.setValue("mobile", vcard.mobile);
                ps.setValue("fax", vcard.fax);
                ps.setValue("photo", vcard.photo);
                ps.setValue("photoType", vcard.photoType);
            }
            ps.endGroup();
            ++index;
        }
        ps.endArray();

        ps.beginWriteArray("CurrentContacts", FriendList::friendlist->currentContactList.size());     //the current contacts list;
        index = 0;
        for (auto& Id : FriendList::friendlist->currentContactList)
        {
            ps.setArrayIndex(index);
            ps.setValue("Id", Id);
            ++index;
        }
        ps.endArray();
    ps.endGroup();


}
void SaveSetting::loadGlobal()
{
    QMutexLocker locker{&bigLock};

    QDir dir(getSettingsDirPath());
    QString filePath = dir.filePath(globalSettingsFile);

    // If no settings file exist -- use the default one
    if (!QFile(filePath).exists())
    {
        qDebug() << "No settings file found, using defaults";
        filePath = ":/conf/" + globalSettingsFile;  //默认设置
    }

    qDebug() << "Loading settings from " + filePath;

    QSettings s(filePath, QSettings::IniFormat);
    s.setIniCodec("UTF-8");

    s.beginGroup("Login");
        autoLogin = s.value("autoLogin", false).toBool();
    s.endGroup();

    s.beginGroup("TempPicture");
        tempPicture=s.value("tempPicture", 0).toInt();
    s.endGroup();

    s.beginGroup("userPassWord");
        int size=s.beginReadArray("loginList");
        for(int i=0; i<size; ++i)
        {
            s.setArrayIndex(i);
            Login login;
            login.userName=s.value("userName", QString()).toString();
            login.password=s.value("password", QString()).toString();
            loginList.append(login);
        }
        s.endArray();
        xmppServerIp=s.value("xmppServerIp").toString();
    s.endGroup();

    s.beginGroup("General");

        translation = s.value("translation", "en").toString();
        showSystemTray = s.value("showSystemTray", true).toBool();
        autoAwayTime = s.value("autoAwayTime", 10).toInt();
        checkUpdates = s.value("checkUpdates", true).toBool();
        showWindow = s.value("showWindow", true).toBool();
        showInFront = s.value("showInFront", false).toBool();
        notifySound = s.value("notifySound", true).toBool();
        busySound = s.value("busySound", false).toBool();
        groupAlwaysNotify = s.value("groupAlwaysNotify", false).toBool();

        autoSaveEnabled = s.value("autoSaveEnabled", false).toBool();
        globalAutoAcceptDir = s.value("globalAutoAcceptDir",
                                      QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory)
                                      ).toString();
        separateWindow = s.value("separateWindow", false).toBool();
        chatLog=s.value("chatLog", true).toBool();
        savePassWord=s.value("savePassord", false).toBool();
    s.endGroup();


    s.beginGroup("GUI");
        const QString DEFAULT_SMILEYS = ":/smileys/emojione/emoticons.xml";
        smileyPack = s.value("smileyPack", DEFAULT_SMILEYS).toString();
        //if (!SmileyPack::isValid(smileyPack))
       // {
        //    smileyPack = DEFAULT_SMILEYS;
       // }
        emojiFontPointSize = s.value("emojiFontPointSize", 16).toInt();

        timestampFormat = s.value("timestampFormat", "hh:mm:ss").toString();
        dateFormat = s.value("dateFormat", "dddd, MMMM d, yyyy").toString();
        useEmoticons = s.value("useEmoticons", true).toBool();
        statusChangeNotificationEnabled = s.value("statusChangeNotificationEnabled", false).toBool();
        themeColor = s.value("themeColor", 0).toInt();
        style = s.value("style", "").toString();
        if (style == "") // Default to Fusion if available, otherwise no style
        {
            if (QStyleFactory::keys().contains("Fusion"))
                style = "Fusion";
            else
                style = "None";
        }
    s.endGroup();

    s.beginGroup("State");
        windowGeometry = s.value("windowGeometry", QByteArray()).toByteArray();
        friendListWidth = s.value("friendListWidth", 0).toInt();
        splitterState = s.value("splitterState", QByteArray()).toByteArray();
        dialogGeometry = s.value("dialogGeometry", QByteArray()).toByteArray();
        dialogSplitterState = s.value("dialogSplitterState", QByteArray()).toByteArray();
        dialogSettingsGeometry = s.value("dialogSettingsGeometry", QByteArray()).toByteArray();
    s.endGroup();

    s.beginGroup("Andio");
        outDevName= s.value("outDevName", QString()).toString();
        inDevName= s.value("inDevName", QString()).toString();
        outDevVoice=s.value("outDevVoice").toInt();
        inDevVoice= s.value("inDevVoice").toInt();
    s.endGroup();

    s.beginGroup("Video");
        camVideoRes = s.value("camVideoRes",QSize()).toSize();
        camVideoFPS = s.value("camVideoFPS", 0).toUInt();
    s.endGroup();

    s.beginGroup("Font");        //保存字体格式和大小
        fontFamily = s.value("fontFamily", "宋体").toString();
        fontSize= s.value("fontSize", 9).toDouble();

        fontWidget= s.value("fontWidget", false).toBool();
        fontItalic= s.value("fontItalic", false).toBool();
        fontUnderline= s.value("fontUnderline", false).toBool();
    s.endGroup();

    s.beginGroup("SIP");
        sipServer=s.value("sipServer", QString()).toString();
        sipAgency=s.value("sipAgency",QString()).toString();
        sipHostName=s.value("sipHostName", QString()).toString();
        sipLoginName=s.value("sipLoginName", QString()).toString();
        sipPassword=s.value("sipPassword", QString()).toString();
        sipUserName=s.value("sipUserName", QString()).toString();
        sipEncryption=s.value("sipEncryption", QString()).toBool();
    s.endGroup();

}
void SaveSetting::loadPersonal(QString profileName)
{
    QMutexLocker locker{&bigLock};

    QDir dir(getSettingsDirPath());
    QString filePath = dir.filePath(globalSettingsFile);

    // load from a profile specific friend data list if possible
    QString tmp = dir.filePath(profileName + ".ini");
    if (QFile(tmp).exists()) // otherwise, filePath remains the global file
        filePath = tmp;

    qDebug()<<"Loading personal settings from"<<filePath;

    QSettings ps(filePath, QSettings::IniFormat);
    ps.setIniCodec("UTF-8");
    FriendList::friendlist->friendList.clear();

    ps.beginGroup("Friends");
        int size=ps.beginReadArray("Friend");
        FriendList::friendlist->friendList.reserve(size);
        for (int i = 0; i < size; i ++)
        {
            ps.setArrayIndex(i);
            ListItemData *data=new ListItemData();
            data->iconPath=ps.value("iconPath").toString();
            data->Name=ps.value("Name").toString();
            data->Id=ps.value("Id").toString();
            data->Group=ps.value("Group").toString();
            data->status=(XmppClient::StatusType)ps.value("status").toInt();
            ps.beginGroup("Vcard");
            XmppClient::FriendVcard vcard;
            {

                vcard.username=ps.value("username").toString();
                vcard.nickname=ps.value("nickname").toString();
                vcard.email=ps.value("email").toString();
                vcard.VoIP=ps.value("VoIP").toString();
                vcard.title=ps.value("title").toString();
                vcard.unit= ps.value("unit").toString();
                vcard.phone= ps.value("phone").toString();
                vcard.mobile=ps.value("mobile").toString();
                vcard.fax=ps.value("fax").toString();
                QByteArray m_photo = ps.value("photo").toByteArray();
                QBuffer buffer;
                buffer.setData(m_photo);
                buffer.open(QIODevice::ReadOnly);
                QImageReader imageReader(&buffer);
                QImage image = imageReader.read();

                vcard.photo=image;
                vcard.photoType=ps.value("photoType").toString();
            }
            data->vcard=vcard;
            ps.endGroup();
            FriendList::friendlist->friendList.insert(data->Id, data);
        }
        ps.endArray();

        size=ps.beginReadArray("CurrentContacts");     //the current contacts list;
        for (int i = 0; i < size; i++)
        {
            ps.setArrayIndex(i);

            FriendList::friendlist->currentContactList.push_front((ps.value("Id").toString()));
        }
        ps.endArray();
    ps.endGroup();

}

QString SaveSetting::getSettingsDirPath()
{
    QMutexLocker locker{&bigLock};

    return QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir::separator()
                           + "AppData" + QDir::separator() + "tox")+QDir::separator();

}

QString SaveSetting::getAppDataDirPath()
{
    QMutexLocker locker{&bigLock};

    return  qApp->applicationDirPath()+QDir::separator()+"file"+QDir::separator();
}

QString SaveSetting::getAppCacheDirPath()
{
    QMutexLocker locker{&bigLock};

    return QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir::separator()
                           + "AppData" + QDir::separator() + "Chang" + QDir::separator() + "tox")+QDir::separator();

}
void SaveSetting::createSettingsDir()
{
    QMutexLocker locker{&bigLock};
    QString dir = SaveSetting::getSettingsDirPath();
    QDir directory(dir);
    if (!directory.exists() && !directory.mkpath(directory.absolutePath()))
        qCritical() << "Error while creating directory " << dir;
}
QString SaveSetting::getGlobalAutoAcceptDir() const
{
    QMutexLocker locker{&bigLock};
    return globalAutoAcceptDir;
}

void SaveSetting::setGlobalAutoAcceptDir(const QString& newValue)
{
    QMutexLocker locker{&bigLock};
    globalAutoAcceptDir = newValue;
}

bool SaveSetting::getEnableLogging() const
{
    QMutexLocker locker{&bigLock};
    return enableLogging;
}

void SaveSetting::setEnableLogging(bool newValue)
{
    QMutexLocker locker{&bigLock};
    enableLogging = newValue;
}
bool SaveSetting::getPassword()
{
    QMutexLocker locker{&bigLock};
    return savePassWord;
}

void SaveSetting::setPassword(bool password)
{
    QMutexLocker locker{&bigLock};
    this->savePassWord=password;
}

int SaveSetting::getEmojiFontPointSize() const
{
    QMutexLocker locker{&bigLock};
    return emojiFontPointSize;
}

void SaveSetting::setEmojiFontPointSize(int value)
{
    QMutexLocker locker{&bigLock};
    emojiFontPointSize = value;

}
QString SaveSetting::getSmileyPack() const
{
    QMutexLocker locker{&bigLock};
    return smileyPack;
}

void SaveSetting::setSmileyPack(const QString &value)
{
    QMutexLocker locker{&bigLock};
    smileyPack = value;

}

QString SaveSetting::getFontFamily()    //字体类型
{
    QMutexLocker locker{&bigLock};
    return fontFamily;
}
void SaveSetting::setFontFamily(QString family)
{
    QMutexLocker locker{&bigLock};
    fontFamily=family;
}
int SaveSetting::getFontSize()    //大小
{
    QMutexLocker locker{&bigLock};
    return fontSize;

}

void SaveSetting::setFontSize(int fontSize)
{
    QMutexLocker locker{&bigLock};
    qDebug()<<fontSize<<"set fontsize";
    this->fontSize=fontSize;
}

bool SaveSetting::getFontWidget()     //加粗
{
    QMutexLocker locker{&bigLock};
    return fontWidget;
}

void SaveSetting::setFontWidget(bool fontWidget)
{
   QMutexLocker locker{&bigLock};
   this->fontWidget=fontWidget;
}

bool SaveSetting::getFontItalic()        //斜体
{
    QMutexLocker locker{&bigLock};
    return fontItalic;

}

void SaveSetting::setFontItalic(bool fontItalic)
{
    QMutexLocker locker{&bigLock};
    this->fontItalic=fontItalic;
}

bool SaveSetting::getFontUnderline()   //下划线
{
    QMutexLocker locker{&bigLock};
    return fontUnderline;
}

void SaveSetting::setFontUnderline(bool fontUnderline)
{
    QMutexLocker locker{&bigLock};
    this->fontUnderline=fontUnderline;
}
int SaveSetting::getTempPictureNum()
{
    return tempPicture;
}
void SaveSetting::setTempPictureNum(int num)
{
    QMutexLocker locker{&bigLock};
    tempPicture=num;
}

bool SaveSetting::getChatLog()         //聊天记录
{
    QMutexLocker locker{&bigLock};
    return chatLog;
}
void SaveSetting::setChatLog(bool chatlog)
{
    QMutexLocker locker{&bigLock};
    chatLog=chatlog;
}

//sip
QString SaveSetting::getSipServer()
{
    QMutexLocker locker{&bigLock};
    return sipServer;
}
void SaveSetting::setSipServer(QString sipserver)
{
    QMutexLocker locker{&bigLock};
    sipServer=sipserver;
}

QString SaveSetting::getSipAgency()
{
    QMutexLocker locker{&bigLock};
    return sipAgency;
}
void SaveSetting::setSipAgency(QString sipagency)
{
    QMutexLocker locker{&bigLock};
    sipAgency=sipagency;
}

QString SaveSetting::getSipHostName()
{
    QMutexLocker locker{&bigLock};
    return sipHostName;
}
void SaveSetting::setSipHostName(QString siphostname)
{
    QMutexLocker locker{&bigLock};
    sipHostName=siphostname;
}

QString SaveSetting::getSipUserName()
{
    QMutexLocker locker{&bigLock};
    return sipUserName;
}
void SaveSetting::setSipUserName(QString sipusername)
{
    QMutexLocker locker{&bigLock};
    sipUserName=sipusername;
}

QString SaveSetting::getSipLoginName()
{
    QMutexLocker locker{&bigLock};
    return sipLoginName;
}
void SaveSetting::setSipLoginName(QString siploginname)
{
    QMutexLocker locker{&bigLock};
    sipLoginName=siploginname;
}

QString SaveSetting::getSipPassword()
{
    QMutexLocker locker{&bigLock};
    return sipPassword;
}
void SaveSetting::setSipPassword(QString sippassword)
{
    QMutexLocker locker{&bigLock};
    sipPassword=sippassword;
}

bool SaveSetting::getSipEncryption()
{
    QMutexLocker locker{&bigLock};
    return sipEncryption;
}
void SaveSetting::setSipEncrypTion(bool sipencryption)
{
    QMutexLocker locker{&bigLock};
    sipEncryption=sipencryption;
}

//voice
QString SaveSetting::getOutDevName()
{
    return outDevName;
}

void SaveSetting::setOutDevName(QString outdevname)
{
    outDevName=outdevname;
}

QString SaveSetting::getInDevName()
{
    return inDevName;
}

void SaveSetting::setInDevName(QString indevname)
{
    inDevName=indevname;
}

int SaveSetting::getOutDevVoice()
{
    return outDevVoice;
}

void SaveSetting::setOutDevVoice(int outdevvoice)
{
    outDevVoice=outdevvoice;
}

int SaveSetting::getInDevVoice()
{
    return inDevVoice;
}

void SaveSetting::setInDevVoice(int indevvoice)
{
    inDevVoice=indevvoice;
}
