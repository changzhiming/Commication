/*
    Copyright © 2014-2015 by The qTox Project

    This file is part of qTox, a Qt-based graphical interface for Tox.

    qTox is libre software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qTox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qTox.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "filetransferwidget.h"
#include "ui_filetransferwidget.h"

#include "style/style.h"
#include "chatwindow.h"
#include "listFriend/my_listmodel.h"
#include "savesetting.h"
#include "core/core.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QFile>
#include <QBuffer>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QPainter>
#include <QVariantAnimation>
#include <QDebug>
#include <math.h>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QProcess>
#include <QPalette>
#include <QtWinExtras/QtWin>

FileTransferWidget::FileTransferWidget(QWidget *parent, QString filepath, bool Direction, QXmppTransferJob *Job)
    : QWidget(parent),ui(new Ui::FileTransferWidget),filePath{filepath}, direction{Direction},job{Job}

{
    //qDebug()<<"FileTransferWidget construct";
    this->xmppClient=Core::getInstance()->xmppClient;        //初始化指针地址
    this->setObjectName("FileTransferWidget");
    QFrame *frame=new QFrame(this);

    QHBoxLayout *hlayout=new QHBoxLayout(this);
    ui->setupUi(frame);

    hlayout->addWidget(frame);
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0, 0, 0,0);
    setFixedHeight(40);

    this->setStyleSheet(Core::getInstance()->getStylesheet(":/qss/filetransferwidget.qss"));
    ui->continue_2->hide();
    if(job==nullptr)
    {

        ui->progressBar1->setValue(100);
        ui->cancle->hide();

        ui->psuam->hide();

        if(!filepath.isEmpty())
        {
            QFileInfo info(filepath);
            ui->filetypebutton->setIcon(getFileIcon(info.fileName()));

            ui->filetypebutton->setIconSize(QSize(32,32));
            ui->filename->setText(info.fileName());
        }
        connect(ui->filetypebutton, SIGNAL(clicked()),this, SLOT(confirmExecutableOpen()));  //连接打开文件按钮
    }
    if(job)
    {
        connect(job, SIGNAL(finished()), this, SLOT(fileTransferFinish()));
        connect(job, SIGNAL(progress(qint64,qint64)), this, SLOT(updateProgess(qint64,qint64)));
        connect(this, SIGNAL(abort()), job, SLOT(abort()));
    }
    connect(ui->cancle, SIGNAL(clicked()), this, SLOT(onCancleFileTransfer()));
    connect(ui->receive, SIGNAL(clicked()), this, SLOT(on_receive_clicked()));
    connect(ui->psuam, SIGNAL(clicked()), this, SLOT(on_psuam_clicked()));
    connect(ui->continue_2, SIGNAL(clicked()), this, SLOT(on_toolButton_clicked()));

    if(job)
    {
    if (direction)
    {

        ui->filename->setToolTip(job->fileName());

        ui->progressBar1->setValue(0);
        ui->filename->setText(job->fileName()+" "+getHumanReadableSize(job->fileSize())+" "+"0k");             //发送速度

        ui->filetypebutton->setIcon(getFileIcon(job->fileName()));
        ui->filetypebutton->setIconSize(QSize(32,32));
        ui->receive->hide();

        connect(ui->filetypebutton, SIGNAL(clicked()),this, SLOT(confirmExecutableOpen()));  //连接打开文件按钮
    }
    else
    {
        QString filename=job->fileName();

        ui->progressBar1->setValue(0);
        ui->filename->setToolTip(filename);
        ui->filename->setText(filename+" "+getHumanReadableSize(job->fileSize())+" "+"0k");             //发送速度

        ui->filetypebutton->setIcon(getFileIcon(filename));
        ui->filetypebutton->setIconSize(QSize(32,32));
        //onReceiveSendJob(xmppClient->job);       //job

        ui->psuam->hide();
    }
    }


}

FileTransferWidget::~FileTransferWidget()
{
    qDebug()<<"delete FileTransfer";
    delete ui;
}
void FileTransferWidget::setJob(QXmppTransferJob *Job)
{
    disconnect(job, SIGNAL(finished()), this, SLOT(fileTransferFinish()));
    disconnect(job, SIGNAL(progress(qint64,qint64)), this, SLOT(updateProgess(qint64,qint64)));
    disconnect(this, SIGNAL(abort()), job, SLOT(abort()));
    connect(Job, SIGNAL(finished()), this, SLOT(fileTransferFinish()));
    connect(Job, SIGNAL(progress(qint64,qint64)), this, SLOT(updateProgess(qint64,qint64)));
    connect(this, SIGNAL(abort()), Job, SLOT(abort()));
    this->job=Job;
}

QIcon FileTransferWidget::getFileIcon(QString fileName)         //win32混合编程获取文件图标
{
#ifndef Q_OS_WIN32
    QFileIconProvider iconProvider;
    QFileInfo info(fileName);
    return iconProvider.icon(info);
#else
    QIcon fileIcon=this->style()->standardIcon(QStyle::StandardPixmap(42));
    if(!fileName.isEmpty())
    {
        std::string str="file";
        int index=fileName.lastIndexOf(".");
        if(index>=0)
        {
            QString suffix=fileName.mid(index);
            str=suffix.toUtf8().constData();

        }
        LPCSTR name=str.c_str();
        SHFILEINFOA info;
        if(SHGetFileInfoA(name, FILE_ATTRIBUTE_NORMAL, &info, sizeof(info), SHGFI_SYSICONINDEX|SHGFI_USEFILEATTRIBUTES|SHGFI_ICON))
        {
            HICON icon=info.hIcon;
            fileIcon=QIcon(QtWin::fromHICON(icon));
        }
    }
    return fileIcon;
#endif
}
QString FileTransferWidget::getFilePath()
{
    return this->filePath;
}

QString FileTransferWidget::getHumanReadableSize(qint64 size)
{
    static const char* suffix[] = {"B","kiB","MiB","GiB","TiB"};
    int exp = 0;

    if (size > 0)
        exp = std::min( (int) (log(size) / log(1024)), (int) (sizeof(suffix) / sizeof(suffix[0]) - 1));

    return QString().setNum(size / pow(1024, exp), 'f', exp > 1 ? 2 : 0).append(suffix[exp]);
}


void FileTransferWidget::updateProgess(qint64 done, qint64 total)
{
    ui->progressBar1->setValue(done*100/total);
    ui->filename->setText(job->fileName()+" "+getHumanReadableSize(job->fileSize())+" "+getHumanReadableSize(job->speed()));
}
void FileTransferWidget::fileTransferFinish()
{

    if(direction)
    {
        ui->cancle->hide();
        ui->psuam->hide();

        ui->receive->show();
        if(ui->progressBar1->value()<=100)
        {
            ui->continue_2->show();
        }
    }
    else
    {
         connect(ui->filetypebutton, SIGNAL(clicked()),this, SLOT(confirmExecutableOpen()));  //连接打开文件按钮
         ui->cancle->hide();
         ui->continue_2->hide();
         ui->psuam->hide();
         ui->receive->show();
    }
}

void FileTransferWidget::confirmExecutableOpen()
{

        QFileInfo file(filePath);
        static const QStringList dangerousExtensions = { "app", "bat", "com", "cpl", "dmg", "exe", "hta", "jar", "js", "jse", "lnk", "msc", "msh", "msh1", "msh1xml", "msh2", "msh2xml", "mshxml", "msi", "msp", "pif", "ps1", "ps1xml", "ps2", "ps2xml", "psc1", "psc2", "py", "reg", "scf", "sh", "src", "vb", "vbe", "vbs", "ws", "wsc", "wsf", "wsh" };

        if (dangerousExtensions.contains(file.suffix()))
        {

            if (QMessageBox::warning(FriendList::friendlist->currentShowChatWindowData->chatwindow,"执行文件", "你要执行文件吗？", QMessageBox::Ok, QMessageBox::Cancel)!=QMessageBox::Ok)
                        return;
            // The user wants to run this file, so make it executable and run it
            QFile(file.filePath()).setPermissions(file.permissions() | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup | QFile::ExeOther);
            QProcess::startDetached(file.filePath());
        }
        else
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(file.filePath()));
        }
}


void FileTransferWidget::on_receive_clicked()
{

    if(direction||job==nullptr)
    {
        QFileInfo fi(filePath);
        QStringList list;
        list.append("/select,"+fi.absoluteFilePath().replace("/", "\\"));

        QProcess::startDetached("explorer.exe", list);
        //QDesktopServices::openUrl(QUrl::fromLocalFile(fi.absoluteFilePath()));
        return;
    }

    if(fileopen)
    {
        QString fileName;
        fileName=Core::getInstance()->xmppClient->checkreceiveconfigdata(this->job);
        if(fileName.isEmpty())
        {
            fileName=QFileDialog::getSaveFileName(NULL, tr("Save File"),job->fileName());
        }

        if(!fileName.isEmpty())
        {
            xmppClient->saveReceiveFile(fileName,job);
            filePath=fileName;
            fileopen=false;

            QRegExp regexp("(.*)(/)");
            int pos=regexp.indexIn(job->jid());

            if(pos<=-1)
            {
                return;
            }
            QString Id=regexp.cap(1);
            qDebug()<<pos<<Id;
            if(FriendList::friendlist->friendList.contains(Id))
            {
                ListItemData *data=FriendList::friendlist->friendList.value(Id);

                historyMessage *hismessage=new historyMessage{
                Id,
                data->Name,
                chatWindow::TransferFile,
                QDateTime::currentDateTime(),
                filePath};

                Core::getInstance()->insertMessageToDb(hismessage);
            }
        }
    }
    else
    {
        QFileInfo fi(filePath);
        QDesktopServices::openUrl(QUrl::fromLocalFile(fi.absoluteDir().absolutePath()));
    }

}
void FileTransferWidget::onCancleFileTransfer()
{
    emit abort();
    fileTransferFinish();
}
void FileTransferWidget::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    qreal ratio = static_cast<qreal>(geometry().height()) / static_cast<qreal>(geometry().width());
    const int r = 24;
    const int buttonFieldWidth = 34;
    const int lineWidth = 1;
    Q_UNUSED(buttonFieldWidth);
    // draw background
    //if (drawButtonAreaNeeded())
      //  painter.setClipRect(QRect(0, 0, width()-buttonFieldWidth, height()));

    painter.setBrush(QBrush(QColor(0xaa, 0xaa, 0xcf)));
    painter.drawRoundRect(geometry(), r * ratio, r);
    QWidget::paintEvent(ev);

    //painter.setBrush(QBrush(Qt::yellow));
    //painter.setClipRect(QRect(ui->filetypebutton->width() ,height()-ui->cancle->height()+lineWidth,ui->cancle->width() ,ui->cancle->height()));
    //painter.drawRoundRect(ui->cancle->geometry(), 20, 10);

}

void FileTransferWidget::on_psuam_clicked()  //暂停
{
    emit abort();
    ui->psuam->hide();
    ui->continue_2->show();
    ui->cancle->show();
    FriendList::friendlist->pauseFileTransfer.append(this);
}

void FileTransferWidget::on_toolButton_clicked()  //继续
{
    qDebug()<<job->filePath<<job->jid();
    QString jid=job->jid();
    jid=jid.left(jid.lastIndexOf("/"));

    Core::getInstance()->sendFile(job->filePath, jid, false);
    ui->psuam->show();
    ui->continue_2->hide();
}
