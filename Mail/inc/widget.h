#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <QTimer>
#include <QTextDocument>
#include "emailmanager.h"
#include "ctagwidget.h"
#include "reademailwidget.h"

class PromptDialog;
class EmailManager;
class SendemailWidget;
class ReademailWidget;
class ETopTitleWidget;
class AddressWidget;
using namespace Mail;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    EmailManager manager;

    void showChildWindow(QString objectName);
    void addChildWindow(QLayout *layout, QWidget *win, QString s, bool showOrhide);

    void leftwidgetarea();

    void setuser(QString user);
    void setpassword(QString password = "123456");
    void sethostip(QString host = "192.168.20.10");

private:
    Ui::Widget *ui;
    SendemailWidget *sendwidget;
    ReademailWidget *readwidget;
    QWidget *ContentWidget;
    ETopTitleWidget *TitleWidget;
    AddressWidget *addrwidget;
    PromptDialog *promdialog;

    QString user;
    QString password;
    QString host;

private slots:
    void childClick(CTagWidget::EmailLabel mail);
    void pushButtonClick();
    //void showChildWindow(QString objname);
    void slotachieveemail();
    //transmit
    void slottransmit(ReademailWidget::_trans transmit);

    void slotchangewidget(QString sendName);

    void slotpromptdialog(QString title, bool val = false, QString str="test");
    //quick reply
    void slotreply(QString recipient,QTextDocument* content);

    void displaystat();

signals:
    void widgetchange(CTagWidget::EmailLabel notic);
};

#endif // WIDGET_H
