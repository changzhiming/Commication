#include "keyboard.h"
#include "ui_keyboard.h"
#include "sip/qsip.h"
#include "savesetting.h"
#include <core/core.h>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QObject>
#include <QTimer>
#include <QMouseEvent>

keyboard::keyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyboard)
{

    timer=nullptr;
    ui->setupUi(this);
    connectPushButton();         //连接button信号
    ui->verticalSlider->setValue(SaveSetting::getInstance().getInDevVoice());
    ui->verticalSlider_2->setValue(SaveSetting::getInstance().getOutDevVoice());
    ui->labelCallName->setAlignment(Qt::AlignCenter);                     //中间对齐
    ui->labelinDevSound->installEventFilter(this);
    ui->labelOutDevSound->installEventFilter(this);

    if(Core::getInstance()->sipPhone)         //sip是否初始化完成
    {
        timer=new QTimer(this);
        timer->setInterval(1000);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
        connect(ui->verticalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(onInSound(int)));      //输入
        connect(ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(onOutSound(int)));
    }
}

keyboard::~keyboard()
{
    delete ui;
    timer->stop();
}

void keyboard::clickPhoneKey()
{
    Core::getInstance()->playSound("qrc:/voice/ringin2.wav");
    for(int i=0; i<15; i++)
    {
        if(sender()->objectName()=="pushButton"+QString::number(i))
        {
            switch(i)
            {
            case 10:
                ui->lineEditPhone->insert(QString("*"));
                ui->lineEditPhone->setFocus();
                return;
            case 11:
                ui->lineEditPhone->insert(QString("#"));
                ui->lineEditPhone->setFocus();
                return;
            case 12:
                ui->lineEditPhone->clear();
                ui->lineEditPhone->setFocus();
                return;
            case 13:
                ui->lineEditPhone->insert(QString("+"));
                ui->lineEditPhone->setFocus();
                return;
            case 14:
                ui->lineEditPhone->backspace();
                ui->lineEditPhone->setFocus();
                return;
            }

        }
        if(sender()->objectName()=="pushButton"+QString::number(i))
        {
            ui->lineEditPhone->insert(QString::number(i));
            ui->lineEditPhone->setFocus();
            return;
        }
    }
}
void keyboard::incomingShow(const QString sipName)
{
    if(timer->isActive()) {
        return;
    }
    this->show();
    Core::getInstance()->playSound();
    if(QMessageBox::information(qobject_cast<QWidget *>(this->parent()), QObject::tr("来电"), sipName,QMessageBox::Cancel, QMessageBox::Ok)==QMessageBox::Ok)
    {
        Core::getInstance()->sipPhone->Answer();
        timer->start();                                     //开启定时器定时跟新时间
        ui->labelCallName->setText(sipName);
        ui->toolButtonDial->setText(tr("挂断"));
    }
    Core::getInstance()->stopSound();
}

void keyboard::connectPushButton()
{
    connect(ui->pushButton0, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton1, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton2, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton3, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton4, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton5, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton6, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton7, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton8, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton9, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton10, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton11, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton12, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton13, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
    connect(ui->pushButton14, SIGNAL(clicked()), this, SLOT(clickPhoneKey()));
}

void keyboard::on_toolButtonDial_clicked()       //拨打电话
{
    if(!Core::getInstance()->sipPhone)
    {
        QMessageBox::information(qobject_cast<QWidget *>(this->parent()), tr("sip电话"), tr("初始化失败"), QMessageBox::Yes);
        return ;
    }

    if(ui->toolButtonDial->text()==tr("挂断"))
    {
        Core::getInstance()->sipPhone->hangup();               //挂断电话
        timer->stop();                                      //close timer;
        ui->toolButtonDial->setText(tr("拨打"));
        ui->labelCallName->setText("");             //通话结束设置标签为空
        ui->labelTime->setText("");
        return;
    }
    if(ui->lineEditPhone->text().isEmpty())
    {
        return;
    }
    Core::getInstance()->sipPhone->MakeCall(ui->lineEditPhone->text()+"@"+SaveSetting::getInstance().getSipServer());         //拨打电话
    timer->start();               //开启更新时间
    ui->labelCallName->setText(ui->lineEditPhone->text());
    //连接父窗体信号和槽
    ui->toolButtonDial->setText(tr("挂断"));
}
void keyboard::onInSound(int i)
{

    if(Core::getInstance()->sipPhone!=nullptr)
    {
        Core::getInstance()->sipPhone->setMicroSignal(i);
    }
}

void keyboard::onOutSound(int i)
{
    if(Core::getInstance()->sipPhone!=nullptr)
    {
        Core::getInstance()->sipPhone->setSoundSignal(i);
    }
    if(Core::getInstance()->mediaPlayer!=nullptr)
    {
        Core::getInstance()->mediaPlayer->setVolume(i);
    }
}
void keyboard::updateTime()            //显示通话时间
{
    long totalTime=0, currentTime=0;
    if(Core::getInstance()->sipPhone)
    {
        Core::getInstance()->sipPhone->getDuration(totalTime, currentTime);
    }
    if(totalTime>15&&currentTime<0)
    {
        on_toolButtonDial_clicked();               //超时挂断电话
    }
    if(currentTime>0)
    {
        ui->labelTime->setText(tr("通话时间：")+QString::number(currentTime));
    }
    else
    {
        ui->labelTime->setText(tr("拨号时间：")+QString::number(totalTime));
    }
    qDebug()<<totalTime<<currentTime;
}

bool keyboard::eventFilter(QObject *obj, QEvent *ev)
{
    static bool inDevSound=false;
    static bool outDevSound=false;
    if(obj==ui->labelinDevSound)
    {
        if(ev->type()==QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent=static_cast<QMouseEvent *>(ev);
            if(mouseEvent->button()==Qt::LeftButton&&inDevSound)
            {
                  onInSound(ui->verticalSlider->value());
                  ui->labelinDevSound->setPixmap(QPixmap(tr(":/image/phone/microphone.png")));
                  inDevSound=false;
            }
            else
            {
                onInSound(0);
                ui->labelinDevSound->setPixmap(QPixmap(tr(":/image/phone/quietmicrophone.png")));
                inDevSound=true;
            }
        }
    }
    if(obj==ui->labelOutDevSound)
    {
        if(ev->type()==QEvent::MouseButtonPress)
        {

            QMouseEvent *mouseEvent= static_cast<QMouseEvent *>(ev);
            if(mouseEvent->button()==Qt::LeftButton&&outDevSound)
            {
                  onOutSound(ui->verticalSlider_2->value());
                  ui->labelOutDevSound->setPixmap(QPixmap(tr(":/image/phone/volume.png")));
                  outDevSound=false;
            }
            else
            {
                onOutSound(0);
                ui->labelOutDevSound->setPixmap(QPixmap(tr(":/image/phone/quiet.png")));
                outDevSound=true;
            }
        }
    }
    return QDialog::eventFilter(obj, ev);
}

void keyboard::showEvent(QShowEvent *ev)
{
    ui->verticalSlider->setValue(SaveSetting::getInstance().getInDevVoice());
    ui->verticalSlider_2->setValue(SaveSetting::getInstance().getOutDevVoice());
    this->show();
}
