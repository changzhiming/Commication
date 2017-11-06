#include "settingui.h"
#include "ui_settingui.h"
#include "savesetting.h"
#include "style/style.h"
#include "core/core.h"
#include "sip/qsip.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QStyleFactory>
#include <QTime>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QComboBox>
#include <QTranslator>
#include <QCoreApplication>
#include <QApplication>

static QStringList locales = {"ar",
                              "be",
                              "bg",
                              "cs",
                              "da",
                              "de",
                              "et",
                              "el",
                              "en",
                              "es",
                              "eo",
                              "fr",
                              "he",
                              "hr",
                              "it",
                              "lt",
                              "hu",
                              "nl",
                              "ja",
                              "no_nb",
                              "pl",
                              "pt",
                              "ru",
                              "sl",
                              "fi",
                              "sv",
                              "tr",
                              "uk",
                              "zh"};
static QStringList langs = {"Arabic",
                            "Беларуская",
                            "Български",
                            "Čeština",
                            "Dansk",
                            "Deutsch",
                            "Eesti",
                            "Ελληνικά",
                            "English",
                            "Español",
                            "Esperanto",
                            "Français",
                            "עברית",
                            "Hrvatski",
                            "Italiano",
                            "Lietuvių",
                            "Magyar",
                            "Nederlands",
                            "日本語",
                            "Norsk Bokmål",
                            "Polski",
                            "Português",
                            "Русский",
                            "Slovenščina",
                            "Suomi",
                            "Svenska",
                            "Türkçe",
                            "Українська",
                            "简体中文"};

static QStringList timeFormats = {"hh:mm AP", "hh:mm", "hh:mm:ss AP", "hh:mm:ss"};
// http://doc.qt.io/qt-4.8/qdate.html#fromString
static QStringList dateFormats = {"yyyy-MM-dd", "dd-MM-yyyy", "d-MM-yyyy", "dddd d-MM-yyyy", "dddd d-MM", "dddd dd MMMM"};

