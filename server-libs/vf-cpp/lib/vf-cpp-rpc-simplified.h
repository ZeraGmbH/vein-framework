#ifndef VFCPPRPCSIMPLIFIED_H
#define VFCPPRPCSIMPLIFIED_H

#include "ve_eventsystem.h"
#include "vfmodulemetainfocontainer.h"
#include <QString>

namespace VfCpp {

class VfCppRpcSimplified : public QObject
{
    Q_OBJECT
public:
    VfCppRpcSimplified(VeinEvent::EventSystem *eventSystem, int entityId, QString rpcSignature);
    QString getSignature();
    void callFunction(const QUuid &callId, const QUuid &peerId, const QVariantMap &parameters);
    void sendRpcResult(const QUuid &callId, QVariant result);
    void sendRpcError(const QUuid &callId, QString errorStr);
    void setRPCScpiInfo(const QString &model, const QString &cmd,
                        int cmdTypeMask, // e.g SCPI::isQuery|SCPI::isCmdwP
                        const QString &rpcSignature);
    void exportRpcSCPIInfo(QJsonArray &jsArr);
signals:
    void callRpc(const QUuid &callId, const QVariantMap &parameters);
private slots:
    virtual void callRPCFunction(const QUuid &callId, const QVariantMap &parameters) = 0;
private:
    void sendRPCresultEvent(const QUuid &callId, QVariantMap returnVal);
    VeinEvent::EventSystem *m_eventSystem;
    int m_entityId;
    QString m_rpcSignature;
    QHash<QUuid, QUuid> m_callIdPeerIdHash;
    std::unique_ptr<VfModuleMetaInfoContainer> m_RpcScpiInfo;
};
typedef std::shared_ptr<VfCppRpcSimplified> VfCppRpcSimplifiedPtr;
}

#endif // VFCPPRPCSIMPLIFIED_H
