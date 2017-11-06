#ifndef COMMUNICATIONACTIVATOR_H
#define COMMUNICATIONACTIVATOR_H
#include <ctkPluginActivator.h>
#include <QObject>
#include "communicationservice.h"

class CommunicationActivator : public QObject, public ctkPluginActivator
{
    Q_OBJECT
    Q_INTERFACES(ctkPluginActivator)

#if (QT_VERSION_MAJOR>4)
    Q_PLUGIN_METADATA(IID "org.commontk.communication.activator")
#endif
public:
    explicit CommunicationActivator(QObject *parent = 0);

    void start(ctkPluginContext *contex);
    void stop(ctkPluginContext *context);
private:
    QScopedPointer<CommunicationService> s;
};

#endif // COMMUNICATIONACTIVATOR_H
