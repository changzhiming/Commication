#include "qqpanel.h"
#include "mylistview.h"
#include "my_listmodel.h"
#include <QListView>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QAbstractButton>

QQPanel::QQPanel(QWidget *parent) : QWidget(parent)
{
    gridlayout=new QGridLayout(this);
    m_pBox=new QToolBox(this);
    m_pListMap=new QMap<QString, MyListView *>();

    //m_pBox->setLineWidth(0);
    gridlayout->addWidget(m_pBox);
    gridlayout->setContentsMargins(0, 0, 0, 0);
    m_pBox->setContentsMargins(0,0,0,0);
    m_pBox->setFrameShape(QFrame::NoFrame);
    m_pBox->setStyleSheet("QToolBox::tab { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3); \
                              border-radius: 5px;\
                              color: block; \
                              text-align:center\
                          }\
                          \
                          QToolBox::tab:selected {\
                              font: italic;\
                              color: block;\
                          }"\
                           );


    m_pBox->layout()->setSpacing(1);  //设置每行的间距

}

QQPanel::~QQPanel()
{
    delete m_pListMap;
}

void QQPanel::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu pMenu;
    QAction *pAddGroupAct=new QAction(tr("添加分组"), &pMenu);
    pMenu.addAction(pAddGroupAct);

    connect(pAddGroupAct, SIGNAL(triggered(bool)), SLOT(addGroupSlot(bool)));
    pMenu.exec(event->globalPos());
    QWidget::contextMenuEvent(event);


}
void QQPanel::addGroupSlot(bool addgroup)
{
    qDebug()<<"addGroupslot";
    QString name=QInputDialog::getText(this, tr("输入分组名"), tr(""));
    if(!name.isEmpty())
    {
        this->addGroup(name);
    }
}

void QQPanel::addFriend(ListItemData *pItem, QString s)
{
    MyListView *pListView=m_pListMap->value(s);

    if(pListView==nullptr)
    {
        addGroup(s);
        addFriend(pItem, s);
        return;
    }
    pListView->addItemToTop(pItem);
}
void QQPanel::addGroup(QString name)
{
    {
        MyListView *pListView=new MyListView(this);
        //qDebug()<<"sedconMylistview";
        pListView->setViewMode(QListView::ListMode);
        pListView->setFlow(QListView::TopToBottom);
        pListView->setStyleSheet("QListView{icon-size:40px}");

        m_pBox->addItem(pListView, this->style()->standardIcon(QStyle::SP_ArrowRight),name);  //增加标准的图标在行上
        m_pListMap->insert(name, pListView);
    }
    for(QMap<QString, MyListView *>::iterator it=m_pListMap->begin(); it!=m_pListMap->end();it++)
    {
        MyListView *pList=it.value();
        pList->setListMap(m_pListMap);
    }

}
void QQPanel::setCurrentFriendCursor(QString username)                //设置光标到当前好友位置
{
    clearAllWidgetCursor();
    for(QMap<QString, MyListView *>::iterator it=this->m_pListMap->begin(); it!=this->m_pListMap->end();it++)
    {
        int i=0;
        MyListView *pList=it.value();
        ListItemData *data;
        while((data=pList->m_pModel->getItem(i++))!=nullptr)
        {
            if(data->Name.compare(username)==0)
            {
                m_pBox->setCurrentWidget(pList);
                pList->setCurrentIndex(pList->m_pModel->index(--i));
                return;
            }
        }
    }
}
void QQPanel::clearAllWidgetCursor()
{
    for(QMap<QString, MyListView *>::iterator it=this->m_pListMap->begin(); it!=this->m_pListMap->end();it++)
    {
        MyListView *pList=it.value();
        pList->setCurrentIndex(pList->m_pModel->index(-1));
    }
}
