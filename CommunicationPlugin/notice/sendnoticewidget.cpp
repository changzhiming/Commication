#include "sendnoticewidget.h"
#include "ui_sendnoticewidget.h"
#include "savesetting.h"
#include "groupChat/invitefrienddialog.h"
#include "core/core.h"
#include "friendlist.h"
#include "listFriend/my_listmodel.h"

SendNoticeWidget::SendNoticeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendNoticeWidget)
{

    ui->setupUi(this);
    inviteDailog=new InviteFriendDialog();
    inviteDailog->setTitleScreen();

    on_fontComboBox_currentIndexChanged((SaveSetting::getInstance().getFontFamily()));

    on_fontSizecomboBox_currentIndexChanged(QString::number(SaveSetting::getInstance().getFontSize()));
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
    connect(ui->textEditPeroration, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
    connect(ui->textEditReason, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
    connect(inviteDailog, &InviteFriendDialog::sendJidList, this, &SendNoticeWidget::sendFriend);
}

SendNoticeWidget::~SendNoticeWidget()
{
    delete ui;
    if(inviteDailog)
        delete inviteDailog;
}

void SendNoticeWidget::on_fontSizecomboBox_currentIndexChanged(QString size)
{
    ui->textEdit->setFontPointSize(size.toDouble());
    ui->textEditPeroration->setFontPointSize(size.toDouble());
    ui->textEditReason->setFontPointSize(size.toDouble());
    ui->fontSizecomboBox->setCurrentText(size);
    SaveSetting::getInstance().setFontSize(size.toDouble());
    ui->textEdit->setFocus();
}

void SendNoticeWidget::on_fontComboBox_currentIndexChanged(const QString &family)
{
    ui->textEdit->setFontFamily(family);
    ui->textEditPeroration->setFontFamily(family);
    ui->textEditReason->setFontFamily(family);
    ui->fontComboBox->setCurrentText(family);
    SaveSetting::getInstance().setFontFamily(family);
    ui->textEdit->setFocus();
}
void SendNoticeWidget::cursorPositionChanged()
{
    QTextEdit *textEdit=qobject_cast<QTextEdit *>(sender());
    QTextCharFormat charFormat;
    charFormat.setFont(QFont(SaveSetting::getInstance().getFontFamily(), SaveSetting::getInstance().getFontSize()));
    if(textEdit)
        textEdit->setCurrentCharFormat(charFormat);
}
void SendNoticeWidget::sendFriend(QStringList list)
{
     friendList=list;
}

void SendNoticeWidget::on_pushButton_clicked()       //send Notice
{
    QString reason, reasonItem, Peroration;
    foreach(QString reciverId, friendList) {
        reason=Core::getInstance()->analysisHtmlContainPicture(ui->textEditReason->toHtml(), reciverId, FriendList::friendlist->myData->Id);
        reasonItem=Core::getInstance()->analysisHtmlContainPicture(ui->textEdit->toHtml(), reciverId, FriendList::friendlist->myData->Id);
        Peroration=Core::getInstance()->analysisHtmlContainPicture(ui->textEditPeroration->toHtml(), reciverId, FriendList::friendlist->myData->Id);
    }

    XmppClient::NoticeBase noticebase = {
        friendList,
        XmppClient::NoticeBase::Instructions,
        ui->checkBoxUrgency->isChecked(),
        ui->checkBoxImportant->isChecked(),
        ui->checkBoxSecret->isChecked(),
        ui->lineEdit->text(),
        ui->lineEditCall->text(),
        reason,
        reasonItem,
        Peroration,
        ui->lineEditUnit->text(),
        ui->dateTimeEdit->dateTime().date().toString(Qt::ISODate),
    };
    if(reason.isEmpty()&&reasonItem.isEmpty()&&Peroration.isEmpty()) {
        return void();
    }
    Core::getInstance()->xmppClient->sendNotice(noticebase);
    ui->lineEditCall->clear();
    friendList.clear();
}

void SendNoticeWidget::on_pushButton_4_clicked()   //接收人列表
{
    inviteDailog->show();
}

void SendNoticeWidget::on_pushButton_2_clicked()   //保存到草稿箱
{
    XmppClient::NoticeBase noticebase = {
        friendList,
        XmppClient::NoticeBase::Instructions,
        ui->checkBoxUrgency->isChecked(),
        ui->checkBoxImportant->isChecked(),
        ui->checkBoxSecret->isChecked(),
        ui->lineEdit->text(),
        ui->lineEditCall->text(),
        ui->textEditReason->toHtml(),
        ui->textEdit->toHtml(),
        ui->textEditPeroration->toHtml(),
        ui->lineEditUnit->text(),
        ui->dateTimeEdit->dateTime().date().toString(Qt::ISODate),
    };
    emit addItemToDraftNotice(noticebase);
}
void SendNoticeWidget::fillNoticeToEdit(XmppClient::NoticeBase noticebase)
{
    ui->checkBoxUrgency->setChecked(noticebase.Urgency);
    ui->checkBoxImportant->setChecked(noticebase.Important);
    ui->checkBoxSecret->setChecked(noticebase.Secret);
    ui->lineEdit->setText(noticebase.Subject);
    ui->lineEditCall->setText(noticebase.Appellation);
    ui->textEditReason->setHtml(noticebase.Reason);
    ui->textEdit->setHtml(noticebase.Thing);
    ui->textEditPeroration->setHtml(noticebase.Peroration);
    ui->lineEditUnit->setText(noticebase.Organization);
    ui->dateTimeEdit->setDate(QDate::fromString(noticebase.date, Qt::ISODate));
}
