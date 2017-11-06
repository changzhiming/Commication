#ifndef FUNCTIONCHOICEWIDGET_H
#define FUNCTIONCHOICEWIDGET_H

#include "chechfriendwidget.h"
#include <QWidget>
struct ListItemData;
namespace Ui {
class FunctionChoiceWidget;
}

class FunctionChoiceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionChoiceWidget(QWidget *parent = 0);
    ~FunctionChoiceWidget();
    void initSelectFriendList(QHash<QString, ListItemData *> friendList);
    void clearSelectFriendList();
public:
    ChechFriendWidget * selectFriend;
private:
    Ui::FunctionChoiceWidget *ui;

};

#endif // FUNCTIONCHOICEWIDGET_H
