#ifndef MYITEMDELEGATE_H
#define MYITEMDELEGATE_H

#include <QStandardItem>
#include <QItemDelegate>

class MyItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    MyItemDelegate(QObject *parent=0);
    //void sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    ~MyItemDelegate();
};

#endif // MYITEMDELEGATE_H
