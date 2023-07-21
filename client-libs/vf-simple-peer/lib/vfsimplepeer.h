#ifndef VFSIMPLEPEER_H
#define VFSIMPLEPEER_H

#include "vfsentityinsubscription.h"
#include "vfsentitysubscribed.h"
#include "ve_eventsystem.h"
#include "ve_commandevent.h"
#include <map>

class VfSimplePeer : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    VfSimplePeer();
    static int getNextTransactionId();
    void getComponent(int transactionId, int entityId, QString componentName);
    void setComponent(int transactionId, int entityId, QString componentName, QVariant value);
    //void getEntityIdFromName(QString entityName);
signals:
    void sigGetFinish(int transactionId, bool ok, QVariant value);
    void sigSetFinish(int transactionId, bool ok);

private slots:
    void onSubscriptionFinish(int entityId);
private:
    void processEvent(QEvent *event) override;
    void processCommandEvent(int entityId, VeinEvent::CommandEvent *cmdEvent);

    QMap<int/*entityId*/, VfsEntityInSubscriptionPtr> m_entitiesInSubsription;
    //std::map<int/*entityId*/, VfsEntitySubscribed> m_entitiesSubsribed;
    struct TransactionData
    {
        int entityId;
        QString componentName;
        QVariant value;
    };

    static int m_transactionId;
};

#endif // VFSIMPLEPEER_H
