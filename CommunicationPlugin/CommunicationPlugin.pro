#-------------------------------------------------
#
# Project created by QtCreator 2016-12-27T16:04:46
#
#-------------------------------------------------
#include (../../fusionbase.pri)
include (xmpp/qxmpp.pri)

QT += core gui  network  svg  multimedia sql winextras
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
CONFIG += c++11 link_pkgconfig thread

TARGET = Communication
DESTDIR = $$PWD/../../bin

unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Rules for creating/updating {ts|qm}-files
include(translations/i18n.pri)
 # Build all the qm files now, to make RCC happy
system($$fromfile(translations/i18n.pri, updateallqm))

QMAKE_RESOURCE_FLAGS += -compress 9 -threshold 0
RESOURCES += \
    CommunicationPlugin.qrc \
    image1.qrc \
    qss.qrc \
    voice.qrc

HEADERS += \
    chatlog/content/filetransferwidget.h \
    chatlog/content/image.h \
    chatlog/content/notificationicon.h \
    chatlog/content/spinner.h \
    chatlog/content/text.h \
    chatlog/content/timestamp.h \
    chatlog/chatline.h \
    chatlog/chatlinecontent.h \
    chatlog/chatlinecontentproxy.h \
    chatlog/chatlog.h \
    chatlog/chatmessage.h \
    chatlog/customtextdocument.h \
    chatlog/documentcache.h \
    chatlog/pixmapcache.h \
    core/core.h \
    core/myapplication.h \
    groupChat/groupchat.h \
    groupChat/invitefrienddialog.h \
    groupChat/mybasedialog.h \
    listFriend/my_listmodel.h \
    listFriend/myitemdelegate.h \
    listFriend/mylistview.h \
    listFriend/qqpanel.h \
    listFriend/roomlistmodel.h \
    listFriend/roomlistview.h \
    listFriend/searchlistview.h \
    mainFunction/chechfriendwidget.h \
    mainFunction/functionchoicewidget.h \
    navigationBar/wnavigationtar.h \
    navigationBar/wtglobal.h \
    notice/noticecore.h \
    notice/noticeitemdelegate.h \
    notice/noticelistmodel.h \
    notice/noticelistview.h \
    notice/noticewidget.h \
    notice/seenotice.h \
    notice/sendnoticewidget.h \
    notice/tagwidget.h \
    notice/toptitlewidget.h \
    platform/autorun.h \
    platform/timer.h \
    style/smileypack.h \
    style/style.h \
    titlebarwidget/myitem.h \
    titlebarwidget/myview.h \
    titlebarwidget/titlebarwidget.h \
    tool/croppinglabel.h \
    tool/facewidget.h \
    tool/friendinformation.h \
    tool/labelthumbnail.h \
    tool/mainfunctionbutton.h \
    tool/newesitemscrollarea.h \
    tool/newestmessageframe.h \
    tool/notitle.h \
    tool/previewpicture.h \
    tool/screenshotpart.h \
    xmpp/xmppclient.h \
    addfriend.h \
    addgroup.h \
    chatwindow.h \
    friendlist.h \
    keyboard.h \
    login.h \
    mainmenustackedwidget.h \
    mytextedit.h \
    savechatlog.h \
    savesetting.h \
    settingui.h \
    wcontainwidget.h \
    sip/qsip.h \
    ../Mail/inc/widget.h
SOURCES += \
    chatlog/content/filetransferwidget.cpp \
    chatlog/content/image.cpp \
    chatlog/content/notificationicon.cpp \
    chatlog/content/spinner.cpp \
    chatlog/content/text.cpp \
    chatlog/content/timestamp.cpp \
    chatlog/chatline.cpp \
    chatlog/chatlinecontent.cpp \
    chatlog/chatlinecontentproxy.cpp \
    chatlog/chatlog.cpp \
    chatlog/chatmessage.cpp \
    chatlog/customtextdocument.cpp \
    chatlog/documentcache.cpp \
    chatlog/pixmapcache.cpp \
    core/core.cpp \
    core/myapplication.cpp \
    groupChat/groupchat.cpp \
    groupChat/invitefrienddialog.cpp \
    groupChat/mybasedialog.cpp \
    listFriend/my_listmodel.cpp \
    listFriend/myitemdelegate.cpp \
    listFriend/mylistview.cpp \
    listFriend/qqpanel.cpp \
    listFriend/roomlistmodel.cpp \
    listFriend/roomlistview.cpp \
    listFriend/searchlistview.cpp \
    mainFunction/chechfriendwidget.cpp \
    mainFunction/functionchoicewidget.cpp \
    navigationBar/wnavigationtar.cpp \
    notice/noticecore.cpp \
    notice/noticeitemdelegate.cpp \
    notice/noticelistmodel.cpp \
    notice/noticelistview.cpp \
    notice/noticewidget.cpp \
    notice/seenotice.cpp \
    notice/sendnoticewidget.cpp \
    notice/tagwidget.cpp \
    notice/toptitlewidget.cpp \
    platform/autorun_win.cpp \
    platform/timer_win.cpp \
    sip/qsip.cpp \
    style/smileypack.cpp \
    style/style.cpp \
    titlebarwidget/myitem.cpp \
    titlebarwidget/myview.cpp \
    titlebarwidget/titlebarwidget.cpp \
    tool/croppinglabel.cpp \
    tool/facewidget.cpp \
    tool/friendinformation.cpp \
    tool/labelthumbnail.cpp \
    tool/mainfunctionbutton.cpp \
    tool/newesitemscrollarea.cpp \
    tool/newestmessageframe.cpp \
    tool/notitle.cpp \
    tool/previewpicture.cpp \
    tool/screenshotpart.cpp \
    xmpp/xmppclient.cpp \
    addfriend.cpp \
    addgroup.cpp \
    chatwindow.cpp \
    friendlist.cpp \
    keyboard.cpp \
    login.cpp \
    main.cpp \
    mainmenustackedwidget.cpp \
    mytextedit.cpp \
    savechatlog.cpp \
    savesetting.cpp \
    settingui.cpp \
    wcontainwidget.cpp \
    platform/autorun_osx.cpp \
    platform/timer_osx.cpp \
    platform/autorun_xdg.cpp

