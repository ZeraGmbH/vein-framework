#ifndef VFSENTITYINSUBSCRIPTION_H
#define VFSENTITYINSUBSCRIPTION_H

#include "vfcommandeventhandler.h"
#include "vfcommandeventhandleritem.h"

class VfsEntityInSubscription : public QObject, public VfCommandEventHandlerItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfsEntityInSubscription> create(int entityId);
    VfsEntityInSubscription(int entityId);
    void sendSubscrption();
    QStringList getComponentNames();
signals:
    void sigSubscribed(bool ok, int entityId);
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void parseIntrospectionData(VeinEvent::EventData *evData);
    void finishSubscription(bool ok);
    QStringList m_componentNames;
};

typedef std::shared_ptr<VfsEntityInSubscription> VfsEntityInSubscriptionPtr;

#endif // VFSENTITYINSUBSCRIPTION_H
