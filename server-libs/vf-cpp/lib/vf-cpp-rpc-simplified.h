#ifndef VFCPPRPCSIMPLIFIED_H
#define VFCPPRPCSIMPLIFIED_H

#include "ve_eventsystem.h"
#include "vf-cpp-rpc-signature.h"
#include <QString>

namespace VfCpp {

class VfCppRpcSimplified
{
public:
    VfCppRpcSimplified(VeinEvent::EventSystem *eventSystem, int entityId, QString rpcName, VfCpp::VfCppRpcSignature::RPCParams parameters);
    QString getSignature();
    void callFunction(const QUuid &callId, const QUuid &peerId, const QVariantMap &parameters);
private:
    QString createRpcSignature(QString rpcName, QMap<QString,QString> paramDescriptions);
    VeinEvent::EventSystem *m_eventSystem;
    int m_entityId;
    QString m_rpcSignature;
};
typedef std::shared_ptr<VfCppRpcSimplified> VfCppRpcSimplifiedPtr;
}

#endif // VFCPPRPCSIMPLIFIED_H
