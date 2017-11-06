#ifndef ANALYSISDATA_H
#define ANALYSISDATA_H

#include "SmtpMime"
#include "emailmanager.h"
#include <QString>
#include <QObject>

class EMAILSHARED_EXPORT AnalysisData : public QObject
{
public:

    AnalysisData(const QString & result = "");
    ~AnalysisData();
    //各部分获取截取
    void devidestrformixed(QString result);

    void setHead(const QString &head);
    const QString& getHead() const;

    void setContent(const QString &content);
    const QString& getContent() const;

    void setAttempt(const QString &attempt);
    const QString& getAttempt() const;

    void setBoundary(const QString &boundary);
    const QString& getBoundary() const;

    void getAttemptData(QString &filepath);
    void getContentData(QString &con);

    QList<QString> getAttemptDatas();

    EmailManager::_top emailTopData(QString data);

    //获取内容编码方式
    QString getcharset();
    void setcharset(QString charset);

    void charset(int p, QString ch1, QString ch2, QString key);

    QString getencode();
    void setencode(QString encode);

    QString code(QString ct);

    void setcontenttype(QString type);
    QString getcontenttype();

    void setboundary2(QString boundary);
    QString getboundary2();

    void boundary2(QString con);

    //草稿邮件和已发邮件处理
    void localmail(QString str);
    void setlocalmailhead(EmailManager::_top t);
    EmailManager::_top localmailhead();

    QString loaclmailcontent();
    QString localmailattempt();

    //邮件主题编码解析函数
    QString subjectdecode(QString str);

    //引用邮件解析
    void setreferences(QString ref);
    QString getreferences();

    void setreferhead(QString refhead);
    QString getreferhead();

    void setrefercontent(QString content);
    QString getrefercontent();

    void referemail(QString strdata);

    void settexttype(QString b);
    QString gettexttype();


private:

    QString head;
    QString content;
    QString attemptment;
    QString boundary;
    QString boundary_c;
    QString type;
    QString texttype;

    QString encode;
    QString cset;
    EmailManager::_top tp;
    QString references;
    QString referhead;
    QString refercontent;
};

#endif // ANALYSISDATA_H
