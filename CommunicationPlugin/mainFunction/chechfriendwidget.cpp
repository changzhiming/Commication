#include "chechfriendwidget.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QtWidgets/QHeaderView>
ChechFriendWidget::ChechFriendWidget(QWidget *parent) :QWidget(parent)
{

    QHBoxLayout *layout=new QHBoxLayout(this);
    layout->setContentsMargins(0, 9,0, 0);
    treeView =new QTreeView(this);
    treeModel=new QStandardItemModel(treeView);
    parentItem=treeModel->invisibleRootItem();

    parentItem->setEditable(false);


    treeView->setModel(treeModel);
    treeView->header()->setVisible(false);
    //treeView->setViewport(this);
    treeView->setIndentation(10);
    treeView->header()->setCascadingSectionResizes(true);

    connect(treeModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(treeItemChanged(QStandardItem*)));
    treeView->setStyleSheet("QTreeView{icon-size: 20px}");
    layout->addWidget(treeView);
}

ChechFriendWidget::~ChechFriendWidget()
{

}

void ChechFriendWidget::treeItemChanged(QStandardItem *item)
{

    if(item==nullptr)
        return;
    emit itemChanget();
    if(item->isCheckable())
    {
        Qt::CheckState state=item->checkState();
        if(item->hasChildren())
        {
            if(state!=Qt::PartiallyChecked)
            {
                treeItemCheckAllChild(item, (state==Qt::Checked)? true: false);
            }
        }
        else
        {
            treeItemCheckChildChanged(item);
        }
    }
}

void ChechFriendWidget::treeItemCheckAllChild(QStandardItem *item, bool check)
{
    if(item==nullptr)
        return;
    int rowCount=item->rowCount();
    for(int i=0; i<rowCount; i++)
    {
        QStandardItem *childItem=item->child(i);
        treeItemCheckAllChild(childItem, check);
    }
    if(item->isCheckable())
        item->setCheckState(check? Qt::Checked: Qt::Unchecked);
}
void ChechFriendWidget::treeItemCheckChildChanged(QStandardItem *item)
{
    if(item==nullptr)
        return;
    Qt::CheckState siblingState=checkSibling(item);
    QStandardItem * parentItem=item->parent();

    if(nullptr==parentItem)
        return;

    if(Qt::PartiallyChecked==siblingState)
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::PartiallyChecked);

    }
    else if(Qt::Checked==siblingState)
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Checked);
    }
    else
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Unchecked);
    }
    treeItemCheckChildChanged(parentItem);
}
Qt::CheckState ChechFriendWidget::checkSibling(QStandardItem *item)
{
    QStandardItem *parent=item->parent();
    if(nullptr==parent)
        return item->checkState();
    int brotherCount=parent->rowCount();
    int checkedCount(0), unCheckedCount(0);
    Qt::CheckState state;
    for(int i=0; i<brotherCount; ++i)
    {
        QStandardItem *siblingitem=parent->child(i);
        state=siblingitem->checkState();
        if(Qt::PartiallyChecked==state)
        {
            return Qt::PartiallyChecked;
        }
        else if(Qt::Unchecked==state)
            ++unCheckedCount;
        else
            ++checkedCount;
        if(checkedCount>0&&unCheckedCount>0)
        {
            return Qt::PartiallyChecked;
        }
    }
    if(unCheckedCount>0)
    {
        return Qt::Unchecked;
    }
    return Qt::Checked;
}
QStandardItem * ChechFriendWidget::getRootItem()
{
    return this->parentItem;
}

void ChechFriendWidget::addItem(QStandardItem *parentItem, QStandardItem *childItem)
{
    //childItem->setCheckable(true);
    childItem->setEditable(false);
    parentItem->appendRow(childItem);
}

QList<QString > ChechFriendWidget::getAllChlidPitchName(QStandardItem * item)               //recursion to find name of child
{
    QList<QString> nameList;
    if(item==nullptr)
        return nameList;
    if(item->hasChildren())
    {

        if(item->checkState()!=Qt::Unchecked||item==parentItem)
        {

            int rowCount=item->rowCount();
            for(int i=0; i<rowCount; i++)
            {
                QStandardItem *childItem=item->child(i);
                nameList.append(getAllChlidPitchName(childItem));
            }
        }

    }
    else
    {
        if(item->checkState()==Qt::Checked)
        {
            nameList.append(item->text());
        }
    }
    return nameList;
}
void ChechFriendWidget::clearAllChildItem(QStandardItem *parentItem)
{
    if(parentItem==nullptr)
        return;
    if(parentItem->hasChildren())
    {
        int rowCount=parentItem->rowCount();
        for(int i=0; i<rowCount; i++)
        {
            parentItem->removeRow(i);
        }
    }
}
