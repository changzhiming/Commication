#include "mybasedialog.h"
#include "ui_mybasedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

MyBaseDialog::MyBaseDialog(QWidget *parent):QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);

    this->setModal(true);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    //this->setAttribute(Qt::WA_DeleteOnClose, true);   //关闭窗口自动销毁类
    QLabel *label=new QLabel(this);
    label->setText(tr("你好 hello"));
    ui->verticalLayout->addWidget(label);
    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(onPushButtonOkClicked()));
}

MyBaseDialog::~MyBaseDialog()
{

}


void MyBaseDialog::onPushButtonOkClicked()     //OK
{
    qDebug()<<"close";
    this->close();
}

void MyBaseDialog::onPushButtonCancleClicked()  //cancle
{


    this->close();
}
