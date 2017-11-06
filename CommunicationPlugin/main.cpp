#include "login.h"
#include "chatwindow.h"
#include "keyboard.h"
#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include "listFriend/qqpanel.h"
#include "friendlist.h"
#include "listFriend/mylistview.h"
#include "savesetting.h"
#include "core/core.h"
#include "platform/autorun.h"

#include <QDebug>
#include <stdio.h>
#include <QApplication>
#include <QCommandLineParser>
#include <QtMultimedia/QAudioDeviceInfo>

#include <QStyleFactory>
#include <QToolBar>
#include <QFileDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QMovie>
#include <QMessageBox>
#include <QSound>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include "core/myapplication.h"

#include "notice/sendnoticewidget.h"
#include "notice/toptitlewidget.h"
#include "notice/noticewidget.h"
#include "notice/seenotice.h"
#include "notice/tagwidget.h"
#include "../Mail/inc/widget.h"
#include <QTextBrowser>
#include <QLabel>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{

   MyApplication a(argc, argv);
////    if(a.isRunning())

//        return -1;
    QApplication a(argc, argv);
    qSetMessagePattern("[%{time h:mm:ss.zzz }%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");

    fclose(stdout);       //屏蔽sip信息

    a.setApplicationName("统一通信终端");

    a.setOrganizationName("communication");
    a.setApplicationVersion("1.1");

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());      //设置字体编码为本地
    QThread::currentThread()->setObjectName("GUI thread");            //设置GUI线程的名字
    SaveSetting::getInstance().loadGlobal();       //加载全局
    Core::getInstance();          //初始化核心
    a.setWindowIcon(QIcon(":/icon/image/logo22.ico"));

/*********************************************************/

    QString arg;
    if(argc!=1)
        arg.append(argv[1]);

    login w(arg);
    w.move(QPoint(QApplication::desktop()->rect().width()/2 - w.width()/2, QApplication::desktop()->rect().height()/2 - w.height()/2));
    w.show();

    a.setStyle(SaveSetting::getInstance().getStyle());

    return a.exec();
}
