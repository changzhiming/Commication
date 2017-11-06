#include "noticelistview.h"
#include "noticelistmodel.h"
#include "noticeitemdelegate.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QDateTime>

NoticeListView::NoticeListView(QWidget *parent): QListView(parent)
{
    m_pModel=new NoticeListModel(this);
    setModel(m_pModel);

   // connect(this, SIGNAL(Dou(QModelIndex)), FriendList::friendlist, SLOT(showChatWindow(QModelIndex)));  //绑定MyListView双击和chat窗口显示
    //键盘上下选择
    this->moveCursor(QAbstractItemView::MoveNext, Qt::KeypadModifier);
    //增加代理重新绘制文字图片
    NoticeItemDelegate *myitemdelegate=new NoticeItemDelegate(this);
    setItemDelegate(myitemdelegate);
    //设置选择时的颜色
    //QPalette palette=this->palette();
    //palette.setColor(QPalette::All, QPalette::Highlight, Qt::blue);
    //setPalette(palette);
}

NoticeListView::~NoticeListView()
{

}
void NoticeListView::contextMenuEvent(QContextMenuEvent *event)  //鼠标右击事件
{
    QListView::contextMenuEvent(event);
    int index = this->indexAt(event->pos()).row();

    if(index >-1){
        QMenu pMenu;
        pMenu.addAction(tr("删除邮件"), this, SLOT(deleteItemSlot(bool)));
        pMenu.exec(event->globalPos());
    }
}

void NoticeListView::deleteItemSlot(bool bDelete)
{
    int index=this->currentIndex().row();
    if(index >-1){
        emit deleteItem(m_pModel->getItem(index));
        m_pModel->deleteLItem(index);
    }
}

void NoticeListView::addItem(NoticeListModel::noticeListItem *pItem)
{
    m_pModel->addItem(pItem);
}
void NoticeListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QListView::mouseDoubleClickEvent(event);
    int index=currentIndex().row();
    if(index >-1){
        QMenu pMenu;
        pMenu.addAction(tr("删除邮件"), this, SLOT(deleteItemSlot(bool)));
        pMenu.exec(event->globalPos());
    }
}
