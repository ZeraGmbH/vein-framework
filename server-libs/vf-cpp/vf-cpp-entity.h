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
    VfCppComponent::Ptr createComponent(QString name, QVariant initval, bool readOnly = false);
    bool hasComponent(const QString name);
    // Notes
    // * parameter: the expected parameters key: name / value: metatype: e.g "int"
    // * There is no reason to use the rpc handler object
    cVeinModuleRpc::Ptr createRpc(QObject *object, QString funcName, QMap<QString,QString> parameter, bool thread = true);
    void initModule();
signals:
    void sigWatchedComponentChanged(int entityId, QString componentName, QVariant value);

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
