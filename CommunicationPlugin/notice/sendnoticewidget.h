 #ifndef SENDNOTICEWIDGET_H
#define SENDNOTICEWIDGET_H
#include <core/core.h>
#include <QWidget>

namespace Ui {
class SendNoticeWidget;
}
class InviteFriendDialog;

class SendNoticeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SendNoticeWidget(QWidget *parent = 0);
    ~SendNoticeWidget();
    void fillNoticeToEdit(XmppClient::NoticeBase noticebase);

private slots:
    void on_fontSizecomboBox_currentIndexChanged(QString index);

    void on_fontComboBox_currentIndexChanged(const QString &arg1);
    void cursorPositionChanged();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();
    void sendFriend(QStringList list);
    void on_pushButton_2_clicked();

signals:
    void addItemToDraftNotice(XmppClient::NoticeBase noticebase);

private:
    Ui::SendNoticeWidget *ui;
    InviteFriendDialog *inviteDailog;
    QStringList friendList;
};

#endif // SENDNOTICEWIDGET_H
