#ifndef VF_ENTITY_RPC_EVENT_HANDLER_H
#define VF_ENTITY_RPC_EVENT_HANDLER_H

#include <vf-cpp-entity.h>
#include <QObject>

class vfEntityRpcEventHandler : public QObject
{
    Q_OBJECT
public:
    vfEntityRpcEventHandler(int id=99999);
    ~vfEntityRpcEventHandler();
    bool initOnce();
    VfCpp::VfCppEntity *getVeinEntity() const;
public slots:
    QVariant RPC_forTest(QVariantMap parameters);

private:
    VfCpp::VfCppEntity *m_entity;
    bool m_isInitalized;
};

#endif // VF_ENTITY_RPC_EVENT_HANDLER_H
