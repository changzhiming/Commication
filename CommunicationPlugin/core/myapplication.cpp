#include "myapplication.h"
#include <QFile>
MyApplication::MyApplication(int argc, char *argv[]):QApplication(argc, argv)
{
    isrunning=false;
    QCoreApplication::setApplicationName(tr("统一通信终端"));
    QString serverName=QCoreApplication::applicationName();
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if(socket.waitForConnected(500))
    {
        QTextStream strem(&socket);
        QStringList args=QCoreApplication::arguments();

        if(args.count()>1)
        {
            strem<<args.last();
        }
        else
        {
            strem<<QString();
        }
        strem.flush();
        socket.waitForBytesWritten();
        isrunning=true;
        return;
    }
    server=new QLocalServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));
    if(server->listen(serverName))
    {
        if(server->serverError()==QAbstractSocket::AddressInUseError&&QFile::exists(server->serverName()))
        {
            QFile::remove(server->serverName());
            server->listen(serverName);
        }
    }
}

MyApplication::~MyApplication()
{

}

void MyApplication::newLocalConnection()
{
    QLocalSocket *socket=server->nextPendingConnection();
    if(!socket)
        return;
    socket->waitForReadyRead(1000);
    delete socket;
    qDebug()<<"wwwwwwwwwwwwwwwwwwwwww";
    //emit mainWindowsShow();
}

bool MyApplication::isRunning()
{
    return isrunning;
}
