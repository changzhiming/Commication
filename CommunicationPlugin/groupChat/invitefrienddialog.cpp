
#include "mainFunction/functionchoicewidget.h"
#include "listFriend/my_listmodel.h"
#include "invitefrienddialog.h"
#include "friendlist.h"
#include "ui_invitefriendwidget.h"

InviteFriendDialog::InviteFriendDialog(QWidget *parent) :
    NoTitle(parent),
    ui(new Ui::inviteWidget)
{
    ui->setupUi(this);
    widget=ui->chechFriendWidget;
    if(FriendList::friendlist)
        initSelectFriendList(FriendList::friendlist->friendList);
    ui->listView->m_pModel->setSize(QSize(30, 30));
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    widget->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->lineEditSearch->installEventFilter(this);
    setAttribute(Qt::WA_NoSystemBackground, false);
}

InviteFriendDialog::~InviteFriendDialog()
{
    delete ui;
}

bool InviteFriendDialog::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj==ui->lineEditSearch)
    {
        if(ev->type()==QEvent::FocusIn)
        {
            ui->lineEditSearch->setStyleSheet("QLineEdit{}");
        }
        if(ev->type()==QEvent::FocusOut)
        {
            ui->lineEditSearch->setStyleSheet("QLineEdit{\
                                                  border: 1px solid gray;\
                                                  background-image: url(:/icon/image/search.png);\
                                                  background-position:right center;\
                                                  background-repeat: no-repeat;\
                                                  }");
        }
    }
    return QWidget::eventFilter(obj, ev);
}

void InviteFriendDialog::initSelectFriendList(QHash<QString, ListItemData *> friendList)
{
    foreach (ListItemData *data, friendList.values())
    {
        QStandardItem *item=new QStandardItem;

        if(group.contains(data->Group))
        {
            item->setText(data->Id);
            item->setIcon(QIcon(data->iconPath));
            widget->addItem(group.value(data->Group),  item);

        }
        else
        {
            group.insert(data->Group, item);
            item->setText(data->Group);
            widget->addItem(this->widget->getRootItem(), item);

            QStandardItem *item=new QStandardItem;
            item->setText(data->Id);
            item->setIcon(QIcon(data->iconPath));
            widget->addItem(group.value(data->Group),  item);
        }
    }
}

void InviteFriendDialog::on_pushButtonCancle_clicked()
{
    this->close();
}
void InviteFriendDialog::setRoomId(QString iD)
{
    ui->lineEditRoom->setText(iD);
}

QString InviteFriendDialog::getReason()
{
    return ui->lineEditReason->text();
}
void InviteFriendDialog::setTitleScreen()
{
    delete ui->lineEditReason;
    delete ui->lineEditRoom;
    delete ui->label_4;
    delete ui->label_6;
    delete ui->formLayout_2;
}

void InviteFriendDialog::on_toolButtonClose_clicked()
{
    this->close();
}

void InviteFriendDialog::on_toolButtonMin_clicked()
{
    this->showMinimized();
}

void InviteFriendDialog::on_pushButtonOk_clicked()
{
    QStringList jidList;
    int size=ui->listView->m_pModel->rowCount();
    for(int i=size-1; i>=0; i--)
    {
        ListItemData* data=ui->listView->m_pModel->getItem(i);
        if(data)
        {
            jidList.append(data->Id);
        }
    }
    emit sendJidList(jidList);
    close();
}

void InviteFriendDialog::on_toolButtonLeft_clicked()
{
    while(ui->listView->selectionModel()->hasSelection())
    {
        QModelIndex index= ui->listView->selectionModel()->selectedRows().first();

        ListItemData* data=ui->listView->m_pModel->getItem(index.row());
        QStandardItem *item=new QStandardItem;

        if(group.contains(data->Group))
        {
                item->setText(data->Id);
                item->setIcon(QIcon(data->iconPath));
                widget->addItem(group.value(data->Group),  item);

        }
        else
        {
                group.insert(data->Group, item);
                item->setText(data->Group);
                widget->addItem(this->widget->getRootItem(), item);

                QStandardItem *item=new QStandardItem;
                item->setText(data->Id);
                item->setIcon(QIcon(data->iconPath));
                widget->addItem(group.value(data->Group),  item);
        }
        if(index.row()>-1)
        {
            ui->listView->m_pModel->deleteLItem(index.row());
        }
    }
}


void InviteFriendDialog::on_toolButtonRight_clicked()
{
    while(widget->treeView->selectionModel()->hasSelection())
    {
        QModelIndex index= widget->treeView->selectionModel()->selectedRows().first();
        QString jid=index.data(Qt::DisplayRole).toString();
        qDebug()<<jid<<index.row();
        if(group.contains(jid))
        {
            widget->treeView->selectionModel()->select(index, QItemSelectionModel::Clear);
        }
        if(FriendList::friendlist->friendList.contains(jid))
        {
            ListItemData* data=FriendList::friendlist->friendList.value(jid);
            if(!ui->listView->m_pModel->m_ItemDataVec.contains(data))
            {
                ui->listView->addItem(data);

                widget->treeView->model()->removeRow(index.row(), index.parent());
            }
        }
        foreach (QStandardItem *item, group.values()) {

            if(!item->hasChildren())
            {
                //qDebug()<<group.size();
                QStandardItem *parent=item->parent();
                if(!parent)
                {
                    parent=widget->getRootItem();
                }
                parent->removeRow(item->row());
                group.remove(group.key(item));
            }
        }
    }
}


void InviteFriendDialog::on_pushButtonAll_clicked()
{
    widget->treeView->selectAll();
}

void InviteFriendDialog::on_pushButtonDelete_clicked()
{
    int size=ui->listView->m_pModel->rowCount();
    for(int i=size-1; i>=0; i--)
    {
        ListItemData* data=ui->listView->m_pModel->getItem(i);
        QStandardItem *item=new QStandardItem;

        if(group.contains(data->Group))
        {
                item->setText(data->Id);
                item->setIcon(QIcon(data->iconPath));
                widget->addItem(group.value(data->Group),  item);

        }
        else
        {
                group.insert(data->Group, item);
                item->setText(data->Group);
                widget->addItem(this->widget->getRootItem(), item);

                QStandardItem *item=new QStandardItem;
                item->setText(data->Id);
                item->setIcon(QIcon(data->iconPath));
                widget->addItem(group.value(data->Group),  item);
        }

        ui->listView->m_pModel->deleteLItem(i);
    }
    ui->listView->update();
}
/*
 * 搜索
 */
void InviteFriendDialog::on_lineEditSearch_cursorPositionChanged(int arg1, int arg2)
{

    QString searchText=ui->lineEditSearch->text();
    //qDebug()<<searchText<<"on_lineEditSearch_cursorPositionChanged";
    foreach (QStandardItem *item, group.values()) {
        if(item->hasChildren())
        {
            for(int i=0; i<item->rowCount(); i++)
            {
                QStandardItem *childitem=item->child(i);
                if((!searchText.isEmpty())&&childitem->text().startsWith(searchText))
                {
                    qDebug()<<childitem->text()<<searchText;
                    widget->treeView->setCurrentIndex(childitem->index());
                    widget->treeView->selectionModel()->select(childitem->index(), QItemSelectionModel::Select);
                }
            }
           // widget->treeView->model()->index(item->row(), item->index());
        }
    }
}
