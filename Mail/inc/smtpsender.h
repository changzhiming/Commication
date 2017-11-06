#ifndef SMTPSENDER_H
#define SMTPSENDER_H

#include <QMessageBox>
#include <QErrorMessage>
#include <QListWidget>
#include <QFile>
#include <QDataStream>
#include "SmtpMime"
#include "emaillocalfile.h"

class EMAILSHARED_EXPORT SmtpSender
{
public:
    SmtpSender();
    ~SmtpSender();

    EmailLocalFile *lf;

    static EmailAddress * stringToEmail(const QString & str);

    bool send(QString hostip,QString Sender, QString Pass, QString recipient, QString CcRecipient, QString subject,
              QTextDocument *Content, QListWidget *attemptPath, bool ssl);
    void sethost(QString host);
signals:

public slots:

private:
    QFile* file;
    QString host;
    bool stat;
};

#endif // SMTPSENDER_H
