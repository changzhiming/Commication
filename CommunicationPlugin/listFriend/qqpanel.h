#ifndef QQPANEL_H
#define QQPANEL_H
#include <QToolBox>
#include <QWidget>
#include <QInputDialog>
#include <QGridLayout>
#include <listFriend/my_listmodel.h>
#include <QMenu>
class MyListView;
//自定义显示好友列表窗体
class QQPanel : public QWidget
{
    Q_OBJECT
public:
    explicit QQPanel(QWidget *parent = 0);
    ~QQPanel();
    void addFriend(ListItemData *pItem, QString name);  //增加好友  参数好友数据结构体
    void addGroup(QString s);  //增加组参数组名
    void setCurrentFriendCursor(QString username);
    void clearAllWidgetCursor();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
protected slots:
    void addGroupSlot(bool addgroup);
public:
    QMap<QString, MyListView *> *m_pListMap;
private:
    friend class  FriendList;
    QToolBox *m_pBox;
    QGridLayout *gridlayout;

};

#endif // QQPANEL_H
