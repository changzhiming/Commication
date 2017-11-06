#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include "friendlist.h"
#include "wcontainwidget.h"
#include "savesetting.h"
#include "listFriend/my_listmodel.h"
#include <QCompleter>
#include <QProcess>

login::login(QString  showMess, QWidget *parent) :
    NoTitle(parent),
    ui(new Ui::login)
{

    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);

    qRegisterMetaType<XmppClient::Error>("XmppClient::Error");           //注册结构 不同线程之间
    qRegisterMetaType<QXmppClient::State>("QXmppClient::State");
    qRegisterMetaType<QXmppMessage>("QXmppMessage");
    qRegisterMetaType<QXmppPresence>("QXmppPresence");
    qRegisterMetaType<QXmppMucItem>("QXmppMucItem");
    qRegisterMetaType<QList<QXmppMucItem>>("QList<QXmppMucItem>");
    qRegisterMetaType<QXmppMucItem::Affiliation>("QXmppMucItem::Affiliation");
    qRegisterMetaType<QXmppMucRoom::Actions>("QXmppMucRoom::Actions");
    qRegisterMetaType<XmppClient::NoticeBase>("XmppClient::NoticeBase");

    containWidget=nullptr;

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);

    if(!showMess.isEmpty())
        ui->loginerror->setText(QObject::tr("断开连接"));


    this->setStyleSheet(Core::getInstance()->getStylesheet(QStringLiteral(":/qss/login.qss")));
    QStringList wordList;           //设置自动补全词条
    for(int i=0; i<SaveSetting::getInstance().loginList.size(); i++)
        wordList.append(SaveSetting::getInstance().loginList.at(i).userName);
    completer=new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit->setCompleter(completer);

    connect(ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(savePassword(int)));
    ui->checkBox->setChecked(SaveSetting::getInstance().getPassword());
    ui->loginerror->setAlignment(Qt::AlignCenter);
    ui->lineEditServer->setText(SaveSetting::getInstance().getXmppServerIp());
    //安装事件过滤器
    ui->lineEdit->installEventFilter(this);
    ui->lineEditServer->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->labelRegister->installEventFilter(this);
    ui->pushButton->installEventFilter(this);


    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onPushButtonClicked()));
    connect(ui->lineEdit_2, SIGNAL(returnPressed()), this, SLOT(onPushButtonClicked()));       //回车自动登陆
    connect(ui->lineEditServer, SIGNAL(returnPressed()), this, SLOT(onPushButtonClicked()));

}

login::~login()
{
    qDebug()<<"delete login";


    delete Core::getInstance();

    if(containWidget){
        delete containWidget;
    }
    delete ui;
    SaveSetting::getInstance().saveGlobal();  //保存全局信息
    delete &SaveSetting::getInstance();
}

void login::on_pushButton_2_clicked()  //推出按钮
{
    this->close();
}

void login::onPushButtonClicked()    //登陆按钮
{
    static int once=1;
    if(once--)
    {
        connect(Core::getInstance()->xmppClient, SIGNAL(logined()), this, SLOT(logInSuccess1()));
        connect(Core::getInstance()->xmppClient, SIGNAL(loginErrortoUI(XmppClient::Error)), this, SLOT(loginError(XmppClient::Error)));
        connect(Core::getInstance()->xmppClient->client, SIGNAL(stateChanged(QXmppClient::State)), this, SLOT(disconnectt(QXmppClient::State)));
    }
    Core::getInstance()->xmppClient->ServerAddress="@"+ui->lineEditServer->text();
    Core::getInstance()->xmppClient->loginWin(ui->lineEdit->text(), ui->lineEdit_2->text());
    ui->loginerror->setText(tr("正在登陆"));
}
void login::logInSuccess1()
{
    if(!containWidget){
        containWidget=new WContainWidget();
        containWidget->show();
    }
    else{
        return;
    }
    close();
    SaveSetting::getInstance().setXmppServerIp(ui->lineEditServer->text());
    for(int i=0; i<SaveSetting::getInstance().loginList.size(); i++)
    {
        if(!SaveSetting::getInstance().loginList.at(i).userName.compare(ui->lineEdit->text(), Qt::CaseInsensitive))
            return;
    }
    SaveSetting::Login login={ui->lineEdit->text(), ui->lineEdit_2->text()};
    SaveSetting::getInstance().loginList.append(login);           //保存登陆用户

    SaveSetting::getInstance().saveGlobal();
}

