#include "roomlistmodel.h"

RoomListModel::RoomListModel(QObject *parent /*=NUll*/):QAbstractListModel(parent)
{

}

RoomListModel::~RoomListModel()
{

}
QVariant RoomListModel::data(const QModelIndex &index, int role) const
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
             return m_ItemDataVec[index.row()]->RoomName;

        }
        case Qt::UserRole+1:
        {
            //return m_ItemDataVec[index.row()]->status;
            return -1;
        }
        break;
        case Qt::UserRole+2:
        {
            //return QIcon(m_ItemDataVec[index.row()]->iconPath);
            return QVariant();
        }
        break;
        case Qt::UserRole+3:
        {
            return m_ItemDataVec[index.row()]->messageNumer;
        }
        break;
        case Qt::SizeHintRole:
        {
            return QSize(50,50);
        }
        break;
        }
    }
    return QVariant();
}


int RoomListModel::rowCount(const QModelIndex &parent) const
{
    return m_ItemDataVec.size();

}

void RoomListModel::deleteLItem(int index)
{
    QVector<RoomListData *>::iterator it=m_ItemDataVec.begin();
    m_ItemDataVec.erase(it+index);
}
void RoomListModel::deleteLItem(RoomListData *data)
{
    m_ItemDataVec.removeOne(data);
}
void RoomListModel::addItem(RoomListData *pItem)
{
    if(pItem)
    {
        this->beginInsertRows(QModelIndex(), 0, 1);
        m_ItemDataVec.push_front(pItem);
        this->endInsertRows();
    }
}
RoomListData *RoomListModel::getItem(int index)
{
    if(index>-1&&index<m_ItemDataVec.size())
    {
        return m_ItemDataVec[index];
    }
    return nullptr;
}