SettingUi::SettingUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingUi)
{
    ui->setupUi(this);
/*************************************初始化设置界面***********************************************/
    for (int i = 0; i < langs.size(); i++)
        ui->transComboBox_2->insertItem(i, langs[i]);

    ui->transComboBox_2->setCurrentIndex(locales.indexOf(SaveSetting::getInstance().getTranslation()));  //语言

    ui->cbAutorun_2->setChecked(SaveSetting::getInstance().getAutorun());  //开机自启动
    ui->checkUpdates_2->setChecked(SaveSetting::getInstance().getCheckUpdates()); //开机自动检测更新


    ui->showSystemTray_2->setChecked(SaveSetting::getInstance().getShowSystemTray()); //最小化到系统托盘
    ui->closeSystem->setChecked(!SaveSetting::getInstance().getShowSystemTray());
    QButtonGroup *showStrygroup=new QButtonGroup(this);
    showStrygroup->addButton(ui->showSystemTray_2);
    showStrygroup->addButton(ui->closeSystem);

    ui->autoAwaySpinBox_2->setValue(SaveSetting::getInstance().getAutoAwayTime());         //多久时间后自动变成离开状态
    ui->autoSaveFilesDir_2->setText(SaveSetting::getInstance().getGlobalAutoAcceptDir());   //自动接收文件目录
    ui->autoacceptFiles_2->setChecked(SaveSetting::getInstance().getAutoSaveEnabled());      //自动接收文件

    bool notifySound = SaveSetting::getInstance().getNotifySound();         //当接受到消息通知声音
    ui->notifySound_4->setChecked(notifySound);
    ui->busySound_4->setChecked(SaveSetting::getInstance().getBusySound());
    ui->busySound_4->setEnabled(notifySound);                                   //忙声音选项状态

    ui->statusChanges_4->setChecked(SaveSetting::getInstance().getStatusChangeNotificationEnabled());//显示联系人状态改变
    ui->groupAlwaysNotify_4->setChecked(SaveSetting::getInstance().getGroupAlwaysNotify()); //组聊天一直提醒
    ui->cbSeparateWindow_4->setChecked(SaveSetting::getInstance().getSeparateWindow());      //多窗口模式
    ui->useEmoticons_3->setChecked(SaveSetting::getInstance().getUseEmoticons());  //使用表情
    /*
    for (auto entry : SmileyPack::listSmileyPacks())     //加载表情
        ui->smileyPackBrowser_3->addItem(entry.first, entry.second);

    ui->smileyPackBrowser_3->setCurrentIndex(ui->smileyPackBrowser_3->findData(SaveSetting::getInstance().getSmileyPack()));   //设置表情的当前索引
    reloadSmiles();
    ui->smileyPackBrowser_3->setEnabled(ui->useEmoticons->isChecked());
    */
    ui->emoticonSize_3->setValue(SaveSetting::getInstance().getEmojiFontPointSize()); //表情大小


    //主题风格
    ui->styleBrowser_3->addItem(tr("None"));
    ui->styleBrowser_3->addItems(QStyleFactory::keys());
    if (QStyleFactory::keys().contains(SaveSetting::getInstance().getStyle()))
        ui->styleBrowser_3->setCurrentText(SaveSetting::getInstance().getStyle());
    else
        ui->styleBrowser_3->setCurrentText(tr("None"));

    for (QString color : Style::getThemeColorNames())  //主题颜色
        ui->themeColorCBox_3->addItem(color);
    ui->themeColorCBox_3->setCurrentIndex(SaveSetting::getInstance().getThemeColor());

    ////////////////**////////////////////////////////

    //时间日期格式
    QStringList timestamps;
    for (QString timestamp : timeFormats)
        timestamps << QString("%1 - %2").arg(timestamp, QTime::currentTime().toString(timestamp));

    QLocale ql;
    QStringList datestamps;
    dateFormats.append(ql.dateFormat());
    dateFormats.append(ql.dateFormat(QLocale::LongFormat));
    dateFormats.removeDuplicates();

    timeFormats.append(ql.timeFormat());
    timeFormats.append(ql.timeFormat(QLocale::LongFormat));
    timeFormats.removeDuplicates();

    for (QString datestamp : dateFormats)
        datestamps << QString("%1 - %2").arg(datestamp, QDate::currentDate().toString(datestamp));

    ui->timestamp_3->addItems(timestamps);
    ui->dateFormats_3->addItems(datestamps);
    ui->timestamp_3->setCurrentText(QString("%1 - %2").arg(SaveSetting::getInstance().getTimestampFormat(), QTime::currentTime().toString(SaveSetting::getInstance().getTimestampFormat())));
    ui->dateFormats_3->setCurrentText(QString("%1 - %2").arg(SaveSetting::getInstance().getDateFormat(), QDate::currentDate().toString(SaveSetting::getInstance().getDateFormat())));
    //sip
    ui->lineEditSipServer->setText(SaveSetting::getInstance().getSipServer());
    ui->lineEditSipAgency->setText(SaveSetting::getInstance().getSipAgency());
    ui->lineEditHostName->setText(SaveSetting::getInstance().getSipHostName());
    ui->lineEditLoginName->setText(SaveSetting::getInstance().getSipLoginName());
    ui->lineEditUserName->setText(SaveSetting::getInstance().getSipUserName());
    ui->lineEditPassword->setText(SaveSetting::getInstance().getSipPassword());
    ui->checkBoxEncryption->setChecked(SaveSetting::getInstance().getSipEncryption());
    //音频
    QList<QVariantMap> soundList;
    if(Core::getInstance()->sipPhone)
    {
        Core::getInstance()->sipPhone->getSoundDevices(soundList);
    }

    int inDev=0, outDev=0;
    if(Core::getInstance()->sipPhone)
    {
        Core::getInstance()->sipPhone->getCurrentSoundDevice(&inDev, &outDev);
    }

    foreach(QVariantMap souneMap , soundList)
    {

        int i=souneMap.value("index").toInt();
        if(souneMap.value("input_count").toInt())
        {
            if(i==inDev)
            {
                inDev=ui->inDevCombobox->count();
            }
            ui->inDevCombobox->addItem(souneMap.value("name").toString(), i);
        }
        if(souneMap.value("output_count").toInt())
        {
            if(i==outDev)
            {
                outDev=ui->outDevCombobox->count();
            }
            ui->outDevCombobox->addItem(souneMap.value("name").toString(), i);
        }
    }

    ui->inDevCombobox->setCurrentText(SaveSetting::getInstance().getInDevName());
    ui->outDevCombobox->setCurrentText(SaveSetting::getInstance().getOutDevName());
    ui->playbackSlider->setValue(SaveSetting::getInstance().getOutDevVoice());
    ui->microphoneSlider->setValue(SaveSetting::getInstance().getInDevVoice());
    onVoiceDeviceChanged(0);                //应用声音设置

    //general
    connect(ui->transComboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onTranslationUpdated()));    //语言
    connect(ui->cbAutorun_2, &QCheckBox::stateChanged, this, &SettingUi::onAutorunUpdated);                //开机自动运行
    connect(ui->checkUpdates_2, &QCheckBox::stateChanged, this, &SettingUi::onCheckUpdateChanged);       //开机自动检测更新
    connect(ui->showSystemTray_2, &QCheckBox::stateChanged, this, &SettingUi::onSetShowSystemTray);      //最小化程序到系统托盘
    connect(ui->autoAwaySpinBox_2, SIGNAL(editingFinished()), this, SLOT(onAutoAwayChanged()));             //多久后自动离开
    connect(ui->autoacceptFiles_2, &QCheckBox::stateChanged, this, &SettingUi::onAutoAcceptFileChange);      //自动接收文件
    connect(ui->autoSaveFilesDir_2, SIGNAL(clicked()), this, SLOT(onAutoSaveDirChange()));                    //自动保存文件目录
    connect(ui->notifySound_4, &QCheckBox::stateChanged, this, &SettingUi::onSetNotifySound);                //开启消息提示音
    connect(ui->busySound_4, &QCheckBox::stateChanged, this, &SettingUi::onSetBusySound);
    connect(ui->statusChanges_4, &QCheckBox::stateChanged, this, &SettingUi::onSetStatusChange);                //联系人状态该bain
    connect(ui->groupAlwaysNotify_4, &QCheckBox::stateChanged, this, &SettingUi::onSetGroupAlwaysNotify);        //总是通知组聊天消息
    connect(ui->cbSeparateWindow_4, &QCheckBox::stateChanged, this, &SettingUi::onSeparateWindowChanged);        //多窗口显示
    connect(ui->saveChatLog, &QCheckBox::stateChanged, this, &SettingUi::onSaveChatLog);                        //保存聊天记录

    //theme
    connect(ui->useEmoticons_3, &QCheckBox::stateChanged, this, &SettingUi::onUseEmoticonsChange);                   //是否使用表情
    connect(ui->smileyPackBrowser_3, SIGNAL(currentIndexChanged(int)), this, SLOT(onSmileyBrowserIndexChanged(int)));  //表情包种类
    connect(ui->styleBrowser_3, SIGNAL(currentTextChanged(QString)), this, SLOT(onStyleSelected(QString)));          //界面风格
    connect(ui->themeColorCBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(onThemeColorChanged(int)));            //主题颜色
    connect(ui->emoticonSize_3, SIGNAL(editingFinished()), this, SLOT(onEmoticonSizeChanged()));                      //表情包大小
    connect(ui->timestamp_3, SIGNAL(currentIndexChanged(int)), this, SLOT(onTimestampSelected(int)));                 //时间格式
    connect(ui->dateFormats_3, SIGNAL(currentIndexChanged(int)), this, SLOT(onDateFormatSelected(int)));              //日期格式
    //sip
    connect(ui->lineEditSipServer, SIGNAL(editingFinished()), this, SLOT(onSipServerComplete()));
    connect(ui->lineEditSipAgency, SIGNAL(editingFinished()), this, SLOT(onSipAgencyComplete()));
    connect(ui->lineEditLoginName, SIGNAL(editingFinished()), this, SLOT(onSipLoginName()));
    connect(ui->lineEditHostName, SIGNAL(editingFinished()), this, SLOT(onSipHostNameComplete()));
    connect(ui->lineEditUserName, SIGNAL(editingFinished()), this, SLOT(onSipUserName()));
    connect(ui->lineEditPassword, SIGNAL(editingFinished()), this, SLOT(onSipPasswordComplete()));
    connect(ui->checkBoxEncryption, SIGNAL(stateChanged(int)), this, SLOT(onSipEncryptionChanged()));

   //音频
    connect(ui->outDevCombobox, SIGNAL(currentIndexChanged(int)), SLOT(onVoiceDeviceChanged(int)));  //输出
    connect(ui->inDevCombobox, SIGNAL(currentIndexChanged(int)), SLOT(onVoiceDeviceChanged(int))); //输入
    connect(ui->playbackSlider, SIGNAL(valueChanged(int)), SLOT(onVoiceSpliteChanged(int)));   //输出
    connect(ui->microphoneSlider, SIGNAL(valueChanged(int)), SLOT(onVoiceSpliteChanged(int)));  //输入

    // prevent stealing mouse wheel scroll
    // scrolling event won't be transmitted to comboboxes or qspinboxes when scrolling
    // you can scroll through general SaveSetting without accidentially changing theme/skin/icons etc.
    // @see GeneralForm::eventFilter(QObject *o, QEvent *e) at the bottom of this file for more
    for (QComboBox *cb : findChildren<QComboBox*>())
    {
        cb->installEventFilter(this);
        cb->setFocusPolicy(Qt::StrongFocus);
    }

    for (QSpinBox *sp : findChildren<QSpinBox*>())
    {
        sp->installEventFilter(this);
        sp->setFocusPolicy(Qt::WheelFocus);
    }

    for (QCheckBox *cb : findChildren<QCheckBox*>()) // this one is to allow scrolling on checkboxes
    {
        cb->installEventFilter(this);
    }
}
bool SettingUi::eventFilter(QObject *o, QEvent *e)   //滚轮事件过滤去
{
    if ((e->type() == QEvent::Wheel) &&
         (qobject_cast<QComboBox*>(o) || qobject_cast<QAbstractSpinBox*>(o) || qobject_cast<QCheckBox*>(o)))
    {
        e->ignore();
        return true;
    }
    return QWidget::eventFilter(o, e);
}
SettingUi::~SettingUi()
{
    delete ui;

}
void SettingUi::onTranslationUpdated()
{
    SaveSetting::getInstance().setTranslation(locales[ui->transComboBox_2->currentIndex()]);
    translate();
}
bool SettingUi::translate()       //未写完
{
        static QTranslator *translator=new QTranslator;
        qApp->removeTranslator(translator);

        if(translator->load(":/qm/translations/zh.qm"))
        {
            qDebug()<<"load qm true";
        }
        else
        {
            qDebug()<<"load qm false";
            return true;
        }
        QApplication::installTranslator(translator);
        return true;
}

