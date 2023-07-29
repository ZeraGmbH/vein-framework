#ifndef VFSIMPLEENTITYSUBSCRIBER_H
#define VFSIMPLEENTITYSUBSCRIBER_H

#include "vfcmdeventitem.h"

class VfSimpleEntitySubscriber : public QObject, public VfCmdEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfSimpleEntitySubscriber> create(int entityId);
    VfSimpleEntitySubscriber(int entityId);
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

typedef std::shared_ptr<VfSimpleEntitySubscriber> VfSimpleEntitySubscriberPtr;

#endif // VFSIMPLEENTITYSUBSCRIBER_H
