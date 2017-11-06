#include "my_listmodel.h"
#include "friendlist.h"
/*
 * 列表模型类，主要实现好友显示模型
 */
My_ListModel::My_ListModel(QObject *parent /*=NUll*/):QAbstractListModel(parent)
{

}

My_ListModel::~My_ListModel()
{

}

QVariant My_ListModel::data(const QModelIndex &index, int role) const
{

    //qDebug()<<index.row();

    if(index.row()>m_ItemDataVec.size()||index.row()<0)
    {
        return QVariant();
    }
    else
    {
        switch(role)
        {
        case Qt::UserRole:
        {
                return m_ItemDataVec[index.row()]->Name;
        }
        case Qt::UserRole+1:
        {
            return m_ItemDataVec[index.row()]->status;
        }
        break;
        case Qt::UserRole+2:
        {
            return QIcon(m_ItemDataVec[index.row()]->iconPath);
        }
        break;
        case Qt::UserRole+3:
        {
            return m_ItemDataVec[index.row()]->messageNumer;
        }
        break;
        case Qt::SizeHintRole:
        {
            return this->size;
        }

        break;
        }
    }
    return QVariant();
}

int My_ListModel::rowCount(const QModelIndex &parent) const
{
    return m_ItemDataVec.size();

}

void My_ListModel::deleteLItem(int index)
{
    if(index>-1)
    {
        this->beginRemoveRows(QModelIndex(), index, index);
        m_ItemDataVec.removeAt(index);
        endRemoveRows();
    }
}
void My_ListModel::deleteLItem(ListItemData *data)
{

    if(data)
    {
        int index=m_ItemDataVec.indexOf(data);
        if(index>-1)
        {
            this->beginRemoveRows(QModelIndex(), index, index);
            m_ItemDataVec.removeOne(data);
            endRemoveRows();
        }
    }
}

void My_ListModel::addItem(ListItemData *pItem)
{
    if(pItem)
    {
        this->beginInsertRows(QModelIndex(), 0, 0);
        m_ItemDataVec.push_front(pItem);
        this->endInsertRows();
    }
}
ListItemData *My_ListModel::getItem(int index)
{
    if(index>-1&&index<m_ItemDataVec.size())
    {
        return m_ItemDataVec[index];
    }
    return nullptr;
}
void My_ListModel::setSize(QSize size)
{
    this->size=size;
}

QString My_ListModel::getIconPath(XmppClient::StatusType status)
{
    switch (status)
    {
    case XmppClient::Online:
        return QString(":/statu/image/status/在线");
    case XmppClient::Away:
        return QString(":/statu/image/status/离开");
    case XmppClient::XA:
        return QString(":/statu/image/status/离线");
    case XmppClient::DND:
        return QString(":/statu/image/status/忙碌");
    case XmppClient::Chat:
        return QString(":/statu/image/status/忙碌");
    case XmppClient::Unavailable:
        return QString(":/statu/image/status/离线");
    default:
        return QString(":/statu/image/status/离线");
    }
}
