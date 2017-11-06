#ifndef EMAILDISPLAY_H
#define EMAILDISPLAY_H

#include <QWidget>
#include "emailmanager.h"

namespace Ui {
class EmailDisplay;
}

class EmailDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit EmailDisplay(QWidget *parent = 0);
    ~EmailDisplay();

    EmailManager manager;
    void setval(EmailManager::_top top, QString strlist);

private:
    Ui::EmailDisplay *ui;
};

#endif // EMAILDISPLAY_H
