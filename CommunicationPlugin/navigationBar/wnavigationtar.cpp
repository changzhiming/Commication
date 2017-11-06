#include "wnavigationtar.h"
#include <QObject>
#include <QPointer>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include "core/core.h"

class WNavigationTarPrivate
{
public:
    WNavigationTarPrivate(WNavigationTar *qq);

    QColor currentItemColor;
    QSize iconSize;
    int itemSpacing;

    QToolButton *pressToolButton;
    QHash<QToolButton *, QWidget *> mapIconToWidget;
    QVBoxLayout *vboxLayout;

    WNavigationTar *q_ptr;
    Q_DECLARE_PUBLIC(WNavigationTar)
};

WNavigationTarPrivate::WNavigationTarPrivate(WNavigationTar *qq)
    :currentItemColor(QColor(Qt::white)), iconSize(QSize(25, 25)), itemSpacing(14),pressToolButton(nullptr) , vboxLayout(nullptr) ,q_ptr(qq)
{
}

WNavigationTar::WNavigationTar(QWidget *parent)
    : QWidget(parent) ,d_ptr(new WNavigationTarPrivate(this))
{
    setAttribute(Qt::WA_StyledBackground);
    d_ptr->vboxLayout=new QVBoxLayout(this);
    d_ptr->vboxLayout->setContentsMargins(0, 14, 0, 0);
    d_ptr->vboxLayout->setSpacing(d_ptr->itemSpacing);
    d_ptr->vboxLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setAcceptDrops(true);
    setStyleSheet(Core::getStylesheet(":/qss/navigation.qss"));
}

WNavigationTar::~WNavigationTar()
{
    delete d_ptr;
}
void WNavigationTar::setCurrentItemColor(QColor &color)
{
    d_ptr->currentItemColor=color;
}
QColor& WNavigationTar::currentItemColor() const
{
    return d_ptr->currentItemColor;
}

void WNavigationTar::setIconSize(QSize size)
{
    foreach(QToolButton *button, d_ptr->mapIconToWidget.keys()){
        button->setIconSize(size);
    }
    d_ptr->iconSize=size;
}
QSize &WNavigationTar::iconSize()
{
    return d_ptr->iconSize;
}

void WNavigationTar::setItemSpacing(int spacing)
{
    d_ptr->itemSpacing=spacing;
    d_ptr->vboxLayout->setSpacing(spacing);
}
int& WNavigationTar::itemSpacing()
{
    return d_ptr->itemSpacing;
}

const QToolButton *WNavigationTar::addItem(QToolButton *toolButton, QWidget *widget, QString objectname)
{
    if(!toolButton)
        return nullptr;
    toolButton->setObjectName(objectname);
    toolButton->setCheckable(true);
    toolButton->setIconSize(d_ptr->iconSize);
    toolButton->setAutoRaise(true);
    toolButton->setMinimumSize(36, 36);
    toolButton->setMaximumSize(36, 36);
    toolButton->installEventFilter(this);

    connect(toolButton, SIGNAL(clicked()), this, SLOT(recvClickButton()));
    d_ptr->mapIconToWidget.insert(toolButton, widget);
    d_ptr->vboxLayout->insertWidget(d_ptr->mapIconToWidget.size()-1, toolButton);
    return toolButton;
}
void WNavigationTar::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons()&Qt::LeftButton){
        if(d_ptr->pressToolButton){

            QToolButton *button=d_ptr->pressToolButton;
            QPixmap pixmap(button->size());
            button->render(&pixmap);

            QMimeData *mimeDta=new QMimeData;
            QByteArray itemData;
            QDataStream dataStream(&itemData, QIODevice::WriteOnly);
            dataStream<<(qint32)(d_ptr->mapIconToWidget.keys().indexOf(button));
            mimeDta->setData(QStringLiteral("itemIndex"), itemData);

            QDrag drag(this);
            drag.setMimeData(mimeDta);
            drag.setPixmap(pixmap);

            drag.setHotSpot(QPoint(button->size().width()/3, button->size().height()/3));
            drag.exec(Qt::MoveAction);
            ev->accept();
        }
    }
    else {
        QWidget::mouseMoveEvent(ev);
    }
}

void WNavigationTar::mouseReleaseEvent(QMouseEvent *ev)
{
    d_ptr->pressToolButton=nullptr;
    QWidget::mouseReleaseEvent(ev);
}

void WNavigationTar::dragEnterEvent(QDragEnterEvent *ev)
{
    if(ev->mimeData()->hasFormat(QStringLiteral("itemIndex"))){
        ev->acceptProposedAction();
    }
    else{
        ev->ignore();
    }
}
bool WNavigationTar::eventFilter(QObject *obj, QEvent *ev)
{
    QToolButton *button=qobject_cast<QToolButton *>(obj);
    if(button) {
        if(ev->type()==QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent=static_cast<QMouseEvent *>(ev);
            if(mouseEvent->button()==Qt::LeftButton){
                d_ptr->pressToolButton=button;
                ev->ignore();
            }
        }
        if(ev->type()==QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent=static_cast<QMouseEvent *>(ev);
            if(mouseEvent->button()==Qt::LeftButton){
                emit clickShowWidget(d_ptr->mapIconToWidget.value(button));
            }
        }
    }
    return QWidget::eventFilter(obj, ev);
}

void WNavigationTar::dropEvent(QDropEvent *ev)
{
    if(ev->mimeData()->hasFormat(QStringLiteral("itemIndex"))){
        QPoint globalPoint=ev->pos();
        QByteArray itemData=ev->mimeData()->data(QStringLiteral("itemIndex"));

        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        qint32 indexDrag;
        dataStream>>indexDrag;
        foreach(QToolButton *button, d_ptr->mapIconToWidget.keys())
        {
            if((globalPoint.y()-button->pos().y())<=(button->size().height()+d_ptr->itemSpacing)){
                QToolButton *childButton=d_ptr->mapIconToWidget.keys().at(indexDrag);
                d_ptr->vboxLayout->removeWidget(childButton);
                int index=d_ptr->vboxLayout->indexOf(button);
                d_ptr->vboxLayout->insertWidget(index+1, childButton);
                break;
            }
        }
        ev->setDropAction(Qt::MoveAction);
        ev->accept();
    } else {
        ev->ignore();
    }
}
void WNavigationTar::recvClickButton()
{
    QToolButton *button=qobject_cast<QToolButton *>(sender());
    if(!button)
        return;
    foreach(QToolButton *pushButton, d_ptr->mapIconToWidget.keys()) {
        if(button==pushButton) {
            pushButton->setChecked(true);
        } else {
            pushButton->setChecked(false);
        }
    }
}
