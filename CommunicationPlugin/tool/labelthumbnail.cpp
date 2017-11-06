#include "labelthumbnail.h"
#include <QPainter>
#include <QDebug>
LabelThumbnail::LabelThumbnail(QWidget *parent):QLabel(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);

    //this->setMaximumSize(200, 300);
}

LabelThumbnail::~LabelThumbnail()
{
    qDebug()<<"delete LabelThumbnail";
}

void LabelThumbnail::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    //qDebug()<<this->pixmap()->rect();
    QPixmap pixmap=*this->pixmap();
    painter.drawPixmap(QPoint(0, 0), pixmap);
    QBrush brush(QColor(0, 0, 0, 100));
    painter.setBrush(brush);
    painter.drawRect(pixmap.rect());

}
