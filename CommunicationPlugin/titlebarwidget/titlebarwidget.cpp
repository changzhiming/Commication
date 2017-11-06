#include "titlebarwidget.h"
#include "ui_widget.h"
#include "myitem.h"
#include <QListView>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QResizeEvent>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "myview.h"
TitleBarWidget::TitleBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleWidget)
{
    ui->setupUi(this);
    view=new MyView(this);
    ui->horizontalLayout->addWidget(view);
    this->setMaximumHeight(MyItem::itemSize.height());

}

TitleBarWidget::~TitleBarWidget()
{
    delete ui;
}

void TitleBarWidget::paintEvent(QPaintEvent *ev)
{
    QWidget::paintEvent(ev);
    QLine line;
    foreach(QGraphicsItem *item, view->scene()->selectedItems())
    {
        QRect rect=item->mapToScene(static_cast<MyItem *>(item)->boundingRect().toRect()).boundingRect().toRect();
        line.setP1(view->mapFromScene(rect.bottomLeft()));
        line.setP2(view->mapFromScene(rect.bottomRight()));
    }
    if(view->sceneRect().width()>view->scene()->items().size()*view->widthItem-32)
    {
        ui->toolButton->hide();
        ui->toolButton_2->hide();
    }
    else
    {
        ui->toolButton->show();
        ui->toolButton_2->show();
    }
}


void TitleBarWidget::on_toolButton_clicked()
{
    view->moveToleftOne();
}

void TitleBarWidget::on_toolButton_2_clicked()
{
    view->moveToRightOne();
}
void TitleBarWidget::focusToItem(ListItemData *item)
{
    view->setFocusItem(item);
}
void TitleBarWidget::focusToItem(RoomListData *item)
{
    view->setFocusItem(item);
}

void TitleBarWidget::addItem(MyItem *item)
{
    view->addMyItem(item);
    if(view->sceneRect().width()>view->scene()->items().size()*view->widthItem-32)
    {
        ui->toolButton->hide();
        ui->toolButton_2->hide();
    }
    else
    {
        ui->toolButton->show();
        ui->toolButton_2->show();
    }
}
