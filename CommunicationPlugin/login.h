#ifndef LOGIN_H
#define LOGIN_H
#include "core/core.h"
#include "tool/notitle.h"
#include <QWidget>

class QCompleter;
class WContainWidget;
namespace Ui {
class login;
}

class login : public NoTitle
{
    Q_OBJECT

public:
    explicit login(QString showMess=QString(), QWidget *parent = nullptr);
    ~login();

private slots:
    void on_pushButton_2_clicked();
    void on_toolButtonClose_clicked();
    void on_toolButtonMin_clicked();

    void onPushButtonClicked();
    void logInSuccess1();
    void completerActivat(QString text);
    void savePassword(int i);

    void loginError(XmppClient::Error error);            //登陆错误
    void disconnectt(QXmppClient::State status);               //当前的连接状态改变

protected:
    bool eventFilter(QObject *obj, QEvent *ev);               //事件过滤器

private:
    Ui::login *ui;
    QCompleter *completer;
    WContainWidget *containWidget;
};

#endif // LOGIN_H
