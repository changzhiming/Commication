#include "notitle.h"
#include <QMouseEvent>
#include <QDebug>
#include <qmath.h>
#ifdef Q_OS_WIN32
#ifdef __cplusplus   //是否在cpp文件中
extern "C"{
#include <windowsx.h>
#include <qt_windows.h>
}
#endif
#endif

NoTitle::NoTitle(QWidget *parent):QWidget(parent)
{

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    //setAttribute(Qt::WA_TranslucentBackground, true);
}

NoTitle::~NoTitle()
{

}
void NoTitle::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons()&Qt::LeftButton)
    {
        if(press&&(cursor().shape()==Qt::ArrowCursor))
        {
            move(ev->globalPos()-startPosition);
            ev->accept();
        }
    }
    else
    {
        ev->ignore();
    }
}

void NoTitle::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        startPosition=ev->globalPos()-geometry().topLeft();
        press=true;
        ev->accept();
    }
    else
    {
        ev->ignore();
    }

}
void NoTitle::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        press=false;
        ev->accept();
    }
    else
    {
        ev->ignore();
    }
}
#ifdef Q_OS_WIN32
bool NoTitle::nativeEvent(const QByteArray &eventType, void *message, long *result)
{

    int m_nBorder=5;
    bool trigger=false;
    MSG *param=static_cast<MSG *>(message);
    switch(param->message)
    {
    case WM_NCHITTEST:
    {
        int nX=GET_X_LPARAM(param->lParam)-this->frameGeometry().x();
        int nY=GET_Y_LPARAM(param->lParam)-this->frameGeometry().y();

        if((nX>0)&&(nX<m_nBorder))
        {
            *result=HTLEFT;
            trigger=true;
        }
        if((nX>this->width()-m_nBorder)&&(nX<this->width()))
        {
            *result=HTRIGHT;
            trigger=true;
        }
        if((nY>0)&&(nY<m_nBorder))
        {

            *result=HTTOP;
            trigger=true;
        }
        if((nY>this->height()-m_nBorder)&&(nY<this->height()))
        {

            *result=HTBOTTOM;
            trigger=true;
        }
        if((nX>0)&&(nX<m_nBorder)&&(nY>0)&&(nY<m_nBorder))
        {

            *result=HTTOPLEFT;
            trigger=true;
        }
        if((nX>this->width()-m_nBorder)&&(nX<this->width())&&(nY>0)&&(nY<m_nBorder))
        {

            *result=HTTOPRIGHT;
            trigger=true;
        }
        if((nX>0)&&(nX<m_nBorder)&&(nY>this->height()-m_nBorder)&&(nY<this->height()))
        {
            *result=HTBOTTOMLEFT;
            trigger=true;
        }
        if((nX>this->width()-m_nBorder)&&(nX<this->width())&&(nY>this->height()-m_nBorder)&&(nY<this->height()))
        {

            *result=HTBOTTOMRIGHT;
            trigger=true;
        }
        if(trigger)
        {
            return true;
        }
        return QWidget::nativeEvent(eventType, message, result);;
    }
    }
    return QWidget::nativeEvent(eventType, message, result);
}
#endif
