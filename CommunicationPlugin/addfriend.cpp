#include "addfriend.h"
#include "ui_addfriend.h"
#include "listFriend/mylistview.h"
#include "listFriend/my_listmodel.h"
AddFriend::AddFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);

}

AddFriend::~AddFriend()
{
    delete ui;
}
