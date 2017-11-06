#include "communicationplugin.h"
#include <QDebug>

CommunicationPlugin::CommunicationPlugin(QObject *parent) : QObject(parent)
{

}
CommunicationPlugin::CommunicationPlugin(ctkPluginContext *pc)
{
    pc->registerService<CommunicationService>(this);
}

void CommunicationPlugin::init()
{
    qDebug()<<"Communication init";
}
