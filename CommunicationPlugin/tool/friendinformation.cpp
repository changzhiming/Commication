#include "friendinformation.h"
#include "ui_friendinformation.h"
#include <QLineEdit>
#include <QDebug>
#include <QToolBar>

FriendInformation::FriendInformation(QWidget *parent, bool showButton) :
    QDialog(parent),
    ui(new Ui::FriendInformation)
{
    ui->setupUi(this);
    //this->setStyleSheet("background-color: darkGray"); //设置背景色
    this->setStyleSheet("QPushButton {\
                            border-style: outset;\
                            border-width: 2px;\
                            border-radius: 10px;\
                            border-color: lightGray;\
                            padding: 6px;\
                        }\
                        QPushButton:pressed {\
                            background-color: rgb(224, 0, 0);\
                            border-style: inset;\
                        }");
    this->setAttribute(Qt::WA_DeleteOnClose, true);   //关闭窗口自动销毁类
    ui->groupBox->setTitle("基本信息");
    ui->save->setToolTip("暂时不能使用");
    ui->cancle->setToolTip("暂时不能使用");
    if(!showButton)
    {
        ui->save->hide();
        ui->cancle->hide();
    }

}

FriendInformation::~FriendInformation()
{
    qDebug()<<"delete FriendInformation";
    delete ui;
}

void FriendInformation::setFriendPhoto(QString path)
{
    ui->friendphoto->setIcon(QIcon(path));
    ui->friendphoto->setAutoRaise(true);
    ui->friendphoto->setIconSize(QSize(40,40));
}
void FriendInformation::setFriendPhoto(QImage image)
{
    QIcon icon;
    icon.addPixmap(QPixmap::fromImage(image));
    ui->friendphoto->setIcon(icon);
    ui->friendphoto->setIconSize(QSize(40, 40));

}

void FriendInformation::setFriendName(QString name)
{
    ui->name->setText(name);
}

void FriendInformation::setFriendStatus(QString status)
{
    ui->status->setText(status);
}
void FriendInformation::addFriendVcardInformation(QString key, QString value)
{
    ui->formLayout->addRow(new QLabel(key, this), new QLineEdit(value, this));
}
