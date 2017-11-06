#ifndef ROOMLISTMODEL_H
#define ROOMLISTMODEL_H
#include <QAbstractListModel>
#include <QObject>
#include <groupChat/groupchat.h>
#include "xmpp/xmppclient.h"

struct RoomListData  //本地好友结构体
{
    QString RoomName=QString();
    GroupChat *groupChat=nullptr;
    QString iconPath;
    QString  messageNumer;
    QXmppMucRoom *room;
    //QStringList friendList;
};
class RoomListModel : public QAbstractListModel
{
public:
    RoomListModel(QObject *parent=nullptr);
    ~RoomListModel();

    void addItem(RoomListData *pItem);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent=QModelIndex())const;
    void deleteLItem(int index);
    void deleteLItem(RoomListData *data);

    //Qt::ItemFlags flags(const QModelIndex &index) const;  //增加编辑功能
    RoomListData * getItem(int index);

public:
    QVector<RoomListData *> m_ItemDataVec;
};

#endif // ROOMLISTMODEL_H
