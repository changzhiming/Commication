#ifndef EMAILMANAGER_H
#define EMAILMANAGER_H

#include <QObject>
#include <QFile>
#include <QListWidget>
#include <QTextDocument>
#include <QByteArray>
#include "smtpsender.h"
#include "pop3achieve.h"
#include "emaillocalfile.h"

class EMAILSHARED_EXPORT EmailManager
{
public:

    typedef struct top
    {
        QString from;
        QString to;
        QString cc;
        QString date;
        QString subject;
    }_top;

    typedef struct send
    {
        QString sender;
        QString subject;
        QString data;
    }_send;

    typedef struct num
    {
        int recvnum;
        int scrinum;
        int delenum;
    }_num;

    EmailManager();
    ~EmailManager();

    EmailLocalFile *lf;

    QFile file;
    bool SendEmail(QString hostip,QString sender, QString password, QString recipient, QString ccrecipient, QString subject,
                   QTextDocument * content, QListWidget *attemptment, bool ssl);//发送邮件操作
    _send finishEmail(int i);   //获取发件箱邮件头信息

    void AcceptEmail(QString hostip,QString user, QString pass, bool ssl);//收取全部邮件
    EmailManager::_top EmailBox(QString name); //获取邮件头，收件箱邮件
    QString EmailContent(QString name); //  获取邮件内容，收件箱邮件
    QList<QString> EmailAttempt(QString name, QString flag); //获取邮件附件


    void WriteToScribble(QString sender, QString recipient, QString subject, QTextDocument * content, QListWidget *attemptment);//草稿箱邮件添加
    EmailManager::_top ScribbleEmail(QString name);//获取草稿箱邮件头
    QString EmailContent_s(QString name);//获取草稿箱邮件内容

    void DeleteEmail(QString name); //添加删除邮件
    EmailManager::_top getDeleteEmail(QString name);//获取删除邮件头信息
    QString EmailContent_d(QString name);//回收站邮件内容

    bool PermanentDelete(QString name, QString box);

    void RsetDelete();
    void TransmitEmail();

    QStringList ReadConfigFile(QString boxname);

    _num updatestat();

    int ReadEmailSign(QString flag,QString key);
    void SetEmailSign(QString flag,QString key,int val);

    void setanalysistexttype(QString type);
    QString getanalysistexttype();

    void setusername(QString user);

private:
    QString type;
    QString host;
    QString user;
};

#endif // EMAILMANAGER_H
