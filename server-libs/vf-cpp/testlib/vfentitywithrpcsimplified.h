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
    void createRpc(QString rpcName, QMap<QString,QString> parameters);
private:
    void processEvent(QEvent *event) override;
    void handleRpcs(VeinEvent::CommandEvent *cmdEvent);
    int m_entityId;
    QMap<QString,VfCpp::VfCppRpcSimplifiedPtr> m_rpcList;
};

#endif // VFENTITYWITHRPCSIMPLIFIED_H
