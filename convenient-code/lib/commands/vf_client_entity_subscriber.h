#ifndef VFATOMICCLIENTENTITYSUBSCRIBER_H
#define VFATOMICCLIENTENTITYSUBSCRIBER_H

#include "vf_cmd_event_item.h"

class VfClientEntitySubscriber : public QObject, public VfCmdEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfClientEntitySubscriber> create(int entityId);
    VfClientEntitySubscriber(int entityId);

    void sendSubscription();
    QStringList getComponentNames();
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void processErrorCommandEventData(VeinEvent::EventData *originalEventData) override;
signals:
    void sigSubscribed(bool ok, int entityId);
private:
    void parseIntrospectionData(VeinEvent::EventData *evData);
    void emitSigSubscribed(bool ok);
    QStringList m_componentNames;
};

typedef std::shared_ptr<VfClientEntitySubscriber> VfClientEntitySubscriberPtr;

#endif // VFATOMICCLIENTENTITYSUBSCRIBER_H
