#ifndef NOTICECORE_H
#define NOTICECORE_H

#include "core/core.h"
#include "noticelistmodel.h"

#include <QObject>

class NoticeCore : public QObject
{
    Q_OBJECT
public:
    explicit NoticeCore(QObject *parent = 0);
    ~NoticeCore();

    //QHash<NoticeListModel::noticeListItem *, XmppClient::NoticeBase> seeNoticeitemList;
    //QHash<NoticeListModel::noticeListItem *, XmppClient::NoticeBase> draftNoticeitemList;

    static QString noticeBaseToHtml(XmppClient::NoticeBase &notice);
    static void saveNoticeList(QList<XmppClient::NoticeBase> itemList, QString iniName);
    static QList<XmppClient::NoticeBase> getNoticeList(QString iniName);
    static NoticeCore *thisNoticeCore;
};

#endif // NOTICECORE_H
