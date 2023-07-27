#ifndef VFCPPENTITY_H
#define VFCPPENTITY_H

#include "vf-cpp-component.h"
#include "vf-cpp-rpc.h"
#include <ve_eventsystem.h>
#include <QMap>

namespace VfCpp {

class VfCppEntity : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    VfCppEntity(int entityId);
    bool hasComponent(const QString name);

    VfCppComponent::Ptr createComponent(QString name, QVariant initval, bool readOnly = false);
    // Notes
    // * parameter: the expected parameters key: name / value: (metatype: e.g "int")
    // * There is no reason to use the rpc handler object
    cVeinModuleRpc::Ptr createRpc(QObject *object, QString funcName, QMap<QString,QString> parameter, bool thread = true);
    void watchComponent(int entityId, const QString &componentName);
    bool unWatchComponent(int entityId, const QString &componentName);

    void initModule();
signals:
    void sigWatchedComponentChanged(int entityId, QString componentName, QVariant value);
private:
    void processEvent(QEvent *event) override;
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent);
    void handleUnknownRpc(VeinEvent::CommandEvent *cmdEvent);
    QMap<QString,VfCppComponent::Ptr> m_componentList;
    QMap<QString,cVeinModuleRpc::Ptr> m_rpcList;
    QMap<int,QSet<QString>> m_watchList;
    int m_entityId;
};

}

#endif //VFCPPENTITY_H
