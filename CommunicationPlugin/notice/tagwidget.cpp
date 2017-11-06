#include "tagwidget.h"
#include "wtglobal.h"
#include <QPushButton>
#include <QToolButton>
#include <QMap>
#include <QHBoxLayout>
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QButtonGroup>
#include "core/core.h"

class  TagWidgetPrivate
{
public:
    Q_DECLARE_PUBLIC(TagWidget)
    TagWidgetPrivate(TagWidget *parent);
    TagWidget *q_ptr;
    QPushButton *currentButton;
    QButtonGroup *buttonGroup;
    QList<QToolButton *> noticeList;
    QHBoxLayout *hboxLayout;

};

TagWidgetPrivate::TagWidgetPrivate(TagWidget *parent):q_ptr(parent)
  ,currentButton(nullptr),buttonGroup{nullptr} , hboxLayout(nullptr)
{
}


TagWidget::TagWidget(QWidget *parent) : QWidget(parent), d_ptr(new TagWidgetPrivate(this))
{
    setAttribute(Qt::WA_StyledBackground);
    d_ptr->buttonGroup=new QButtonGroup(this);
    d_ptr->hboxLayout=new QHBoxLayout(this);
    d_ptr->hboxLayout->setContentsMargins(5, 0, 0, 0);
    d_ptr->hboxLayout->addStretch();
    setStyleSheet(Core::getStylesheet(":/qss/noticeTarget.qss"));
}

TagWidget::~TagWidget()
{
    delete d_ptr;
}
QToolButton* TagWidget::addItem(QString noticeName)
{
    QToolButton *button=nullptr;
    foreach(QToolButton *pushButton, d_ptr->noticeList) {
        if(pushButton->property("noticeLabel").toString()==noticeName) {
            button=pushButton;
            break;
        }
    }
    if(!button) {
        button=new QToolButton(this);
        button->setText(noticeName);
        button->setToolTip(noticeName);
        button->setAutoRaise(true);
        d_ptr->buttonGroup->addButton(button);

        button->setMinimumSize(100, 30);
        button->setMaximumWidth(100);
        button->setProperty("noticeLabel", noticeName);
        button->setCheckable(true);

        button->setSizePolicy(QSizePolicy::Preferred,  QSizePolicy::Preferred);

        connect(button, SIGNAL(clicked()), this, SLOT(recvClickButton()));
    qDebug()<<d_ptr->noticeList.size();
        d_ptr->hboxLayout->insertWidget(d_ptr->noticeList.size(), button);
        d_ptr->noticeList.append(button);
    }
    button->setChecked(true);

    return button;
}
void TagWidget::setButtonCheked(QString noticeName)
{
    foreach(QToolButton *pushButton, d_ptr->noticeList) {
        if(pushButton->property("noticeLabel").toString()==noticeName) {
            pushButton->setChecked(true);
            break;
        }
    }
}

void TagWidget::recvClickButton()
{
    QToolButton *button=qobject_cast<QToolButton *>(sender());
    if(!button)
        return;
    button->setChecked(true);
    QString sendName=button->property("noticeLabel").toString();
    emit sendclickButton(sendName);
}
