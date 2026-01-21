#ifndef VFCPPENTITY_H
#define VFCPPENTITY_H

#include "vf-cpp-component.h"
#include "vf-cpp-rpc.h"
#include <QMap>

namespace VfCpp {

class VfCppEntity : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    VfCppEntity(int entityId);
    ~VfCppEntity();

    VfCppComponent::Ptr createComponent(const QString &name, const QVariant &initval, bool readOnly = false);
    void removeEntity();
    void removeComponent(const QString &componentName);
    int getEntityId() const;
    bool hasComponent(const QString &name);
    QMap<QString,VfCppComponent::Ptr> getComponents() const;

    // Notes
    // * parameter: the expected parameters key: name / value: metatype: e.g "int"
    // * There is no reason to use the rpc handler object
    cVeinModuleRpc::Ptr createRpc(QObject *object,
                                  const QString &funcName,
                                  const QMap<QString,QString> &parameter,
                                  bool thread = true);

    void initModule();
signals:
    void sigWatchedComponentChanged(int entityId, const QString &componentName, const QVariant &value);

private:
    void processEvent(QEvent *event) override;
    void handleComponents(VeinEvent::CommandEvent *cmdEvent);
    void handleRpcs(VeinEvent::CommandEvent *cmdEvent);
    int m_entityId;
    QMap<QString,VfCppComponent::Ptr> m_componentList;
    QMap<QString,cVeinModuleRpc::Ptr> m_rpcList;
};

}

#endif //VFCPPENTITY_H