FORMS += \
    chatlog/content/filetransferwidget.ui \
    groupChat/groupchat.ui \
    groupChat/invitefriendwidget.ui \
    groupChat/mybasedialog.ui \
    mainFunction/functionchoicewidget.ui \
    notice/noticewidget.ui \
    notice/seenotice.ui \
    notice/sendnoticewidget.ui \
    titlebarwidget/widget.ui \
    tool/form.ui \
    tool/friendinformation.ui \
    tool/newesitem.ui \
    tool/newesitemscrollarea.ui \
    tool/newestmessageframe.ui \
    addfriend.ui \
    addgroup.ui \
    chatwindow.ui \
    friendlist.ui \
    keyboard.ui \
    login.ui \
    mainmenustackedwidget.ui \
    settingui.ui \
    wcontainwidget.ui


    INCLUDEPATH += $$PWD/xmpp/src/base $$PWD/xmpp/src/client $$PWD/xmpp/src/server

INCLUDEPATH +=  $$PWD/sip/include/pjlib/include \
                $$PWD/sip/include/pjlib-util/include \
                $$PWD/sip/include/pjnath/include \
                $$PWD/sip/include/pjmedia/include \
                $$PWD/sip/include/pjsip/include\
                $$PWD/xmpp/src/base $$PWD/xmpp/src/client $$PWD/xmpp/src/server
LIBS += -L$$PWD/sip/lib
LIBS += -lpjsua2-x86_64-pc-mingw32 \
    -lpjsua-x86_64-pc-mingw32 \
    -lpjsip-ua-x86_64-pc-mingw32 \
    -lpjsip-simple-x86_64-pc-mingw32 \
    -lpjsip-x86_64-pc-mingw32 \
    -lpjsdp-x86_64-pc-mingw32 \
    -lpjmedia-audiodev-x86_64-pc-mingw32 \
    -lpjmedia-videodev-x86_64-pc-mingw32 \
    -lpjmedia-codec-x86_64-pc-mingw32 \
    -lpjmedia-x86_64-pc-mingw32 \
    -lpjnath-x86_64-pc-mingw32 \
    -lpjlib-util-x86_64-pc-mingw32 \
    -lpj-x86_64-pc-mingw32 \
    -lresample-x86_64-pc-mingw32 \
    -lsrtp-x86_64-pc-mingw32 \
    -lg7221codec-x86_64-pc-mingw32 \
    -lspeex-x86_64-pc-mingw32 \
    -lilbccodec-x86_64-pc-mingw32 \
    -lgsmcodec-x86_64-pc-mingw32 \
    -lm \
    -lwinmm \
    -lole32 \
    -lws2_32 \
    -lwsock32 \
    -lgdi32

DISTFILES += \
    xmpp/qxmpp.pri \

#PKGCONFIG += libpjproject
#LIBS += -lpjsua2-x86_64-unknown-linux-gnu -lpjsua-x86_64-unknown-linux-gnu -lpjsip-ua-x86_64-unknown-linux-gnu -lpjsip-simple-x86_64-unknown-linux-gnu -lpjsip-x86_64-unknown-linux-gnu -lpjmedia-codec-x86_64-unknown-linux-gnu -lpjmedia-x86_64-unknown-linux-gnu -lpjmedia-videodev-x86_64-unknown-linux-gnu -lpjmedia-audiodev-x86_64-unknown-linux-gnu -lpjmedia-x86_64-unknown-linux-gnu -lpjnath-x86_64-unknown-linux-gnu -lpjlib-util-x86_64-unknown-linux-gnu  -lsrtp-x86_64-unknown-linux-gnu -lresample-x86_64-unknown-linux-gnu -lgsmcodec-x86_64-unknown-linux-gnu -lspeex-x86_64-unknown-linux-gnu -lilbccodec-x86_64-unknown-linux-gnu -lg7221codec-x86_64-unknown-linux-gnu -lyuv-x86_64-unknown-linux-gnu  -lpj-x86_64-unknown-linux-gnu -lopus -lm  -lpthread  -lasound
CONFIG(debug, debug | release) {

    LIBS += -L$$PWD/xmpp/lib -lqxmpp_d0
    LIBS += -L$$PWD/../Mail/lib -lEmail -lMail
} else {
    LIBS += -L$$PWD/xmpp/lib -lqxmpp0
    LIBS += -L$$PWD/../Mail/lib -lEmail -lMail
}

RC_FILE += myicon.rc
