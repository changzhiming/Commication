#ifndef READEMAILWIDGET_H
#define READEMAILWIDGET_H

#include <QWidget>
#include <QFile>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QList>
#include <QTextCodec>
#include <QDebug>
#include <QStyledItemDelegate>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QtGlobal>
#include <QFileDialog>

#include "emailmanager.h"
#include "ctagwidget.h"
#include "emaildisplay.h"
using namespace Mail;

namespace Ui {
class ReademailWidget;
}

class ReademailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReademailWidget(QWidget *parent = 0);
    ~ReademailWidget();

    EmailManager manager;

    EmailDisplay *emaildispaly;

    QList<QString> lists;
    QList<QString> listcopy;
    QList<QString> filterlist;
    QList<EmailManager::_top> listtop;

    typedef struct transmit
    {
        QTextDocument *doc;
        //QObjectList objlist;
        QList<QListWidgetItem *> lw;
    }_trans;

    void recvmailstylesheetset();
    //删除邮件
    void tablewidgetitemdelete();
    //转发邮件
    void tablewidgetitemtransmit();


    //邮箱处理
    void recyclebox();
    void scribblebox();
    //tablewidgettest星标菜单后刷新显示
    void tablewidgetdisplay(QList<QString> listcopy, QString wh = "recv");
    void tablewidgetinit();
    void tablewidgetset(int i, QString text);
    void tablewidgetsetrs(int i,QString text);
    QPushButton *button1;
    QPushButton *button2;

    //附件显示
    void attemptdisplay(QList<QString> filelist);
    //初始读取界面
    void slotemailhandle(CTagWidget::EmailLabel mail);
    //标记为未读菜单
    void tablewidgetitemsignunread();
    //星标菜单处理
    void tablewidgetitemsignstar();
    //星标置顶自动刷新
    void starsortupdate(QList<QString> listcopy);

    //void paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const;
    //只读未读处理函数
    //void readunreadfun(int i);

    void username(QString user);

private:
    Ui::ReademailWidget *ui;
    int flag;

    bool button1ischecked = false;

    QSortFilterProxyModel *proxyModel;
    QStandardItemModel *model;

    QString user;

private slots:
    void ShowcontextMenu(QPoint pos);

    void ShowattemptMenu(QPoint pos);

    void slottableitemclock(QTableWidgetItem *it);

    void slotopenattempt(QListWidgetItem *item);

    //已读未读button1处理
    void slotreadlistbutton();
    //星标置顶button2处理
    void slotstarlistbutton();
    //图标设置
    QString seticon(bool ok, bool x);

    //过滤处理
    void slotfilter();
    //过滤下拉列表
    void filtercombox(QString str);
    //快速回复发送
    void slotreplysend();

signals:
    void signaltransmit(ReademailWidget::_trans);

    void updatewidget(QString);

    void signalreply(QString recipient,QTextDocument* content);

    void signalopenattemptfailed(QString str, bool b,QString name);

    void updatestat();
};

#endif // READEMAILWIDGET_H
