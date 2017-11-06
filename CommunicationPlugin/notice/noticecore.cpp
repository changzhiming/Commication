#include "noticecore.h"
#include "savesetting.h"
#include <QFileInfo>
NoticeCore * NoticeCore::thisNoticeCore=nullptr;

NoticeCore::NoticeCore(QObject *parent) : QObject(parent)
{
    thisNoticeCore=this;
}

NoticeCore::~NoticeCore()
{

}
QString NoticeCore::noticeBaseToHtml(XmppClient::NoticeBase &notice)
{
    QString html;
    html += QString("<html><head/><body>");
    html += QString("<p align=\"center\"><span style=\" font-size:16pt; font-weight:600;\">%1</span></p>").arg(notice.Subject);
    html += ("<p><span style=\" font-size:11pt; font-weight:600;\">"+notice.Appellation+"：</span></p>");
    html += QStringLiteral("<p><span style=\" font-size:11pt;\">1.通知缘由</span></p>");
    html += QString("<p style=\"margin-left:20px;\"><span style=\" font-size:11pt;\">%1</span></p>").arg(notice.Reason);
    html += QStringLiteral("<p><span style=\" font-size:11pt;\">2.通知事项</span></p>");
    html += QString("<p style=\"margin-left:20px;\"><span style=\" font-size:11pt;\">  %1</span></p>").arg(notice.Thing);
    html += QStringLiteral("<p><span style=\" font-size:11pt;\">3.通知结语</span></p>");
    html += QString("<p style=\"margin-left:20px;\"><span style=\" font-size:11pt;\">  %1</span></p>").arg(notice.Peroration);
    html += QString("<p align=\"right\"><span style=\" font-size:11pt;\">%1</span></p>").arg(notice.Organization);
    html += QString("<p align=\"right\"><span style=\" font-size:11pt;\">%1</span></p>").arg(notice.date);
    html += QString("</body></html>");
    return html;
}
void NoticeCore::saveNoticeList(QList<XmppClient::NoticeBase> itemList, QString iniName)
{
    QFileInfo fileInfo(iniName);
    if(fileInfo.suffix()!=QLatin1String(".ini")) {
        iniName=fileInfo.baseName()+".ini";
    }
    QString path = SaveSetting::getInstance().getSettingsDirPath() +iniName;
    qDebug() << "Saving Notice settings at " << path;
    QSettings ps(path, QSettings::IniFormat);
    ps.setIniCodec("UTF-8");  //设置编码
    ps.clear();               //清理
    ps.beginGroup("Notice");
        ps.beginWriteArray("Notices", itemList.size());     //好友 列表大小
        int index = 0;
        foreach(XmppClient::NoticeBase item ,itemList)
        {
            ps.setArrayIndex(index);
            ps.setValue("NoticeStyle", item.NoticeStyle);
            ps.setValue("Urgency", item.Urgency);
            ps.setValue("Important", item.Important);
            ps.setValue("Secret", item.Secret);
            ps.setValue("Subject", item.Subject);
            ps.setValue("Appellation", item.Appellation);
            ps.setValue("Reason", item.Reason);
            ps.setValue("Thing", item.Thing);
            ps.setValue("Peroration", item.Peroration);
            ps.setValue("Organization", item.Organization);
            ps.setValue("date", item.date);
            index++;
        }
        ps.endArray();
    ps.endGroup();
}
QList<XmppClient::NoticeBase> NoticeCore::getNoticeList(QString iniName)
{
    QFileInfo fileInfo(iniName);
    if(fileInfo.suffix()!=QLatin1String(".ini")) {
        iniName=fileInfo.baseName()+".ini";
    }
    QList<XmppClient::NoticeBase> noticelist;
    QString path = SaveSetting::getInstance().getSettingsDirPath() +iniName;
    qDebug() << "Load Notice settings at " << path;
    QSettings ps(path, QSettings::IniFormat);
    ps.setIniCodec("UTF-8");  //设置编码

    ps.beginGroup("Notice");
        int size = ps.beginReadArray("Notices");     //好友 列表大小
        noticelist.reserve(size);
        for (int index=0; index < size; index++)
        {
            ps.setArrayIndex(index);
            XmppClient::NoticeBase item;
            item.NoticeStyle=(XmppClient::NoticeBase::style)ps.value("NoticeStyle", 0x0).toInt();
            item.Urgency=ps.value("Urgency", false).toBool();
            item.Important=ps.value("Important", false).toBool();
            item.Secret=ps.value("Secret", false).toBool();
            item.Subject=ps.value("Subject", QString()).toString();
            item.Appellation=ps.value("Appellation", QString()).toString();
            item.Reason=ps.value("Reason", QString()).toString();
            item.Thing=ps.value("Thing", QString()).toString();
            item.Peroration=ps.value("Peroration", QString()).toString();
            item.Organization=ps.value("Organization", QString()).toString();
            item.date=ps.value("date", QString()).toString();
            noticelist.append(item);
        }
        ps.endArray();
    ps.endGroup();
    qDebug()<<noticelist.size();
    return noticelist;
}
