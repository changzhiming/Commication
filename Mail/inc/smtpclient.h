#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include<QtNetwork/QTcpSocket>
#include <QtNetwork/QSslSocket>


#include "mimemessage.h"
#include "email.h"

class EMAILSHARED_EXPORT SmtpClient : public Email
{
public:
    SmtpClient(const QString & host = "localhost", int port = 0, ConnectionType ct = TcpConnection);
    ~SmtpClient();

    bool SmtpconnectToHost();

    bool login();
    bool login(const QString &user, const QString &password, AuthMethod method = AuthLogin);

    bool sendMail(MimeMessage& email);

    void quit();

protected:
    void SmtpwaitForResponse();

private:
    Email *e;
};

#endif // SMTPCLIENT_H
