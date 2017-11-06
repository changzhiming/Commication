#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include <QMap>
#include <QContextMenuEvent>
#include <QListView>
#include <QAction>
#include <QMenu>
#include <QMapIterator>
#include <QModelIndex>
#include <QTimer>

class My_ListModel;
struct ListItemData;
class MyListView : public QListView
{
    Q_OBJECT
public:
    MyListView(QWidget *parent=nullptr);
    ~MyListView();
    void setListMap(QMap<QString, MyListView *>  *pListMap);
    void addItem(ListItemData *pItem);
    void show_pmenu(const QPoint &point);
    void addItemToTop(ListItemData *pItem);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void deleteItemSlot(bool bDelete);
    void moveSlot(bool bMove);
    void showFriendInformation();

public:
     My_ListModel *m_pModel;
private:
    QMenu *pMenu;
    QMap<QString, MyListView *>* m_pListMap;
    QMap<QAction *, MyListView *>* m_ActionMap;
};

#endif // MYLISTVIEW_H
