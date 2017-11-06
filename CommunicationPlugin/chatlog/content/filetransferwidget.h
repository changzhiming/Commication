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

#ifndef FILETRANSFERWIDGET_H
#define FILETRANSFERWIDGET_H

#include <QWidget>
#include <QTime>
#include "xmpp/xmppclient.h"
#include "chatlog/chatlinecontent.h"

#include <QEvent>
#include "friendlist.h"

namespace Ui {
class FileTransferWidget;
}

class QVariantAnimation;
class QPushButton;

class FileTransferWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileTransferWidget(QWidget *parent, QString filepath, bool Direction, QXmppTransferJob *Job);
    virtual ~FileTransferWidget();

    QIcon getFileIcon(QString fileName);
    QString getFilePath();
    void setJob(QXmppTransferJob *Job);

signals:
    void abort();

protected:

    QString getHumanReadableSize(qint64 size);  //获取文件大小

   virtual void paintEvent(QPaintEvent *ev) final override;

private slots:

    void updateProgess(qint64 done, qint64 total);
    void fileTransferFinish();
    void confirmExecutableOpen();

    void on_receive_clicked();
    void onCancleFileTransfer();

    void on_psuam_clicked();

    void on_toolButton_clicked();

private:
    //static QPixmap scaleCropIntoSquare(const QPixmap &source, int targetSize);

private:
    Ui::FileTransferWidget *ui;
    QTime lastTick;
    quint64 lastBytesSent = 0;

    QString filePath;                  //文件路径
    bool direction;                       //传书方向
    XmppClient *xmppClient;
    QXmppTransferJob *job;               //文件传输任务job

    bool fileopen=true;                   //文件是否接受
};

#endif // FILETRANSFERWIDGET_H
