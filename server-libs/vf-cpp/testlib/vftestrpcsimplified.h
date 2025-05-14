#ifndef VFTESTRPCSIMPLIFIED_H
#define VFTESTRPCSIMPLIFIED_H

#include "vfentitywithrpcsimplified.h"
#include <timerfactoryqt.h>

class VfTestRpcSimplified : public QObject
{
    Q_OBJECT
public:
    VfTestRpcSimplified(int entityId);
    VfEntityWithRpcSimplified *getEntity();
    void initOnce();
    void RPC_PublicMethod(QVariantMap parameters);
public slots:
    void RPC_forTest(QVariantMap parameters);
    void RPC_addDelay(QVariantMap parameters);
private:
    void RPC_PrivateMethod(QVariantMap parameters);
    std::shared_ptr<VfEntityWithRpcSimplified> m_entity;
    bool m_initialized = false;
    TimerTemplateQtPtr m_delayResponse;
};

#endif // VFTESTRPCSIMPLIFIED_H
