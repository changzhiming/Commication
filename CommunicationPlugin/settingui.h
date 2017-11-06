#ifndef SETTINGUI_H
#define SETTINGUI_H

#include <QWidget>

namespace Ui {
class SettingUi;
}

class SettingUi : public QWidget
{
    Q_OBJECT

public:
    explicit SettingUi(QWidget *parent = 0);
    ~SettingUi();

Q_SIGNALS:
    void sysTryIconchanged(bool changed);
private slots:

    void onTranslationUpdated();
    void onAutorunUpdated();
    void onSetShowSystemTray();
    void onCheckUpdateChanged();
    void onAutoAwayChanged();
    void onAutoAcceptFileChange();
    void onAutoSaveDirChange();         //自动保存文件目录
    void onSetNotifySound();
    void onSetBusySound();
    void onSaveChatLog();

    void onSetStatusChange();     //联系人装太改变
    void onEmoticonSizeChanged();
    void onStyleSelected(QString style);
    void onTimestampSelected(int index);
    void onDateFormatSelected(int index);
    void onUseEmoticonsChange();
    void onSmileyBrowserIndexChanged(int index);
    void onSetGroupAlwaysNotify();

    void onSeparateWindowChanged();
    void onThemeColorChanged(int);
    //sip

    void onSipServerComplete();
    void onSipAgencyComplete();
    void onSipHostNameComplete();
    void onSipLoginName();
    void onSipUserName();
    void onSipPasswordComplete();
    void onSipEncryptionChanged();
    //音频
    void onVoiceDeviceChanged(int index);
    void onVoiceSpliteChanged(int value);

private:
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::SettingUi *ui;
public:
    bool translate();
};

#endif // SETTINGUI_H
