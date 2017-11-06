#ifndef POP3ACHIEVE_H
#define POP3ACHIEVE_H

#include <QObject>
#include <QMessageBox>
#include <QTimer>
#include "pop3client.h"

class EMAILSHARED_EXPORT Pop3Achieve
{
public:
    Pop3Achieve(QString hostip, QString user, QString password, bool ssl);
    ~Pop3Achieve();

    Pop3Client *pop3;
    QByteArray achieveAll();
    int getStat();
    int getList();
    int getUidl(int index);
    QByteArray getTop(unsigned int index);

    QString getmailid(QString str);
private:
    QString user;
};

#endif // POP3ACHIEVE_H
