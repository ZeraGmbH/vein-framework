#ifndef VFATOMICCLIENTENTITYUNSUBSCRIBER_H
#define VFATOMICCLIENTENTITYUNSUBSCRIBER_H

#include "vfcmdeventitem.h"

class VfAtomicClientEntityUnsubscriber : public QObject, public VfCmdEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfAtomicClientEntityUnsubscriber> create(int entityId);
    VfAtomicClientEntityUnsubscriber(int entityId);

    void sendUnsubscription();
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void processErrorCommandEventData(VeinEvent::EventData *originalEventData) override;
signals:
    void sigUnsubscribed(bool ok, int entityId);
private:
    void emitSigUnsubscribed(bool ok);
};

typedef std::shared_ptr<VfAtomicClientEntityUnsubscriber> VfAtomicClientEntityUnsubscriberPtr;

#endif // VFATOMICCLIENTENTITYUNSUBSCRIBER_H
