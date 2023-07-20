#ifndef VFSENTITYSUBSCRIBED_H
#define VFSENTITYSUBSCRIBED_H

#include <QObject>
#include <memory>

class VfsEntitySubscribed : public QObject
{
    Q_OBJECT
public:
    VfsEntitySubscribed(int entityId);
    void getComponent(int transactionId, int entityId, QString componentName);
    void setComponent(int transactionId, int entityId, QString componentName, QVariant value);
signals:
    void sigGetFinish(int transactionId, bool ok, QVariant value);
    void sigSetFinish(int transactionId, bool ok);

private:
    int m_entityId;
};

typedef std::unique_ptr<VfsEntitySubscribed> VfEntitySubscribedPtr;

#endif // VFSENTITYSUBSCRIBED_H
