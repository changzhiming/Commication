#ifndef SAVECHATLOG_H
#define SAVECHATLOG_H

#include <QObject>
class SaveChatLog:public QObject
{
    Q_OBJECT

public:
    SaveChatLog(QObject *parent=0);
    ~SaveChatLog();
};

#endif // SAVECHATLOG_H
