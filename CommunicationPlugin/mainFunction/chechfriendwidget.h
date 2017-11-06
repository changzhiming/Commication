#ifndef CHECHFRIENDWIDGET_H
#define CHECHFRIENDWIDGET_H
#include <QStandardItemModel>
#include <QTableView>
#include <QTreeView>
#include <QWidget>
#include <QStandardItemModel>

class ChechFriendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChechFriendWidget(QWidget *parent = 0);
    ~ChechFriendWidget();

    void treeItemCheckAllChild(QStandardItem *item, bool check);
    void treeItemCheckChildChanged(QStandardItem *item);
    Qt::CheckState checkSibling(QStandardItem *item);

    void addItem(QStandardItem *parentItem, QStandardItem *childItem);
    QStandardItem * getRootItem();
    void clearAllChildItem(QStandardItem *parentItem);

signals:
    void itemChanget();
public slots:
    void treeItemChanged(QStandardItem *item);
    QList<QString > getAllChlidPitchName(QStandardItem *item );                      //get all name of pitch on item

public:
    QTreeView *treeView;

private:

    QStandardItemModel *treeModel;
    QStandardItem *parentItem;

};

#endif // CHECHFRIENDWIDGET_H
