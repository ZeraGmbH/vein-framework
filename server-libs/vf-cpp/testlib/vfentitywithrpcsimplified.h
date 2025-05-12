#ifndef VFENTITYWITHRPCSIMPLIFIED_H
#define VFENTITYWITHRPCSIMPLIFIED_H

#include "ve_commandevent.h"
#include "ve_eventsystem.h"
#include "vf-cpp-rpc-simplified.h"

class VfEntityWithRpcSimplified : public VeinEvent::EventSystem
{
public:
    VfEntityWithRpcSimplified(int entityId);
    ~VfEntityWithRpcSimplified();
    void initModule();
    void createRpc(QObject *object, QString rpcName, QMap<QString,QString> parameters);
    void sendRpcResult(const QUuid &callId, QString rpcName, QVariant result);
private:
    void processEvent(QEvent *event) override;
    void handleRpcs(VeinEvent::CommandEvent *cmdEvent);
    int m_entityId;
    QMap<QString, QString> m_rpcSignatureList;
    QMap<QString, VfCpp::VfCppRpcSimplifiedPtr> m_rpcHandlerList;
};

#endif // VFENTITYWITHRPCSIMPLIFIED_H
