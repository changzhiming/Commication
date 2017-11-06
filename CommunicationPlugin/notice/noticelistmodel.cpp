#include "noticelistmodel.h"
#include <QIcon>
#include <QDebug>

NoticeListModel::NoticeListModel(QObject *parent):QAbstractListModel(parent)
{
}

NoticeListModel::~NoticeListModel()
{
}
QVariant NoticeListModel::data(const QModelIndex &index, int role) const
{
    if(index.row()>m_ItemDataVec.size()||index.row()<0) {
        return QVariant();
    } else {
        switch(role)
        {
        case Qt::UserRole:
             return m_ItemDataVec[index.row()]->title;
             break;
        case Qt::UserRole+1:
            return m_ItemDataVec[index.row()]->department;
            break;
        case Qt::UserRole+2:
            return QIcon(getIconPath(m_ItemDataVec[index.row()]->importLevelFlags));
            break;
        case Qt::UserRole+3:
            return m_ItemDataVec[index.row()]->date;
            break;
        case Qt::SizeHintRole:
            return this->size;
            break;
        }
    }
    return QVariant();
}

int NoticeListModel::rowCount(const QModelIndex &parent) const
{
    return m_ItemDataVec.size();
}

void NoticeListModel::deleteLItem(int index)
{
    if(index>-1) {
        this->beginRemoveRows(QModelIndex(), index, index);
        m_ItemDataVec.removeAt(index);
        endRemoveRows();
    }
}
void NoticeListModel::deleteLItem(noticeListItem *data)
{
    if(data) {
        int index=m_ItemDataVec.indexOf(data);
        if(index>-1) {
            this->beginRemoveRows(QModelIndex(), index, index);
            m_ItemDataVec.removeOne(data);
            endRemoveRows();
        }
    }
}

void NoticeListModel::addItem(noticeListItem *pItem)
{
    if(pItem) {
        this->beginInsertRows(QModelIndex(), 0, 0);
        m_ItemDataVec.push_front(pItem);
        this->endInsertRows();
    }
}
NoticeListModel::noticeListItem *NoticeListModel::getItem(int index)
{
    if(index>-1&&index<m_ItemDataVec.size())
        return m_ItemDataVec[index];
    return nullptr;
}
void NoticeListModel::setSize(QSize size)
{
    this->size=size;
}
QString NoticeListModel::getIconPath(NoticeListModel::ImportantLevels status)
{
    /*if(status.testFlag(NoticeListModel::Import)) {
        return QString("");
    }
    if(status.testFlag(NoticeListModel::Urgency)) {
        return QString("");
    }
    if(status.testFlag(NoticeListModel::Secret)) {
        return QString("");
    }*/
    return QString(":/icon/image/man1.png");
}
