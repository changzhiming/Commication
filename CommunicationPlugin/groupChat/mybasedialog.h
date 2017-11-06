#ifndef MYBASEDIALOG_H
#define MYBASEDIALOG_H

#include <QDialog>


//邀请好友的加入界面

namespace Ui {
class Dialog;
}
class MyBaseDialog : public QDialog
{
    Q_OBJECT
public:
    MyBaseDialog(QWidget *parent=0);
    ~MyBaseDialog();
    Ui::Dialog *ui;
private slots:
    void onPushButtonOkClicked();
    void onPushButtonCancleClicked();
};

#endif // MYBASEDIALOG_H
