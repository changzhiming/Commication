#ifndef SEARCHLISTVIEW_H
#define SEARCHLISTVIEW_H

#include <QListView>
#include <QModelIndex>
#include <QFlags>

class My_ListModel;
struct ListItemData;

class SearchListView : public QListView
{
    Q_OBJECT
public:
    enum ListType{
        Search=0,
        CurrentContact,
        GroupFriend,
    };

public:
    explicit SearchListView(QWidget *parent=nullptr);
    ~SearchListView();

    void addItem(ListItemData *pItem);
    void addItemToTop(ListItemData *pItem);
    void setListType(SearchListView::ListType type=SearchListView::Search);

protected:
    void contextMenuEvent(QContextMenuEvent *event);  //鼠标右击事件
public slots:
    bool ban();
    //加入黑名单 jid为wangyanwei@im.wfh.net
    bool kick();//踢人 jid为room234@conference.im.wfh.net/wangyanwei
    bool leave();//离开聊天室
    /*
     * 移除黑名单/
     */
    void removeFromBan();
public:
     My_ListModel *m_pModel;

private:
     SearchListView::ListType type;
};

#endif // SEARCHLISTVIEW_H
