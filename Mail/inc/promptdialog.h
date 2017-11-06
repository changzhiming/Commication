#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

#include <QDialog>

namespace Ui {
class PromptDialog;
}

class PromptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromptDialog(QWidget *parent = 0);
    ~PromptDialog();

    void setval(QString str);
    void seticon();
    void hidebutton();


    void setattemptname(QString str);
    QString getattemptname();

private:
    Ui::PromptDialog *ui;
    QString name;

private slots:
    void searchapplication();
};

#endif // PROMPTDIALOG_H
