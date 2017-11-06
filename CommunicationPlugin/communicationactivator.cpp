#include "communicationactivator.h"
#include "communicationplugin.h"
CommunicationActivator::CommunicationActivator(QObject *parent) : QObject(parent)
{

}
void CommunicationActivator::start(ctkPluginContext *contex)
{
    s.reset(new CommunicationPlugin(contex));
}
void CommunicationActivator::stop(ctkPluginContext *context)
{
    Q_UNUSED(context)
}
