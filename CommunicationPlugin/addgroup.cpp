#include "addgroup.h"
#include "ui_addgroup.h"
#include <QMessageBox>
#include "core/core.h"
#include "savesetting.h"

AddGroup::AddGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddGroup)
{
    ui->setupUi(this);
    connect(this, SIGNAL(addRoom(QString,QString,QString)), Core::getInstance(), SLOT(coreaddRoom(QString,QString,QString)));
}

AddGroup::~AddGroup()
{
    delete ui;
}

void AddGroup::on_pushButton_clicked()
{
    if(ui->lineEdit_3->text()!=ui->lineEdit_4->text())
    {
        QMessageBox::warning(this,tr("创建群聊"), tr("密码不一致"), QMessageBox::Ok);
        return;
    }
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,tr("创建群聊"), tr("房间名不能为空"), QMessageBox::Ok);
        return;
    }
    else
    {
        emit addRoom(ui->lineEdit->text()+"@conference."+SaveSetting::getInstance().getXmppServerIp()
                     , ui->lineEdit_2->text(), ui->lineEdit_3->text());
    }
}

void AddGroup::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
}
