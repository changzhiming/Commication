#ifndef FRIENDINFORMATION_H
#define FRIENDINFORMATION_H

#include <QDialog>

namespace Ui {
class FriendInformation;
}

class FriendInformation : public QDialog
{
    Q_OBJECT

public:
    explicit FriendInformation(QWidget *parent = 0, bool showButton=false);
    ~FriendInformation();

public:
    void setFriendPhoto(QString path);
    void setFriendPhoto(QImage image);
    void setFriendName(QString name);
    void setFriendStatus(QString status);
    void addFriendVcardInformation(QString key, QString value);

private:
    Ui::FriendInformation *ui;
};

#endif // FRIENDINFORMATION_H
