#ifndef NOTICEITEMDELEGATE_H
#define NOTICEITEMDELEGATE_H
#include <QItemDelegate>

class NoticeItemDelegate : public QItemDelegate
{
public:
    NoticeItemDelegate(QObject *parent = 0);
    ~NoticeItemDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // NOTICEITEMDELEGATE_H
