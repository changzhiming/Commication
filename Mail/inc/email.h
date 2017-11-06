#ifndef EMAIL_H
#define EMAIL_H

#include <QObject>
#include <QTcpSocket>
#include <QtNetwork/QSslSocket>
#include "email_global.h"

class EmailPrivate;

class EMAILSHARED_EXPORT Email : public QObject
{
public:

    enum AuthMethod
    {
        AuthPlain,
        AuthLogin
    };
    enum SmtpPop3Error
    {
        ConnectionTimeoutError,
        ResponseTimeoutError,
        SendDataTimeoutError,
        AuthenticationFailedError,
        ServerError,    // 4xx smtp error
        ClientError     // 5xx smtp error
    };

    enum ConnectionType
    {
        TcpConnection,
        SslConnection,
        TlsConnection       // STARTTLS
    };

    Email();
    ~Email();

    const QString& getHost() const;
    void setHost(const QString &host);

    int getPort() const;
    void setPort(int port);

    const QString& getName() const;
    void setName(const QString &name);

    ConnectionType getConnectionType() const;
    void setConnectionType(ConnectionType ct);

    const QString & getUser() const;
    void setUser(const QString &user);

    const QString & getPassword() const;
    void setPassword(const QString &password);

    Email::AuthMethod getAuthMethod() const;
    void setAuthMethod(AuthMethod method);

    const QString & getResponseText() const;
    int getResponseCode() const;

    int getConnectionTimeout() const;
    void setConnectionTimeout(int msec);

    int getResponseTimeout() const;
    void setResponseTimeout(int msec);

    int getSendMessageTimeout() const;
    void setSendMessageTimeout(int msec);

    QTcpSocket* getSocket();

    QTcpSocket *socket;

    QString host;
    int port;
    QString name;
    QString user;
    QString password;
    AuthMethod authMethod;
    ConnectionType connectionType;

    int connectionTimeOut;
    int responseTimeOut;
    int sendMessageTimeout;

    QString resposeText;
    char Pop3responseText[1024];
    int responseCode;

    class ResponseTimeoutException {};
    class SendMessageTimeoutException {};

    void sendMessage(const QString &text);

    /* [5] Protected methods */

//    void SmtpwaitForResponse();
//    void Pop3waitForResponse();
    /* [5] --- */

protected slots:

    /* [6] Protected slots */

    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketError(QAbstractSocket::SocketError error);
    void socketReadyRead();

    /* [6] --- */

};

#endif // EMAIL_H
