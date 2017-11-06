#ifndef COMMUNICATIONPLUGIN_H
#define COMMUNICATIONPLUGIN_H

#include <QObject>
#include <QWidget>
#include <ctkPluginContext.h>
#include "communicationservice.h"

class CommunicationPlugin : public QObject, public CommunicationService
{
    Q_OBJECT
     Q_INTERFACES(CommunicationService)
public:
    CommunicationPlugin(QObject *parent = 0);

    CommunicationPlugin(ctkPluginContext *pc);
    virtual void init();
};

#endif // COMMUNICATIONPLUGIN_H
