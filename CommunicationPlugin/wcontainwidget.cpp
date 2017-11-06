#include "wcontainwidget.h"
#include "ui_wcontainwidget.h"
#include "savesetting.h"
#include "core/myapplication.h"
#include "settingui.h"
#include "notice/noticewidget.h"
#include "../Mail/inc/widget.h"
#include "friendlist.h"
#include "listFriend/my_listmodel.h"
#include <QPalette>
#include <friendlist.h>

WContainWidget * WContainWidget::MainWidget(nullptr);
WContainWidget::WContainWidget(QWidget *parent) :
    NoTitle(parent),
    ui(new Ui::WContainWidget)
{
    ui->setupUi(this);
    classInit();
}
void WContainWidget::classInit()
{
    /*******************初始化系统托盘图标****************************/
    MainWidget=this;
    tryIcon=new QSystemTrayIcon(QIcon(QStringLiteral(":/icon/image/logo22.ico")) , this);
    tryIcon->setToolTip("局域网聊天");
    QMenu *tryMenu=new QMenu(this);
    tryMenu->addAction(tr("退出"), this, SLOT(saveAndQuit()));
    tryIcon->setContextMenu(tryMenu);
    connect(tryIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    if(SaveSetting::getInstance().getShowSystemTray()){
            tryIcon->show();
    }
    FriendList *friendList= new FriendList(ui->widgetCenter);
    NoticeWidget *noticewidget=new NoticeWidget(ui->widgetCenter);
    Widget * mailWidget=new Widget(ui->widgetCenter);

    //mailWidget->setuser(FriendList::friendlist->myData->Id);
    //qDebug()<<"ssssssssssssssssssssssssssssssssssss"<<FriendList::friendlist->myData->Id;
    mailWidget->setuser(FriendList::friendlist->myData->Id);
    mailWidget->setpassword();
    mailWidget->sethostip();

/**************************增加导航栏标题***************************/
    QToolButton *toolButtonzh=new QToolButton(this);
    ui->widgetNavigation->addItem(toolButtonzh, nullptr, "toolButtonzh");
    QToolButton *toolButtonchat=new QToolButton(this);
    ui->widgetNavigation->addItem(toolButtonchat, friendList, "toolButtonchat");
    QToolButton *toolButtongg=new QToolButton(this);
    ui->widgetNavigation->addItem(toolButtongg, noticewidget, "toolButtongg");
    QToolButton *toolButtonwj=new QToolButton(this);
    ui->widgetNavigation->addItem(toolButtonwj, nullptr, "toolButtonwj");
    QToolButton *toolButtonem=new QToolButton(this);
    ui->widgetNavigation->addItem(toolButtonem, mailWidget, "toolButtonem");
    QToolButton *toolButtoncz=new QToolButton(this);
    ui->widgetNavigation->addItem(toolButtoncz, nullptr, "toolButtoncz");
/*****************************************************************************/

    initWindow(ui->horizontalLayoutCenter, friendList, tr("friendList"), true);
    initWindow(ui->horizontalLayoutCenter, noticewidget, tr("noticewidget"), false);
    //initWindow(ui->horizontalLayoutCenter, nullptr, tr("noticewidget"), false);
    initWindow(ui->horizontalLayoutCenter, mailWidget, tr("mailWidget"), false);

    connect(ui->widgetNavigation, SIGNAL(clickShowWidget(QWidget*)), this, SLOT(showWindow(QWidget *)));
    connect(qobject_cast<MyApplication *>(qApp), SIGNAL(mainWindowsShow()), this, SLOT(acticeMainWindow()));
    connect(FriendList::friendlist->settingui, &SettingUi::sysTryIconchanged, this, &WContainWidget::sysTryIconChanged);       //是否显示系统托盘
    restoreGeometry(SaveSetting::getInstance().getWindowGeometry());       //重新加载窗体位置
}

WContainWidget::~WContainWidget()
{
    delete ui;
    deleteHandle();
}

void WContainWidget::initWindow(QLayout *layout, QWidget *win, QString s, bool show_hide)
{
    layout->addWidget(win);
    win->setObjectName(s);
    if(!show_hide)
        win->hide();
}

void WContainWidget::showWindow(QWidget *widget)             //根据名字显示子类窗体
{
    if(!widget)
        return;
    QList<QWidget *> widgets = ui->widgetCenter->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly);
    foreach(QWidget *child, widgets)
    {
        if(child==widget)
            child->show();
        else
            child->hide();
    }
}
void WContainWidget::deleteHandle()
{
    SaveSetting::getInstance().setWindowGeometry(saveGeometry());
}
void WContainWidget::saveAndQuit()
{
    qApp->quit();
}
void WContainWidget::trayIconActivated(QSystemTrayIcon::ActivationReason activationReason)
{
    if(activationReason==QSystemTrayIcon::Trigger){
            show();
            activateWindow();          //使窗体变为顶层窗体显示
        }
}
void WContainWidget::acticeMainWindow()
{
    if(!this->isVisible()){
      activateWindow();          //使窗体变为顶层窗体显示
    }
}

void WContainWidget::closeEvent(QCloseEvent *event)
{
//    if(SaveSetting::getInstance().getShowSystemTray()){
//        event->ignore();   //事件继续传递
//        this->setVisible(false);
//    }
//    else{
//        QWidget::closeEvent(event);
//    }
    QWidget::closeEvent(event);
}
void WContainWidget::sysTryIconChanged(bool change)
{
    tryIcon->setVisible(change);
}
