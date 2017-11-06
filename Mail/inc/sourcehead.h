#ifndef SOURCEHEAD_H
#define SOURCEHEAD_H
#include <QString>
#include "SmtpMime"

class EMAILSHARED_EXPORT SourceHead
{
public:
    typedef struct head
    {
        QString Return_Path;
        QString Received;
        QString Data;
        QString From;
        QString To;
        QString Cc;
        QString Bcc;
        QString Subject;
        QString References;
        int X_Priority;
        QString X_Has_Attach;
        QString X_Mailer;
        QString Mime_Version;
        QString Message_ID;
        QString Content_Type;
        QString boundary;
    }_head;

    SourceHead(const QString & result = "");
    ~SourceHead();

    void setboundary(QString bound);
    QString getboundary();

    void setcontenttype(QString type);
    QString getcontenttype();

    void setfrom(QString fm);
    QString getfrom();

    void setto(QString t);
    QString getto();

    void setcc(QString c);
    QString gettcc();

    void setbcc(QString b);
    QString gettbcc();

    void setsubject(QString sub);
    QString getsubject();

    void setmessageid(QString id);
    QString getmessageid();

    void setreferences(QString ref);
    QString getreferences();

    void setcharset(QString ch);
    QString getcharset();

    void setrefcode(QString code);
    QString getrefcode();

private:
    QString boundary;
    QString contenttype;
    QString from;
    QString to;
    QString cc;
    QString bcc;
    QString subject;
    QString messageid;
    QString references;

    QString ch;
    QString co;
};

#endif // SOURCEHEAD_H
