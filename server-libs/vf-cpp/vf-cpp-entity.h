
#pragma once

#include <QList>
#include <QMap>
#include <ve_eventsystem.h>
#include <vf-cpp-component.h>
#include <vf-cpp-rpc.h>

namespace VfCpp {

class VfCppEntity : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    typedef QSharedPointer<VfCppEntity> Ptr;
    VfCppEntity(int p_entityId,QObject *p_parent=nullptr);

    bool hasComponent(const QString name);
    int getEntitiyId() const;
    VfCppComponent::Ptr createComponent(QString name, QVariant initval, bool readOnly=false);
    /**
     * @brief createRpc creates a vein rpc
     * @param p_object: the object handling the rpc
     * @param p_funcName: the mehtod name in this object
     * @param p_parameter: the expected parameters key : name value: type
     * @return the rpc handler
     *
     * There is no reason to use the rpc handler object
     *
     */
    cVeinModuleRpc::Ptr createRpc(QObject *object, QString funcName, QMap<QString,QString> parameter, bool thread=true);
    void watchComponent(int entityId, const QString &componentName);
    bool unWatchComponent(int entityId, const QString &componentName);

signals:
    void sigWatchedComponentChanged(int entityId, QString componentName, QVariant value);
public slots:
    void initModule();
private:
    bool processEvent(QEvent *event) override;
    bool processCommandEvent(VeinEvent::CommandEvent *cmdEvent);
    QMap<QString,VfCppComponent::Ptr> m_componentList;
    QMap<QString,cVeinModuleRpc::Ptr> m_rpcList;
    QMap<int,QSet<QString>> m_watchList;
    int m_entityId;
public:
};

}
