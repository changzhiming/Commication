#ifndef ADDRECIPIENTER_H
#define ADDRECIPIENTER_H

#include <QWidget>
#include <QModelIndex>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStandardItem>
#include <QFile>

namespace Ui {
class addRecipienter;
}

class addRecipienter : public QWidget
{
    Q_OBJECT

public:
    explicit addRecipienter(QWidget *parent = 0);
    ~addRecipienter();

    void listallcontacts();

    QList<QListWidgetItem *> listw;

private:
    Ui::addRecipienter *ui;

private slots:
    void slotpushbuttonright();
    void slotpushbuttonleft();

    void slotpushbuttonok();

signals:
    void signalrecitienter(QList<QListWidgetItem*>);
};

#endif // ADDRECIPIENTER_H
