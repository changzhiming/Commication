#include "toptitlewidget.h"
#include "tagwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
TopTitleWidget::TopTitleWidget(QWidget *parent) : QWidget(parent)
{
    this->setObjectName(QStringLiteral("TopTitleWidget"));
    this->setMinimumSize(QSize(0, 36));
    this->setMaximumSize(QSize(16777215, 36));
    this->setStyleSheet(QString("QWidget#TopTitleWidget{\
                                          background-color: rgb(46, 54, 65);\
                                      }\
                                      QLabel{\
                                          font: 500 14pt \"微软雅黑\";\
                                          color: white;\
                                      }\
"));
    QHBoxLayout *horizontalLayout_6 = new QHBoxLayout(this);
    horizontalLayout_6->setSpacing(0);
    horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
    horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
    labelTitle = new QLabel(this);
    labelTitle->setObjectName(QStringLiteral("labelTitle"));
    labelTitle->setMinimumSize(QSize(171, 0));
    labelTitle->setAlignment(Qt::AlignCenter);

    horizontalLayout_6->addWidget(labelTitle);

    widgetTagBar = new TagWidget(this);
    widgetTagBar->setObjectName(QStringLiteral("widgetTagBar"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(widgetTagBar->sizePolicy().hasHeightForWidth());
    widgetTagBar->setSizePolicy(sizePolicy);

    horizontalLayout_6->addWidget(widgetTagBar);

    QVBoxLayout *verticalLayout_4 = new QVBoxLayout();
    verticalLayout_4->setSpacing(0);
    verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
    verticalLayout_4->setSizeConstraint(QLayout::SetDefaultConstraint);
    QHBoxLayout *horizontalLayout_5 = new QHBoxLayout();
    horizontalLayout_5->setSpacing(0);
    horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
    horizontalLayout_5->setSizeConstraint(QLayout::SetNoConstraint);
    QToolButton *toolButtonMin = new QToolButton(this);
    toolButtonMin->setObjectName(QStringLiteral("toolButtonMin"));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(toolButtonMin->sizePolicy().hasHeightForWidth());
    toolButtonMin->setSizePolicy(sizePolicy1);
    toolButtonMin->setAutoFillBackground(false);
    toolButtonMin->setStyleSheet(QString("QToolButton{\
                                             border-image: url(:/login/image/login/小zc.png);\
                                         }\
                                         QToolButton:hover\
                                         {\
                                             border-image: url(:/login/image/login/小move.png);\
                                         }\
                                         QToolButton:pressed{\
                                             border-image: url(:/login/image/login/小down.png);\
                                         }\
}"));
    toolButtonMin->setIconSize(QSize(28, 23));
    toolButtonMin->setAutoRaise(true);

    horizontalLayout_5->addWidget(toolButtonMin);

    /*******************************/
    QToolButton *toolButtonMax = new QToolButton(this);
    toolButtonMax->setObjectName(QStringLiteral("toolButtonMax"));
    QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(toolButtonMax->sizePolicy().hasHeightForWidth());
    toolButtonMax->setSizePolicy(sizePolicy1);
    toolButtonMax->setAutoFillBackground(false);
    toolButtonMax->setStyleSheet(QString("QToolButton{\
                                             border-image: url(:/login/image/login/da-zc.png);\
                                         }\
                                         QToolButton:hover\
                                         {\
                                             border-image: url(:/login/image/login/da-move.png);\
                                         }\
                                         QToolButton:pressed{\
                                             border-image: url(:/login/image/login/da-down.png);\
                                         }\
}"));
    toolButtonMax->setIconSize(QSize(28, 23));
    toolButtonMax->setAutoRaise(true);

    horizontalLayout_5->addWidget(toolButtonMax);




    QToolButton *toolButtonClose = new QToolButton(this);
    toolButtonClose->setObjectName(QStringLiteral("toolButtonClose"));
    QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(toolButtonClose->sizePolicy().hasHeightForWidth());
    toolButtonClose->setSizePolicy(sizePolicy2);
    toolButtonClose->setAutoFillBackground(false);
    toolButtonClose->setStyleSheet(QString("QToolButton{\
                                           border-image: url(:/login/image/login/关.png);\
                                       }\
                                       QToolButton:hover\
                                       {\
                                           border-image: url(:/login/image/login/关move.png);\
                                       }\
                                       QToolButton:pressed{\
                                           border-image: url(:/login/image/login/关down.png);\
                                       }\
}"));
    toolButtonClose->setAutoRaise(true);
    toolButtonClose->setMinimumWidth(40);

    horizontalLayout_5->addWidget(toolButtonClose);


    verticalLayout_4->addLayout(horizontalLayout_5);
    //verticalLayout_4->addStretch();

    QSpacerItem *horizontalSpacer_4 = new QSpacerItem(110, 15, QSizePolicy::Preferred, QSizePolicy::Minimum);

    verticalLayout_4->addItem(horizontalSpacer_4);

    horizontalLayout_6->addLayout(verticalLayout_4);

    connect(toolButtonMin, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(toolButtonMax, SIGNAL(clicked()), this, SLOT(showMaximized()));
    connect(toolButtonClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(widgetTagBar, SIGNAL(sendclickButton(QString)), this, SLOT(reciverClickSignal(QString)));
    widgetTagBar->addItem("查看");
    widgetTagBar->addItem("查看");
    widgetTagBar->addItem("xuanze");
    setTitle(50, "hello");


}

TopTitleWidget::~TopTitleWidget()
{

}
const QToolButton *TopTitleWidget::addItem(QString noticeName)
{
    return widgetTagBar->addItem(noticeName);
}
void TopTitleWidget::reciverClickSignal(QString clickname)
{
    emit sendclickButton(clickname);
}
void TopTitleWidget::setButtonCheked(QString noticeName)
{
    widgetTagBar->setButtonCheked(noticeName);
}
void TopTitleWidget::setTitle(int titlewidth, QString name)
{
    labelTitle->setMinimumWidth(titlewidth);
    labelTitle->setMaximumWidth(titlewidth);
    labelTitle->setText(name);
}
