#include "mainfunctionbutton.h"
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

MainFunctionButton::MainFunctionButton(MainFunctionButton::MessageType messagetype, QWidget *parent) : QToolButton(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);

    this->setIconSize(QSize(90, 70));
    switch(messagetype)
    {
    case MainFunctionButton::TEXT:
        this->setText(tr("消息通知"));
        this->setIcon(QIcon(":/mainBoundary/image/mainBoundary/电话会议.png"));
        break;
    case MainFunctionButton::EMAIl:
        this->setText("邮件通知");
        this->setIcon(QIcon(":/mainBoundary/image/mainBoundary/电话会议.png"));
        break;
    case MainFunctionButton::NOTICE:
        this->setText("公告通知");
        this->setIcon(QIcon(":/mainBoundary/image/mainBoundary/电话会议.png"));
    }
    this->messagtType=messagetype;
}

MainFunctionButton::~MainFunctionButton()
{

}

void MainFunctionButton::setNewsNum(int newsnum)
{
    this->newsNum=newsnum;
}

int MainFunctionButton::getNewsNum()
{
    return this->newsNum;
}
void MainFunctionButton::paintEvent(QPaintEvent *ev)
{
    QToolButton::paintEvent(ev);
    QRect rect;

    rect.setTopLeft(QPoint(this->width()-30, 0));
    rect.setSize(QSize(30, 30));

    QPainter painter(this);
    painter.setBrush(Qt::red);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(rect);
    QFont font("宋体", 15, QFont::Bold);
    painter.setPen(Qt::white);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, QString::number(getNewsNum()));
}

void MainFunctionButton::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        QMimeData *mimeDta=new QMimeData;
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream<<(qint32)MainFunctionButton::TEXT;
        mimeDta->setData(tr("MainFunctionButton"), itemData);

        QDrag drag(this);
        drag.setMimeData(mimeDta);
        drag.exec(Qt::MoveAction);
    }
}
