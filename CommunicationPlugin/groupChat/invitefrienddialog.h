#ifndef INVITEFRIENDDIALOG_H
#define INVITEFRIENDDIALOG_H

#include "tool/notitle.h"
#include <QStandardItem>
struct ListItemData;
namespace Ui {
class inviteWidget;
}
class ChechFriendWidget;

class InviteFriendDialog : public NoTitle
{
    Q_OBJECT

public:
    explicit InviteFriendDialog(QWidget *parent = 0);
    ~InviteFriendDialog();

    void setRoomId(QString iD);

    QString getReason();
    void setTitleScreen();
signals:
    void sendJidList(QStringList list);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
private slots:
    void initSelectFriendList(QHash<QString, ListItemData *> friendList);

    void on_pushButtonCancle_clicked();

    void on_toolButtonClose_clicked();

    void on_toolButtonMin_clicked();

    void on_pushButtonOk_clicked();
    void on_toolButtonLeft_clicked();
    void on_toolButtonRight_clicked();
    void on_pushButtonAll_clicked();

    void on_pushButtonDelete_clicked();

    void on_lineEditSearch_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::inviteWidget *ui;
    ChechFriendWidget *widget;

    QHash<QString, QStandardItem *> group;               //组名和组项目
};

#endif // INVITEFRIENDDIALOG_H