void SettingUi::onAutorunUpdated()
{
    SaveSetting::getInstance().setAutorun(ui->cbAutorun_2->isChecked());
}

void SettingUi::onCheckUpdateChanged()
{
    SaveSetting::getInstance().setCheckUpdates(ui->checkUpdates_2->isChecked());
}

void SettingUi::onSetShowSystemTray()
{
    SaveSetting::getInstance().setShowSystemTray(ui->showSystemTray_2->isChecked());
    emit sysTryIconchanged(ui->showSystemTray_2->isChecked());
}
void SettingUi::onAutoAwayChanged()
{
    int minutes = ui->autoAwaySpinBox_2->value();
    SaveSetting::getInstance().setAutoAwayTime(minutes);
}
void SettingUi::onAutoAcceptFileChange()
{
    SaveSetting::getInstance().setAutoSaveEnabled(ui->autoacceptFiles_2->isChecked());
}

void SettingUi::onAutoSaveDirChange()
{
    QString previousDir = SaveSetting::getInstance().getGlobalAutoAcceptDir();
    QString directory = QFileDialog::getExistingDirectory(0,
                                                          tr("Choose an auto accept directory", "popup title"),  //opens in home directory
                                                             QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory)
                                                          );
    if (directory.isEmpty())  // cancel was pressed
        directory = previousDir;

    SaveSetting::getInstance().setGlobalAutoAcceptDir(directory);
    ui->autoSaveFilesDir_2->setText(directory);
}
void SettingUi::onSetNotifySound()
{
    SaveSetting::getInstance().setNotifySound(ui->notifySound_4->isChecked());
}