void login::completerActivat(QString text)
{

    for(int i=0; i<SaveSetting::getInstance().loginList.size(); i++)
    {
        if(!SaveSetting::getInstance().loginList.at(i).userName.compare(text))
        {
            ui->lineEdit_2->setText(SaveSetting::getInstance().loginList.at(i).password);
            ui->lineEdit_2->setFocus();
            return;
        }
    }
}
void login::savePassword(int i)
{
    connect(completer, SIGNAL(activated(QString)), this, SLOT(completerActivat(QString)));
    qDebug()<<ui->checkBox->isCheckable()<<ui->checkBox->isChecked()<<"123"<<i;
    SaveSetting::getInstance().setPassword(ui->checkBox->isChecked());
    SaveSetting::getInstance().saveGlobal();

}

void login::loginError(XmppClient::Error error)
{
    ui->loginerror->setText("登陆失败");
}
void login::disconnectt(QXmppClient::State status)               //当前的连接状态改变
{
    qDebug()<<"login QXmppClient::State"<<status;
    static int once=1;
    if(status==QXmppClient::DisconnectedState&&FriendList::friendlist&&once--){
        qDebug()<<"断开连接";
        qDebug()<<disconnect(Core::getInstance()->xmppClient->client, SIGNAL(stateChanged(QXmppClient::State)), this, SLOT(disconnectt(QXmppClient::State)));

        QStringList list;
        list.append(tr("abc"));
        QProcess::startDetached(qApp->applicationFilePath(), list);
        containWidget->saveAndQuit();
    }
}

bool login::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj==ui->lineEdit){
        if(ev->type()==QEvent::FocusIn){
            ui->toolButton->setIcon(QIcon(tr(":/login/image/login/usernom.png")));
            ui->toolButton->setStyleSheet("QToolButton{	background-color: #1f679f;}");
        }
        if(ev->type()==QEvent::FocusOut){
            ui->toolButton->setIcon(QIcon(tr(":/login/image/login/usermove.png")));
            ui->toolButton->setStyleSheet("QToolButton{	background-color: #dcddde;}");
        }
    }
    if(obj==ui->lineEdit_2){
        if(ev->type()==QEvent::FocusIn){
            ui->toolButton_2->setIcon(QIcon(tr(":/login/image/login/locknom.png")));
            ui->toolButton_2->setStyleSheet("QToolButton{background-color: #1f679f;}");
        }
        if(ev->type()==QEvent::FocusOut){
            ui->toolButton_2->setIcon(QIcon(tr(":/login/image/login/lockmove.png")));
            ui->toolButton_2->setStyleSheet("QToolButton{	background-color: #dcddde;}");
        }
    }
    if(obj==ui->lineEditServer){
        if(ev->type()==QEvent::FocusIn){
            ui->toolButtonServer->setIcon(QIcon(tr(":/login/image/login/ipnom.png")));
            ui->toolButtonServer->setStyleSheet("QToolButton{	background-color: #1f679f;}");
        }
        if(ev->type()==QEvent::FocusOut){
            ui->toolButtonServer->setIcon(QIcon(tr(":/login/image/login/ipmove.png")));
            ui->toolButtonServer->setStyleSheet("QToolButton{	background-color: #dcddde;}");
        }
    }
    if(obj==ui->labelRegister){
        if(ev->type()==QEvent::Enter)
            this->setCursor(Qt::PointingHandCursor);
        if(ev->type()==QEvent::Leave)
            this->setCursor(Qt::ArrowCursor);
    }
    return QWidget::eventFilter(obj, ev);
}

void login::on_toolButtonClose_clicked()
{
    this->close();
}


void login::on_toolButtonMin_clicked()
{
    this->showMinimized();
}
