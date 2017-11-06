#include "functionchoicewidget.h"
#include "ui_functionchoicewidget.h"
#include "listFriend/my_listmodel.h"
FunctionChoiceWidget::FunctionChoiceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionChoiceWidget)
{
    ui->setupUi(this);
    selectFriend=new ChechFriendWidget(this);

    ui->verticalLayout_2->addWidget(selectFriend);

}

FunctionChoiceWidget::~FunctionChoiceWidget()
{
    delete ui;
}

void FunctionChoiceWidget::initSelectFriendList(QHash<QString, ListItemData *> friendList)
{
    QHash<QString, QStandardItem *> group;               //组名和组项目
    foreach (ListItemData *data, friendList.values())
    {
        QStandardItem *item=new QStandardItem;

        if(group.contains(data->Group))
        {
            item->setText(data->Id);
            item->setIcon(QIcon(data->iconPath));
            selectFriend->addItem(group.value(data->Group),  item);
        }
        else
        {
            group.insert(data->Group, item);
            item->setText(data->Group);
            selectFriend->addItem(selectFriend->getRootItem(), item);

            QStandardItem *item=new QStandardItem;
            item->setText(data->Id);
            item->setIcon(QIcon(data->iconPath));
            selectFriend->addItem(group.value(data->Group),  item);
        }
    }
}
void FunctionChoiceWidget::clearSelectFriendList()
{
    selectFriend->clearAllChildItem(selectFriend->getRootItem());
}
