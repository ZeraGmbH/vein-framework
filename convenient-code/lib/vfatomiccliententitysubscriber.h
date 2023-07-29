#ifndef VFATOMICCLIENTENTITYSUBSCRIBER_H
#define VFATOMICCLIENTENTITYSUBSCRIBER_H

#include "vfcmdeventitem.h"

class VfAtomicClientEntitySubscriber : public QObject, public VfCmdEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfAtomicClientEntitySubscriber> create(int entityId);
    VfAtomicClientEntitySubscriber(int entityId);
    void sendSubscription();
    QStringList getComponentNames();
signals:
    void sigSubscribed(bool ok, int entityId);
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void parseIntrospectionData(VeinEvent::EventData *evData);
    void finishSubscription(bool ok);
    QStringList m_componentNames;
};

typedef std::shared_ptr<VfAtomicClientEntitySubscriber> VfAtomicClientEntitySubscriberPtr;

#endif // VFATOMICCLIENTENTITYSUBSCRIBER_H
