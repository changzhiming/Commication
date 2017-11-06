#include "newesitemscrollarea.h"
#include "ui_newesitemscrollarea.h"
#include <QPainter>
#include "mainfunctionbutton.h"
#include <QMouseEvent>
#include "friendlist.h"
NewesItemScrollArea::NewesItemScrollArea(MainFunctionButton::MessageType messagetype, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->messagetype=messagetype;
    switch(messagetype)
    {
    case MainFunctionButton::TEXT:
        ui->labelTitle->setText(tr("消息通知"));
        break;
    case MainFunctionButton::EMAIl:
        ui->labelTitle->setText("邮件通知");
        break;
    case MainFunctionButton::NOTICE:
        ui->labelTitle->setText("公告通知");
    }
    connect(this, SIGNAL(showFriend(ListItemData*)), FriendList::friendlist, SLOT(showChatWindow(ListItemData*)));
    ui->toolButton->hide();

    setMinimumHeight(200);
    setMaximumHeight(200);
}

NewesItemScrollArea::~NewesItemScrollArea()
{
    delete ui;
}
void NewesItemScrollArea::insertListItem(ListItemData *data, QString toolTip)
{

    if(this->widgetMaplistItem.values().contains(data))
    {
        QWidget *widget=this->widgetMaplistItem.key(data);
        widget->setToolTip(toolTip);
        return;
    }
    QWidget *widget=new QWidget();
    QPalette palette=widget->palette();
    palette.setColor(QPalette::Text, Qt::blue);
    widget->setPalette(palette);
    widget->setMaximumHeight(50);

    widget->setToolTip(toolTip);
    widget->setObjectName(QStringLiteral("widget"));
    QHBoxLayout *horizontalLayout = new QHBoxLayout(widget);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    QLabel *label_3 = new QLabel(widget);
    label_3->setObjectName(QStringLiteral("labelPixmap"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label_3->setSizePolicy(sizePolicy);
    label_3->setMinimumSize(40, 40);
    label_3->setMaximumSize(40, 40);

    label_3->installEventFilter(this);
    widget->installEventFilter(this);

    horizontalLayout->setContentsMargins(0, 0,0, 0);
    horizontalLayout->addWidget(label_3);


    QLabel *label_2 = new QLabel(widget);
    label_2->setObjectName(QStringLiteral("label_2"));


    sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy);
    label_2->setText(data->Name);


    horizontalLayout->addWidget(label_2);
    horizontalLayout->addSpacerItem(new QSpacerItem(40, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));


    ui->verticalLayout->insertWidget(this->widgetMaplistItem.size(), widget);
    this->widgetMaplistItem.insert(widget, data);
}

void NewesItemScrollArea::setTitle(QString name)
{
    ui->labelTitle->setText(name);
}
void NewesItemScrollArea::setCloseHide()
{
    ui->toolButton->hide();
}
void NewesItemScrollArea::setCloseShow()
{
    ui->toolButton->show();
}
void NewesItemScrollArea::removeListItemWidget(QWidget *widget)
{
    this->widgetMaplistItem.remove(widget);
    widget->findChild<QLabel *>("labelPixmap")->removeEventFilter(this);
    ui->verticalLayout->removeWidget(widget);
    widget->deleteLater();
}

bool NewesItemScrollArea::eventFilter(QObject *obj, QEvent *ev)
{

    if(qobject_cast<QWidget *>(obj))
    {
        if(ev->type()==QEvent::MouseButtonPress)
        {

            QMouseEvent *mouseevent=static_cast<QMouseEvent *>(ev);
            if(mouseevent->button()==Qt::LeftButton)
            {
                QWidget *widget=qobject_cast<QWidget *>(obj);
                if(widgetMaplistItem.contains(widget))
                {
                    qDebug()<<"showFriend";
                    emit showFriend(widgetMaplistItem.value(widget));
                    return true;
                }
            }
        }
    }
    if(qobject_cast<QLabel *>(obj))
    {
        if(ev->type()==QEvent::Paint)
        {

            QLabel *label=qobject_cast<QLabel *>(obj);
            if(label)
            {
            if(this->widgetMaplistItem.contains(label->parentWidget()))
            {
                qDebug()<<label->objectName();
                ListItemData *data= this->widgetMaplistItem.value(label->parentWidget());

                QPainter painter(label);
                QIcon icon(data->iconPath);
                QPixmap Pixmap = icon.pixmap(QSize(40, 40), QIcon::Active, QIcon::On);

                painter.drawPixmap(0, 0, Pixmap);
                painter.setBrush(Qt::red);
                painter.setPen(Qt::red);
                QRect rect=label->rect();
                rect.setWidth(10);
                rect.setHeight(10);
                rect.moveLeft(rect.left()+Pixmap.width()-10);

                painter.drawEllipse(rect);

                QFont font;
                painter.setPen(Qt::white);
                font.setPixelSize(9);
                font.setBold(true);
                painter.setFont(font);
                QString num=data->messageNumer;
                if(num.isEmpty())
                {
                    this->removeListItemWidget(label->parentWidget());
                }
                else
                {

                    if(num.toInt()>99)
                        num.setNum(99);
                    painter.drawText(rect, Qt::AlignCenter, num);
                }
            }
            }

        }
        return true;
    }
    return QWidget::eventFilter(obj, ev);
}

void NewesItemScrollArea::on_toolButton_clicked()
{
    emit removeItem(this);
}
