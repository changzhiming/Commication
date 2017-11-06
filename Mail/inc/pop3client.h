#ifndef POP3CLIENT_H
#define POP3CLIENT_H

#include "email.h"

class EMAILSHARED_EXPORT Pop3Client : public Email
{
public:

    struct MailData
    {
        unsigned int index;
        unsigned int size;
    };
    typedef struct top
    {
        QString from;
        QString to;
        QString date;
        QString subject;
    }_top;

    Pop3Client(const QString &host, int port, const QString &username, const QString &password, Email::ConnectionType ct);
    ~Pop3Client();

    bool Pop3connectToHost();

    bool login();
    bool logoff();
    bool list(std::vector<MailData> &result,unsigned int index=0);
    bool uidl(std::vector<MailData> &result,unsigned int index=0);
    bool retr(unsigned int index, QString &result);
    bool dele(unsigned int index);
    bool rset();
    bool stat(unsigned int &c, unsigned int &s);
    bool top(unsigned int index, _top &t);

    bool islogin;

protected:
    void Pop3waitForResponse();
};

#endif // POP3CLIENT_H
