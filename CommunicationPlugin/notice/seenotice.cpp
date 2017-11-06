#include "seenotice.h"
#include "ui_seenotice.h"
#include <QTextBrowser>
#include <QSettings>
#include "notice/noticecore.h"
#include <QLabel>
SeeNotice::SeeNotice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SeeNotice)
{
    ui->setupUi(this);
    currentShowItem=nullptr;
    ui->textEdit->setContentsMargins(20, 20, 20, 20);
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showNotice(QModelIndex)));
    connect(ui->listView, SIGNAL(deleteItem(NoticeListModel::noticeListItem*)), this, SLOT(viewDeleteItem(NoticeListModel::noticeListItem*)));
    ui->splitter->setStretchFactor(1,1); 
    ui->textEdit->setContentsMargins(20, 20, 20, 20);
}

SeeNotice::~SeeNotice()
{
    delete ui;
    foreach(NoticeListModel::noticeListItem *item, itemList.keys()) {
        delete item;
    }
}
void SeeNotice::NoticeReceived(XmppClient::NoticeBase notice, QString noticeXml)
{
    addItemToListView(notice);
}
void SeeNotice::addItemToListView(XmppClient::NoticeBase notice)
{
    NoticeListModel::noticeListItem *item=new NoticeListModel::noticeListItem();

    item->date =notice.date;
    item->title= notice.Subject;
    item->department=notice.Organization;
    qDebug()<<item->date<<item->title<<item->department;
    if(notice.Urgency)
        item->importLevelFlags |=NoticeListModel::Urgency;
    if(notice.Important)
        item->importLevelFlags |=NoticeListModel::Import;
    if(notice.Secret)
        item->importLevelFlags |=NoticeListModel::Secret;

    ui->listView->addItem(item);
    itemList.insert(item, notice);
    emit ItemNumChanged(itemList.size());
}
void SeeNotice::showNotice(QModelIndex index)
{
    NoticeListView *listview=qobject_cast<NoticeListView *>(sender());
    if(listview) {
        currentShowItem=listview->m_pModel->getItem(index.row());
        if(currentShowItem) {
            for(QHash<NoticeListModel::noticeListItem *, XmppClient::NoticeBase>::iterator i=itemList.begin(); i!=itemList.end(); i++)
            {
                if(i.key()==currentShowItem) {
                    ui->textEdit->setHtml(NoticeCore::noticeBaseToHtml(i.value()));
                    break;
                }
            }
        }
    }
}

void SeeNotice::on_pushButton_2_clicked()     //删除当前项
{
    int index=ui->listView->currentIndex().row();
    if(index >-1){
        ui->listView->m_pModel->deleteLItem(index);
        itemList.remove(ui->listView->m_pModel->getItem(index));
        ui->textEdit->clear();
    }
}

void SeeNotice::on_pushButton_clicked()   //编辑当前项
{
    int index=ui->listView->currentIndex().row();
    NoticeListModel::noticeListItem *item=nullptr;
    if(index >-1){
        item=ui->listView->m_pModel->getItem(index);
    } else {
        return;
    }
    if(item) {
        XmppClient::NoticeBase notice=itemList.value(item);
        emit editItem(notice);
    }
}
void SeeNotice::viewDeleteItem(NoticeListModel::noticeListItem *item)
{
    if(item==currentShowItem) {
        ui->textEdit->clear();
    }
    itemList.remove(item);
    delete item;
}