void SettingUi::onSetBusySound()
{
    SaveSetting::getInstance().setBusySound(ui->busySound_4->isChecked());
}
void SettingUi::onSetStatusChange()
{
    SaveSetting::getInstance().setStatusChangeNotificationEnabled(ui->statusChanges_4->isChecked());
}
void SettingUi::onSetGroupAlwaysNotify()
{
    SaveSetting::getInstance().setGroupAlwaysNotify(ui->groupAlwaysNotify_4->isChecked());
}

void SettingUi::onSeparateWindowChanged()
{
    SaveSetting::getInstance().setSeparateWindow(ui->cbSeparateWindow_4->isChecked());
    //emit parent->separateWindowToggled(bodyUI->cbSeparateWindow->isChecked());
}
void SettingUi::onUseEmoticonsChange()
{
    SaveSetting::getInstance().setUseEmoticons(ui->useEmoticons_3->isChecked());
    ui->smileyPackBrowser_3->setEnabled(ui->useEmoticons_3->isChecked());
}
void SettingUi::onSmileyBrowserIndexChanged(int index)
{
    QString filename = ui->smileyPackBrowser_3->itemData(index).toString();
    SaveSetting::getInstance().setSmileyPack(filename);
    //reloadSmiles();
}
void SettingUi::onStyleSelected(QString style)
{
    qDebug()<<"onStyleSelected";
    if (ui->styleBrowser_3->currentIndex() == 0)
    {
        SaveSetting::getInstance().setStyle("None");
        qApp->setStyle(QStyleFactory::create("WindowsVista"));
    }
    else
    {
        SaveSetting::getInstance().setStyle(style);
        qApp->setStyle(QStyleFactory::create(style));
    }
}
void SettingUi::onEmoticonSizeChanged()
{
    SaveSetting::getInstance().setEmojiFontPointSize(ui->emoticonSize_3->value());
}
void SettingUi::onThemeColorChanged(int)
{
    int index = ui->themeColorCBox_3->currentIndex();
    SaveSetting::getInstance().setThemeColor(index);
    Style::setThemeColor(index);
    Style::applyTheme();
}
void SettingUi::onTimestampSelected(int index)
{
    SaveSetting::getInstance().setTimestampFormat(timeFormats.at(index));
}

