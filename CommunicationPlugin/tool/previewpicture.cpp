#include "previewpicture.h"
#include <QPalette>
#include <QPainter>
#include <QDebug>
#include <QToolButton>
#include <QPushButton>
#include <QStyle>
PreviewPicture::PreviewPicture(QString picturepath):picturepath{picturepath}
{
    setWindowFlags(Qt::Window|Qt::FramelessWindowHint|Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint);
    /*QPalette palette;
    palette=this->palette();
    palette.setColor(QPalette::Window, Qt::darkGray);
    this->setPalette(palette);*/

    this->setStyleSheet("background-color: darkGray"); //设置背景色

    this->setAttribute(Qt::WA_DeleteOnClose, true);   //关闭窗口自动销毁类

    QImage image(picturepath);
    setMaximumSize(image.size().width()+margin, image.size().height()+margin);
    setMinimumSize(image.size().width()+margin, image.size().height()+margin);
    QToolButton *button=new QToolButton(this);
    button->setIcon(this->style()->standardIcon(QStyle::StandardPixmap(1)));  //设置图标为系统关闭图标
    button->setAutoRaise(true);
    button->setAutoFillBackground(true);
    //button->setStyleSheet("QPushButton{icon-size:50px}");
    connect(button, SIGNAL(clicked()), SLOT(close()));

}

PreviewPicture::~PreviewPicture()
{
    qDebug()<<"preview delecte";
}

void PreviewPicture::paintEvent(QPaintEvent *painevent)
{
    QImage image(picturepath);

    QPainter painter(this);
    painter.drawImage(margin/2, margin/2, image);

}
