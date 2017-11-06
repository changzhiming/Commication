#include "facewidget.h"
#include <QPushButton>
#include <QDir>
#include <QDebug>
#include <QFocusEvent>
#include <QCursor>
#include <QApplication>
FaceWidget::FaceWidget(QWidget *parent) : QMenu(parent)
{
    this->setStyleSheet("background-color: rgb(0, 0, 0);\
                        QPushButton {\
                            border-style: outset;\
                            background-color:block;\
                            padding: 5px;\
                        }\
                        QPushButton:pressed {\
                            background-color: rgb(224, 0, 0);\
                            border-style: inset;\
                        }\
                        QPushButton:hover {\
                                          background-color:darkBlue;\
                                          border-style: inset;}");

    gridLayout=new QGridLayout(this);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);
    setFacePath(qApp->applicationDirPath() + "/Basic");
    iconNum=0;
    this->setFocus();
}
void FaceWidget::setFacePath (QString dirPath)
{
    QDir dir(dirPath);
    if(!dir.exists())
    {
        qDebug()<<dirPath<<QDir::currentPath();
        return;
    }
    //qDebug()<<"sssssssss";
    QFileInfoList list=dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    if(list.count()<=0)
    {
        return;
    }
    int thisFaceCount=0;
    //qDebug()<<"sssssssss";
    for(int i=0; i<list.count(); i++)
    {

        QFileInfo fileinfo=list.at(i);
        QString suffix=fileinfo.suffix();
        if(QString::compare(suffix, QString("png"), Qt::CaseInsensitive)||QString::compare(suffix, QString("svg"), Qt::CaseInsensitive)
                ||QString::compare(suffix, QString("gif"), Qt::CaseInsensitive))
        {
            facePath.append(fileinfo.absoluteFilePath());
            //qDebug()<<"sssssssss"<<fileinfo.absoluteFilePath();
            QPushButton *button=new QPushButton(this);
            button->setObjectName(fileinfo.absoluteFilePath());
            button->setIcon(QIcon(facePath.last()));
            button->setIconSize(QSize(16,16));
            gridLayout->addWidget(button, (i)/8, (i)%8);
            connect(button, SIGNAL(clicked()), this, SLOT(recivePushbutton()));

            thisFaceCount++;
        }

    }
    iconNum+=thisFaceCount;
}
void FaceWidget::addFacePathDit (QString dirPath)
{
    QDir dir(dirPath);
    if(!dir.exists())
    {
        return;
    }
    QFileInfoList list=dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    if(list.count()<=0)
    {
        return;
    }
    int thisFaceCount=0;
    for(int i=0; i<list.count(); i++)
    {

        QFileInfo fileinfo=list.at(i);
        QString suffix=fileinfo.suffix();
        if(QString::compare(suffix, QString("png"), Qt::CaseInsensitive)||QString::compare(suffix, QString("svg"), Qt::CaseInsensitive)
                ||QString::compare(suffix, QString("gif"), Qt::CaseInsensitive))
        {
            facePath.append(fileinfo.absoluteFilePath());
            QPushButton *button=new QPushButton(this);
            button->setObjectName(fileinfo.absoluteFilePath());
            button->setIcon(QIcon(facePath.last()));
            button->setIconSize(QSize(16,16));
            gridLayout->addWidget(button, (iconNum+i)/8, (iconNum+i)%8);
            connect(button, SIGNAL(clicked()), this, SLOT(recivePushbutton()));

            thisFaceCount++;
        }

    }
    iconNum+=thisFaceCount;
}

FaceWidget::~FaceWidget()
{
    qDebug()<<"delete FaceWidget";

}

void FaceWidget::recivePushbutton()
{
    QPushButton *button=qobject_cast<QPushButton *>(sender());
    if(button)
    {
        sendFacePath(button->objectName());
    }
}
