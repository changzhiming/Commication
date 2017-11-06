#ifndef NOTICEWIDGET_H
#define NOTICEWIDGET_H
#include "tagwidget.h"
#include "core/core.h"
#include <QWidget>
class SeeNotice;
class SendNoticeWidget;
namespace Ui {
class NoticeWidget;
}

class NoticeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NoticeWidget(QWidget *parent = 0);
    ~NoticeWidget();
    void showChildWindow(QWidget *widget);

private slots:
    void childClick(QString notice);
    void pushButtonClick();

    void on_toolButtonClose_clicked();

    void on_toolButtonMin_clicked();
    void noticeNumChanged(int num);
    void editCurrentItem(XmppClient::NoticeBase notice);

private:
    Ui::NoticeWidget *ui;
    SendNoticeWidget *sendWidget;
    SeeNotice *seeNoticeWidget;
    SeeNotice *draftNoticeWidget;
    QWidget *ContentWidget;
};

#endif // NOTICEWIDGET_H
