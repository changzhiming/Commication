#ifndef SEENOTICE_H
#define SEENOTICE_H
#include <QWidget>
#include "core/core.h"
#include "noticelistmodel.h"

namespace Ui {
class SeeNotice;
}

class SeeNotice : public QWidget
{
    Q_OBJECT

public:
    explicit SeeNotice(QWidget *parent = 0);
    ~SeeNotice();

    QHash<NoticeListModel::noticeListItem *, XmppClient::NoticeBase> itemList;

public slots:
    void addItemToListView(XmppClient::NoticeBase notice);
    void NoticeReceived(XmppClient::NoticeBase notice, QString noticeXml);
    void showNotice(QModelIndex index);
signals:
    void ItemNumChanged(int num);
    void editItem(XmppClient::NoticeBase notice);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();
    void viewDeleteItem(NoticeListModel::noticeListItem *item);

private:
    Ui::SeeNotice *ui;
    NoticeListModel::noticeListItem *currentShowItem;
};

#endif // SEENOTICE_H
