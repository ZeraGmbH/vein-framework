#ifndef VFCPPRPCSIMPLIFIED_H
#define VFCPPRPCSIMPLIFIED_H

#include "ve_eventsystem.h"
#include <QString>

namespace VfCpp {

class VfCppRpcSimplified : public QObject
{
    Q_OBJECT
public:
    VfCppRpcSimplified(VeinEvent::EventSystem *eventSystem, int entityId, const QString &rpcSignature);
    QString getSignature();
    void callFunction(const QUuid &callId, const QUuid &peerId, const QVariantMap &parameters);
    void sendRpcResult(const QUuid &callId, const QVariant &result);
    void sendRpcError(const QUuid &callId, const QString &errorStr);
signals:
    void callRpc(const QUuid &callId, const QVariantMap &parameters);
private slots:
    virtual void callRPCFunction(const QUuid &callId, const QVariantMap &parameters) = 0;
private:
    void sendRPCresultEvent(const QUuid &callId, const QVariantMap &returnVal);
    VeinEvent::EventSystem *m_eventSystem;
    int m_entityId;
    QString m_rpcSignature;
    QHash<QUuid, QUuid> m_callIdPeerIdHash;
};
typedef std::shared_ptr<VfCppRpcSimplified> VfCppRpcSimplifiedPtr;
}

#endif // VFCPPRPCSIMPLIFIED_H
