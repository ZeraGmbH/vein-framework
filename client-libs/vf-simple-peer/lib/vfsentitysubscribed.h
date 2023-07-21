#ifndef VFSENTITYSUBSCRIBED_H
#define VFSENTITYSUBSCRIBED_H

#include "vfcommandeventhandleritem.h"
#include <memory>

class VfsEntitySubscribed : public QObject, public VfCommandEventHandlerItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfsEntitySubscribed> create(int entityId, QStringList componentNames);
    VfsEntitySubscribed(int entityId, QStringList componentNames);

    void getComponent(QString componentName);
    void setComponent(QString componentName, QVariant value);
signals:
    void sigEntityDeleted(int entityId);
    void sigGetFinish(int entityId, QString componentName, bool ok, QVariant value);
    void sigSetFinish(int entityId, QString componentName, bool ok);
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    QStringList m_componentNames;
};

typedef std::shared_ptr<VfsEntitySubscribed> VfsEntitySubscribedPtr;

#endif // VFSENTITYSUBSCRIBED_H
