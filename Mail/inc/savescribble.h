#ifndef SAVESCRIBBLE_H
#define SAVESCRIBBLE_H

#include <QMessageBox>
#include <QErrorMessage>
#include <QListWidget>
#include <QFile>
#include <QDataStream>
#include "SmtpMime"
#include "emaillocalfile.h"


class EMAILSHARED_EXPORT SaveScribble
{
public:
    SaveScribble();
    ~SaveScribble();
    EmailLocalFile *lf;

    static EmailAddress * stringToEmail(const QString & str);

    void save(QString Sender, QString recipient, QString subject,
              QTextDocument *Content, QListWidget *attemptPath);
private:
    QFile* file;
};

#endif // SAVESCRIBBLE_H
