#ifndef COMMUNICATIONSERVICE_H
#define COMMUNICATIONSERVICE_H

#include <qobject.h>
struct CommunicationService
{
public:
    virtual ~CommunicationService(){}
    virtual void init()=0;
};
Q_DECLARE_INTERFACE(CommunicationService, "org.commontk.communication.server")


#endif // COMMUNICATIONSERVICE_H
