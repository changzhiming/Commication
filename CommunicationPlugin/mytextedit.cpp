#include "mytextedit.h"
#include <QMimeData>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTextFrame>
#include "savesetting.h"
#include <QApplication>
MyTextEdit::MyTextEdit(QWidget *parent):QTextEdit(parent)
{
    setAcceptRichText(true);
}

MyTextEdit::~MyTextEdit()
{

}

bool MyTextEdit::canInsertFromMimeData( const QMimeData *source ) const
{
    if (source->hasImage())
        return true;
    else
        return QTextEdit::canInsertFromMimeData(source);
}

void MyTextEdit::insertFromMimeData( const QMimeData *source )
{

    if (source->hasImage()){
        qDebug()<<"insertFormMimeData";
        static int temp_picture_number=SaveSetting::getInstance().getTempPictureNum();
        QImage image= qvariant_cast<QImage>(source->imageData());

        QDir dir(qApp->applicationDirPath()+"/temp_picture");
        if(!dir.exists()) {
            dir.mkpath(dir.absolutePath());
        }

        qDebug()<<image.save(QString("temp_picture/mypic%1.png").arg(temp_picture_number));
        int width=400, heigth=200;
        if(image.size().width()<width){
            width=image.size().width();
        }

        if(image.size().height()<heigth){
            heigth=image.size().height();
        }
        QDir dirq("temp_picture");
        qDebug()<<dirq.absolutePath();
        this->append(QString("<img width=%3 height=%2 src=temp_picture/mypic%1.png>").arg(temp_picture_number).arg(heigth).arg(width));
        temp_picture_number++;
        SaveSetting::getInstance().setTempPictureNum(temp_picture_number);
    }
    QTextEdit::insertFromMimeData(source);
}
void MyTextEdit::keyPressEvent(QKeyEvent *event)   //设置回车发送消息
{

    if(event->key()==Qt::Key_Return){
        emit return_sendMessage();
        return;
    }
    QTextEdit::keyPressEvent(event);
}
