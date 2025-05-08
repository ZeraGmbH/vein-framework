#ifndef VFCPPRPCSIMPLIFIED_H
#define VFCPPRPCSIMPLIFIED_H

#include "ve_eventsystem.h"
#include <QString>

class VfCppRpcSimplified
{
public:
    VfCppRpcSimplified(VeinEvent::EventSystem *eventSystem, int entityId, QString rpcName, QMap<QString,QString> parameters);
private:
    QString createRpcSignature(QString rpcName, QMap<QString,QString> paramDescriptions);
    VeinEvent::EventSystem *m_eventSystem;
    int m_entityId;
    QString m_rpcSignature;
};

#endif // VFCPPRPCSIMPLIFIED_H
