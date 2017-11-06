#ifndef NEWESTMESSAGEFRAME_H
#define NEWESTMESSAGEFRAME_H

#include <QFrame>
#include "newesitemscrollarea.h"

namespace Ui {
class Frame;
}

class NewestMessageFrame : public QFrame
{
    Q_OBJECT

public:
    explicit NewestMessageFrame(QWidget *parent = 0);
    ~NewestMessageFrame();
public slots:
    void insertnewesItem(NewesItemScrollArea *item);
    void removenewsItem(NewesItemScrollArea *item);
protected:
    void dragEnterEvent(QDragEnterEvent *ev);
    void dropEvent(QDropEvent *ev);

private slots:
    void on_pushButton_clicked(bool checked);
public:
    QList<NewesItemScrollArea *>itemList;
private:
    Ui::Frame *ui;

};

#endif // NEWESTMESSAGEFRAME_H