void SettingUi::onDateFormatSelected(int index)
{
    SaveSetting::getInstance().setDateFormat(dateFormats.at(index));
}

void SettingUi::onSaveChatLog()
{
    SaveSetting::getInstance().setChatLog(ui->saveChatLog->isChecked());
}
void SettingUi::onSipServerComplete()
{
    if(SaveSetting::getInstance().getSipServer()==ui->lineEditSipServer->text())
    {
        return;
    }
    SaveSetting::getInstance().setSipServer(ui->lineEditSipServer->text());
    Core::getInstance()->sipPhone->RegisterSIP(SaveSetting::getInstance().getSipUserName(), SaveSetting::getInstance().getSipPassword(), SaveSetting::getInstance().getSipServer());

}

void SettingUi::onSipAgencyComplete()
{
    if(SaveSetting::getInstance().getSipAgency()==ui->lineEditSipAgency->text())
    {
        return;
    }
    SaveSetting::getInstance().setSipAgency(ui->lineEditSipAgency->text());
}

void SettingUi::onSipHostNameComplete()
{
    if(SaveSetting::getInstance().getSipHostName()==ui->lineEditPassword->text())
    {
        return;
    }
    SaveSetting::getInstance().setSipHostName(ui->lineEditHostName->text());
}

void SettingUi::onSipLoginName()
{
    if(SaveSetting::getInstance().getSipLoginName()==ui->lineEditLoginName->text())
    {
        return;
    }
    SaveSetting::getInstance().setSipLoginName(ui->lineEditLoginName->text());
}

void SettingUi::onSipUserName()
{
    if(SaveSetting::getInstance().getSipUserName()==ui->lineEditUserName->text())
    {
        return;
    }
    SaveSetting::getInstance().setSipUserName(ui->lineEditUserName->text());
    Core::getInstance()->sipPhone->RegisterSIP(SaveSetting::getInstance().getSipUserName(), SaveSetting::getInstance().getSipPassword(), SaveSetting::getInstance().getSipServer());
}

void SettingUi::onSipPasswordComplete()
{
    if(SaveSetting::getInstance().getSipPassword()==ui->lineEditPassword->text())
    {
        return;
    }
    SaveSetting::getInstance().setSipPassword(ui->lineEditPassword->text());
    Core::getInstance()->sipPhone->RegisterSIP(SaveSetting::getInstance().getSipUserName(), SaveSetting::getInstance().getSipPassword(), SaveSetting::getInstance().getSipServer());
}

void SettingUi::onSipEncryptionChanged()
{
    SaveSetting::getInstance().setSipEncrypTion(ui->checkBoxEncryption->isChecked());
}

void SettingUi::onVoiceDeviceChanged(int index)
{
    qDebug()<<"音频设备高标"<<ui->inDevCombobox->currentText()<<ui->outDevCombobox->currentText();
    if(Core::getInstance()->sipPhone)
    {
        Core::getInstance()->sipPhone->setSoundDevice(ui->inDevCombobox->currentData().toInt(), ui->outDevCombobox->currentData().toInt());
    }
    SaveSetting::getInstance().setOutDevName(ui->outDevCombobox->currentText());
    SaveSetting::getInstance().setInDevName(ui->inDevCombobox->currentText());

}
void SettingUi::onVoiceSpliteChanged(int value)
{
    qDebug()<<"音频设备高标"<<ui->playbackSlider->value()<<ui->microphoneSlider->value();
    SaveSetting::getInstance().setOutDevVoice(ui->playbackSlider->value());
    SaveSetting::getInstance().setInDevVoice(ui->microphoneSlider->value());
}
