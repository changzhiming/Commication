#include "screenshotpart.h"
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QDebug>
#include <QEvent>
#include <QClipboard>

ScreenShotPart::ScreenShotPart(QWidget *parent):QDialog(parent)
{
    screen = QGuiApplication::primaryScreen();
    fullScreenPix=screen->grabWindow(0);
           // QPixmap::grabWindow(QApplication::desktop()->winId());
    fullTempPix=fullScreenPix;
    this->setAttribute(Qt::WA_DeleteOnClose, true);   //关闭窗口自动销毁类
    complete=new QPushButton(this);
    complete->hide();
    complete->installEventFilter(this);
    connect(complete, SIGNAL(clicked()), this, SLOT(copytoclipboard()));

}

ScreenShotPart::~ScreenShotPart()
{
    qDebug()<<"delete SCreenShotPart";
}

void ScreenShotPart::showEvent(QShowEvent *ev)
{
    showFullScreen();
    setCursor(Qt::CrossCursor);
}
void ScreenShotPart::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.drawPixmap(0,0, fullScreenPix);
    painter.save();
    painter.setBrush(QBrush(QColor(0x00, 0x00, 0x00, 0x70)));
    painter.drawRect(0, 0, fullScreenPix.width(), fullScreenPix.height());
    painter.restore();
    if(isDrawing)
    {
        fullTempPix=fullScreenPix.copy(shotRect);
        painter.eraseRect(shotRect);
        painter.drawPixmap(startPoint, fullTempPix);
    }
    QDialog::paintEvent(ev);
}

void ScreenShotPart::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
         isDrawing=true;
         startPoint=ev->pos();
    }
}
void ScreenShotPart::mouseMoveEvent(QMouseEvent *ev)
{
    if(isDrawing)
    {
        endPoint=ev->pos();
        shotRect=QRect(startPoint, endPoint);
        repaint();
    }
}
void ScreenShotPart::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        endPoint=ev->pos();
        shotRect=QRect(startPoint, endPoint);
        repaint();
    }
    qDebug()<<fullScreenPix.rect()<<fullTempPix.rect();
    if(fullScreenPix.rect().height()-fullTempPix.rect().height()<20)
        complete->setGeometry(endPoint.x()-50, endPoint.y()-20, 50, 20);
    else
        complete->setGeometry(endPoint.x()-50, endPoint.y(), 50, 20);
    complete->setText("完成");
    complete->show();
    isDrawing=false;
}

bool ScreenShotPart::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj==complete)
    {
        if(ev->type()==QEvent::HoverEnter)
        {
            setCursor(Qt::ArrowCursor);
        }
        if(ev->type()==QEvent::HoverLeave)
        {
            setCursor(Qt::CrossCursor);
        }
    }
    return QDialog::eventFilter(obj, ev);
}
void ScreenShotPart::copytoclipboard()
{
     QClipboard* clipboard = QApplication::clipboard();
     clipboard->setImage(fullTempPix.toImage());
     close();
}
