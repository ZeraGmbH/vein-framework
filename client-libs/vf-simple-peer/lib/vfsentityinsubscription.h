#ifndef VFSENTITYINSUBSCRIPTION_H
#define VFSENTITYINSUBSCRIPTION_H

#include "vfcommandeventhandleritem.h"

class VfsEntityInSubscription : public QObject, public VfCommandEventHandlerItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfsEntityInSubscription> create(int entityId);
    VfsEntityInSubscription(int entityId);
    void sendSubscrption();
    // get subscription data
signals:
    void sigSubscribed(bool ok, int entityId);
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
};

typedef std::shared_ptr<VfsEntityInSubscription> VfsEntityInSubscriptionPtr;

#endif // VFSENTITYINSUBSCRIPTION_H
