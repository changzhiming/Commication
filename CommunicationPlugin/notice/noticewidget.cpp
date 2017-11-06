#include "noticewidget.h"
#include "ui_noticewidget.h"
#include "sendnoticewidget.h"
#include "seenotice.h"
#include "core/core.h"
#include "wcontainwidget.h"
#include "noticecore.h"
#include <QMenu>
#include <QDebug>
#include <QButtonGroup>
#include <QStackedWidget>

NoticeWidget::NoticeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoticeWidget)
{
    ui->setupUi(this);
 /***************增加下拉在线zhuangtia ************************/

    QMenu *statusButtonMenu = new QMenu(ui->pushButtonTitlIcon);
    statusButtonMenu->addAction(tr("在线"));
    statusButtonMenu->addAction(tr("离开"));
    statusButtonMenu->addAction(tr("忙碌"));
    statusButtonMenu->addAction(tr("离线"));
    ui->pushButtonTitlIcon->setMenu(statusButtonMenu);   //设置状态按钮下拉式菜单
/**************************************************************************/

    sendWidget=new SendNoticeWidget(this);  //发送文件窗体   
    seeNoticeWidget=new SeeNotice(this);
    draftNoticeWidget=new SeeNotice(this);

    /* 增加窗体 */
    ui->contentStackedWidget->addWidget(sendWidget);
    ui->contentStackedWidget->addWidget(seeNoticeWidget);
    ui->contentStackedWidget->addWidget(draftNoticeWidget);

    connect(seeNoticeWidget, SIGNAL(ItemNumChanged(int)), this, SLOT(noticeNumChanged(int)));
    connect(draftNoticeWidget, SIGNAL(ItemNumChanged(int)), this, SLOT(noticeNumChanged(int)));
    connect(seeNoticeWidget, SIGNAL(editItem(XmppClient::NoticeBase)), this, SLOT(editCurrentItem(XmppClient::NoticeBase)));
    connect(draftNoticeWidget, SIGNAL(editItem(XmppClient::NoticeBase)), this, SLOT(editCurrentItem(XmppClient::NoticeBase)));

    /************初始化本地存储的公告 ************************/
    foreach(XmppClient::NoticeBase notice, NoticeCore::getNoticeList(QLatin1String("SeeNotice"))) {
        seeNoticeWidget->addItemToListView(notice);
    }
    foreach(XmppClient::NoticeBase notice, NoticeCore::getNoticeList(QLatin1String("draftNotice"))) {
        draftNoticeWidget->addItemToListView(notice);
    }
    QButtonGroup *buttonGroup=new QButtonGroup(this);
    buttonGroup->addButton(ui->pushButtonSeeNotice);
    buttonGroup->addButton(ui->pushButtonSendNotice);
    buttonGroup->addButton(ui->pushButtonDraftNotice);

    connect(ui->widgetTagBar, SIGNAL(sendclickButton(QString)), this, SLOT(childClick(QString)));
    connect(ui->pushButtonSeeNotice, &QPushButton::clicked, this, &NoticeWidget::pushButtonClick);
    connect(ui->pushButtonSendNotice, &QPushButton::clicked, this, &NoticeWidget::pushButtonClick);
    connect(ui->pushButtonDraftNotice, &QPushButton::clicked, this, &NoticeWidget::pushButtonClick);
    connect(Core::getInstance()->xmppClient, SIGNAL(NoticeReceived(XmppClient::NoticeBase,QString)), seeNoticeWidget, SLOT(NoticeReceived(XmppClient::NoticeBase,QString)));
    connect(sendWidget, SIGNAL(addItemToDraftNotice(XmppClient::NoticeBase)), draftNoticeWidget, SLOT(addItemToListView(XmppClient::NoticeBase)));

    /***********************************************************/
    sendWidget->setProperty("showObjName",("SendNoticeWidget"));
    sendWidget->setProperty("showObjName", ("draftNoticeWidget"));
    ui->pushButtonSendNotice->click();
}

NoticeWidget::~NoticeWidget()
{
    qDebug()<<"delete NoticeWidget";
    NoticeCore::saveNoticeList(seeNoticeWidget->itemList.values(), QStringLiteral("SeeNotice"));
    NoticeCore::saveNoticeList(draftNoticeWidget->itemList.values(), QStringLiteral("draftNotice"));
    delete ui;
}

void NoticeWidget::showChildWindow(QWidget *widget)             //根据名字显示seperatorwidget子类窗体
{
    if(!widget) {
        return;
    }
    ui->contentStackedWidget->setCurrentWidget(widget);
}
void NoticeWidget::childClick(QString notice)   //需要联动
{
    if(notice==QString("发公告")) {
        showChildWindow(sendWidget);
        ui->pushButtonSendNotice->setChecked(true);
    }
    if(notice==QString("查公告")) {
        showChildWindow(seeNoticeWidget);
        ui->pushButtonSeeNotice->setChecked(true);
    }
    if(notice==QString("草稿箱")) {
        showChildWindow(draftNoticeWidget);
        ui->pushButtonDraftNotice->setChecked(true);
    }
}
void NoticeWidget::pushButtonClick()
{
    QPushButton *button=qobject_cast<QPushButton *>(sender());
    if(button) {
        QString sendName=button->text();
        if(sendName==QStringLiteral("发公告")) {
            ui->widgetTagBar->addItem(QStringLiteral("发公告"));
            childClick(QStringLiteral("发公告"));
        } else if(sendName==QStringLiteral("查公告")) {
            ui->widgetTagBar->addItem(QStringLiteral("查公告"));
            childClick(QStringLiteral("查公告"));
        } else if(sendName == QStringLiteral("草稿箱")) {
            ui->widgetTagBar->addItem(QString("草稿箱"));
            childClick(QStringLiteral("草稿箱"));
        }
    }
}

void NoticeWidget::on_toolButtonClose_clicked()
{
    hide();
}

void NoticeWidget::on_toolButtonMin_clicked()   //最小化
{
    if(WContainWidget::MainWidget)
        WContainWidget::MainWidget->showMinimized();
}
void NoticeWidget::noticeNumChanged(int num)
{
    QString objname=sendWidget->property("showObjName").toString();
    if(objname==QStringLiteral("SendNoticeWidget")) {
        ui->labelSeeNum->setText(QString::number(num));
    }
    if(objname==QStringLiteral("draftNoticeWidget")) {
        ui->labelDraftNum->setText(QString::number(num));
    }
}
void NoticeWidget::editCurrentItem(XmppClient::NoticeBase notice)
{
    sendWidget->fillNoticeToEdit(notice);
    showChildWindow(sendWidget);
}
