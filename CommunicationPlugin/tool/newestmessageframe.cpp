#include "newestmessageframe.h"
#include "ui_newestmessageframe.h"
#include "mainfunctionbutton.h"
#include <QDropEvent>
#include <QMimeData>
NewestMessageFrame::NewestMessageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    this->setMaximumWidth(200);
}

NewestMessageFrame::~NewestMessageFrame()
{
    delete ui;
}
void NewestMessageFrame::insertnewesItem(NewesItemScrollArea *item)
{
    connect(item, &NewesItemScrollArea::removeItem, this, &NewestMessageFrame::removenewsItem);
    ui->verticalLayout->insertWidget(itemList.size()+1, item);
    itemList.append(item);
}
void NewestMessageFrame::removenewsItem(NewesItemScrollArea *item)
{
    ui->verticalLayout->removeWidget(item);
    itemList.removeOne(item);
    disconnect(item, &NewesItemScrollArea::removeItem, this, &NewestMessageFrame::removenewsItem);
    delete item;
}

void NewestMessageFrame::dragEnterEvent(QDragEnterEvent *ev)
{
    if(ev->mimeData()->hasFormat(tr("MainFunctionButton")))
    {
        ev->acceptProposedAction();
    }
    else
        {
        ev->ignore();
    }
}
void NewestMessageFrame::dropEvent(QDropEvent *ev)
{
    if(ev->mimeData()->hasFormat(tr("MainFunctionButton")))
    {

        QByteArray itemData=ev->mimeData()->data(tr("MainFunctionButton"));
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        qint32 messagetype;

        dataStream>>messagetype;

        insertnewesItem(new NewesItemScrollArea((MainFunctionButton::MessageType)messagetype));
        ev->setDropAction(Qt::MoveAction);
        ev->accept();
    }
    else
        {
        ev->ignore();
    }
}

void NewestMessageFrame::on_pushButton_clicked(bool checked)
{
    if(checked)
    {
        foreach(NewesItemScrollArea *item, itemList)
        {
            item->setCloseShow();
        }
    }
    else
    {
        foreach(NewesItemScrollArea *item, itemList)
        {
            item->setCloseHide();
        }
    }
}
