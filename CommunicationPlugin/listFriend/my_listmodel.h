#ifndef MY_LISTMODEL_H
#define MY_LISTMODEL_H
#include <QObject>
#include <QAbstractListModel>
#include <QVariant>
#include <QVector>
#include <QFile>
#include <QIcon>
#include <QDebug>
#include "xmpp/xmppclient.h"
enum class Status;
class chatWindow;
struct ListItemData  //本地好友结构体
{
    QString iconPath=":/icon/image/three.png";
    QString Name=QString();
    QString Id=QString();
    QString GroupId=QString();
    QString Group=QString();
    XmppClient::StatusType status=XmppClient::Unavailable;
    XmppClient::FriendVcard vcard;
    chatWindow *chatwindow=nullptr;
    QString  messageNumer;

};

class My_ListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit My_ListModel(QObject *parent=nullptr);

    ~My_ListModel();
    void addItem(ListItemData *pItem);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent=QModelIndex())const;
    void deleteLItem(int index);
    void deleteLItem(ListItemData *data);
    //Qt::ItemFlags flags(const QModelIndex &index) const;  //增加编辑功能
    ListItemData * getItem(int index);
    static QString getIconPath(XmppClient::StatusType status);
    void setSize(QSize size);

public:
    QVector<ListItemData *> m_ItemDataVec;
    QSize size=QSize(50, 50);
};

#endif // MY_LISTMODEL_H
