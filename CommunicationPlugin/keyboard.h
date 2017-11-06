#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
class simple_pjsua;
namespace Ui {
class keyboard;
}

class keyboard : public QDialog
{
    Q_OBJECT

public:
    explicit keyboard(QWidget *parent = 0);
    ~keyboard();
public:
    void connectPushButton();
protected:
    void showEvent(QShowEvent *ev);

public slots:

    void clickPhoneKey();
    void incomingShow(const QString sipName);

    void on_toolButtonDial_clicked();  //拨打电话
    void onInSound(int i);
    void onOutSound(int i);
    void updateTime();
protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private:
    Ui::keyboard *ui;
    QTimer *timer;            //定时更新时间
};

#endif // KEYBOARD_H
