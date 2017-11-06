#ifndef EMAILLOCALFILE_H
#define EMAILLOCALFILE_H

#include <QCoreApplication>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QTextStream>

#include "email_global.h"

class EMAILSHARED_EXPORT EmailLocalFile
{
public:
    EmailLocalFile();
    ~EmailLocalFile();
    QFile file;
    //收件箱添加读取
    bool addRecvEmail(QString content);
    QString readRecvEmail(QString name);
    //回收站添加读取
    bool addDeleEmail(QString name);
    QString readDeleEmail(QString name);
    //发件箱添加读取
    bool addSendEmail(QString content);
    QString readSendEmail(QString name);
    //草稿箱添加读取
    bool addScribbleEmail(QString content);
    QString readScribbleEmail(QString name);
    //配置文件添加读取
    bool checkConfigFile(int index);
    void addConfigData(int index);

    //收件箱配置文件操作
    void addmailConfig(QString name, QString flag);
    void delmailConfig(QString name, QString flag);
    QStringList readmailConfig(QString flag);
    //发件箱配置文件操作
//    void addSendConfig(QString name);
//    void addSendConfig(QString name);
//    QStringList sendRecvConfig();
    //获取邮件message-id
    QString getmailid(QString str);

    int getemailnum(QString flag);

    int getmailval(QString flag,QString key);
    void setmailsign(QString flag,QString key,int val);

    void setuserpath(QString user);
    QString getuserpath();

    //永久删除操作
    bool permanentDele(QString name, QString box);
private:
    QString username;
};

#endif // EMAILLOCALFILE_H
