#ifndef NOTICELISTMODEL_H
#define NOTICELISTMODEL_H
#include <QFlags>
#include <QAbstractListModel>
#include <QSize>

class NoticeListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_FLAGS(ImportantLevels)

public:

    enum ImportantLevel
    {
        NoImport=0x0,
        Import = 0x1,
        Urgency = 0x2,
        Secret = 0x4,
    };
    Q_DECLARE_FLAGS(ImportantLevels, ImportantLevel)

    struct noticeListItem
    {
        QString title;         //标题
        QString date;          //日期
        QString department;     //单位
        ImportantLevels importLevelFlags;
    };

    NoticeListModel(QObject *parent);
    ~NoticeListModel();
    static QString getIconPath(NoticeListModel::ImportantLevels status);

    void addItem(noticeListItem *pItem);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent=QModelIndex())const;
    void deleteLItem(int index);
    void deleteLItem(noticeListItem *data);

    noticeListItem * getItem(int index);

    void setSize(QSize size);

public:
    QVector<noticeListItem *> m_ItemDataVec;
    QSize size=QSize(80, 80);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(NoticeListModel::ImportantLevels)

#endif // NOTICELISTMODEL_H
