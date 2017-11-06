#ifndef WCONTAINWIDGET_H
#define WCONTAINWIDGET_H
#include "tool/notitle.h"
#include <QSystemTrayIcon>

namespace Ui {
class WContainWidget;
}
class WContainWidget : public NoTitle
{
    Q_OBJECT

public:
    static WContainWidget* MainWidget;
    explicit WContainWidget(QWidget *parent = 0);
    ~WContainWidget();
    void initWindow(QLayout *layout, QWidget *win, QString s, bool show_hide);
    void classInit();

public Q_SLOTS:
    void showWindow(QWidget *widget);
    void saveAndQuit();         //推出应用程序
private Q_SLOTS:

    void trayIconActivated(QSystemTrayIcon::ActivationReason activationReason);
    void sysTryIconChanged(bool change);      //是否显示系统图标
    void acticeMainWindow();

protected:
    void closeEvent(QCloseEvent *event);
private:
    void deleteHandle();          //save setting information

    Ui::WContainWidget *ui;
    QSystemTrayIcon *tryIcon;              //系统托盘
};

#endif // WCONTAINWIDGET_H
