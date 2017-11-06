#ifndef ADDGROUP_H
#define ADDGROUP_H

#include <QWidget>

namespace Ui {
class AddGroup;
}

class AddGroup : public QWidget
{
    Q_OBJECT

public:
    explicit AddGroup(QWidget *parent = 0);
    ~AddGroup();
signals:
    void addRoom(const QString &roomJid,const QString &Subject,const QString &password);
protected:
    void showEvent(QShowEvent *ev);

private slots:
    void on_pushButton_clicked();

private:
    Ui::AddGroup *ui;
};

#endif // ADDGROUP_H
