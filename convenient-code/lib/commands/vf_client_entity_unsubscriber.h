#ifndef VFATOMICCLIENTENTITYUNSUBSCRIBER_H
#define VFATOMICCLIENTENTITYUNSUBSCRIBER_H

#include "vf_entity_event_item.h"

class VfClientEntityUnsubscriber : public QObject, public VfEntityEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfClientEntityUnsubscriber> create(int entityId);
    VfClientEntityUnsubscriber(int entityId);

    static QEvent* generateEvent(int entityId);
    void sendUnsubscription();
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void processErrorCommandEventData(VeinEvent::EventData *originalEventData) override;
signals:
    void sigUnsubscribed(bool ok, int entityId);
private:
    void emitSigUnsubscribed(bool ok);
};

typedef std::shared_ptr<VfClientEntityUnsubscriber> VfClientEntityUnsubscriberPtr;

#endif // VFATOMICCLIENTENTITYUNSUBSCRIBER_H
