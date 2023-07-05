
#pragma once

#include <QList>
#include <QMap>
#include <ve_eventsystem.h>
#include <vf-cpp-component.h>
#include <vf-cpp-rpc.h>

namespace VfCpp {

class veinmoduleentity : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    typedef QSharedPointer<veinmoduleentity> Ptr;
    veinmoduleentity(int p_entityId,QObject *p_parent=nullptr);

    bool hasComponent(const QString name);
    int getEntitiyId() const;
    cVeinModuleComponent::Ptr createComponent(QString name, QVariant initval, bool p_readOnly=false);
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
    cVeinModuleRpc::Ptr createRpc(QObject *p_object, QString p_funcName, QMap<QString,QString> p_parameter, bool p_thread=true);
    void watchComponent(int p_EntityId, const QString &p_componentName);
    bool unWatchComponent(int p_EntityId, const QString &p_componentName);

signals:
    void sigWatchedComponentChanged(int p_entityId,QString p_componentName,QVariant p_value);
public slots:
    void initModule();
private:
    bool processEvent(QEvent *t_event) override;
    bool processCommandEvent(VeinEvent::CommandEvent *p_cEvent);
    QMap<QString,cVeinModuleComponent::Ptr> m_componentList;
    QMap<QString,cVeinModuleRpc::Ptr> m_rpcList;
    QMap<int,QSet<QString>> m_watchList;
    int m_entityId;
public:
};

}
