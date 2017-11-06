#ifndef NOTICELISTVIEW_H
#define NOTICELISTVIEW_H
#include <QListView>
#include "noticelistmodel.h"

class NoticeListView : public QListView
{
    Q_OBJECT
public:
    NoticeListView(QWidget *parent=0);
    ~NoticeListView();

    void addItem(NoticeListModel::noticeListItem *pItem);
    NoticeListModel *m_pModel;

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
signals:
    void deleteItem(NoticeListModel::noticeListItem *item);

private slots:
    void deleteItemSlot(bool bDelete);

private:
    QMenu *pMenu;
};

#endif // NOTICELISTVIEW_H
