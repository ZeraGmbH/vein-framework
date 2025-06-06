#include "entitycomponentmap.h"
#include <ve_commandevent.h>
#include <vcmp_componentdata.h>
#include <vf_client_component_fetcher.h>
#include <vf_client_component_setter.h>
#include <vcmp_remoteproceduredata.h>

using namespace VeinEvent;
using namespace VeinComponent;

Q_LOGGING_CATEGORY(VEIN_API_QML_INTROSPECTION, VEIN_DEBUGNAME_QML_INTROSPECTION)

namespace VeinApiQml
{
EntityComponentMap::EntityComponentMap(int entityId,
                                       const QVariantMap &componentValues,
                                       const QStringList &rpcNames,
                                       QObject *parent) :
    QQmlPropertyMap(this, parent),
    m_introspectionComponentValues(componentValues),
    m_introspectionRpcNames(rpcNames),
    m_entityId(entityId)
{
    Q_ASSERT(m_entityId>=0);
}

void EntityComponentMap::processComponentData(const ComponentData *cData)
{
    switch(cData->eventCommand()) {
    case ComponentData::Command::CCMD_ADD:
    case ComponentData::Command::CCMD_SET:
        vCDebug(VEIN_API_QML_VERBOSE) << "Updated value" << cData->componentName() << cData->newValue();
        insert(cData->componentName(), cData->newValue()); // bypasses the function updateValue(...)
        break;
    case ComponentData::Command::CCMD_REMOVE:
        /// @note It is not possible to remove keys from the map; once a key has been added, you can only modify or clear its associated value.
        /// @note Keys that have been cleared will still appear in this list, even though their associated values are invalid
        clear(cData->componentName());
        break;
    default:
        break;
    }
}

void EntityComponentMap::processRemoteProcedureData(const RemoteProcedureData *rpcData)
{
    const QUuid rpcIdentifier = rpcData->invokationData().value(RemoteProcedureData::s_callIdString).toUuid();
    if(m_pendingRPCCallbacks.contains(rpcIdentifier)) {
        switch(rpcData->command()) {
        case RemoteProcedureData::Command::RPCMD_RESULT:
            vCDebug(VEIN_API_QML_VERBOSE) << "Received RPC result for entity:" << m_entityId << "procedureName:" << rpcData->procedureName() << "rpcData:" << rpcData->invokationData();
            emit sigRPCFinished(rpcIdentifier, rpcData->invokationData());
            m_pendingRPCCallbacks.remove(rpcIdentifier);
            break;
        case RemoteProcedureData::Command::RPCMD_PROGRESS:
            emit sigRPCProgress(rpcIdentifier, rpcData->invokationData());
            break;
        default:
            break;
        }
    }
}

EntityComponentMap::DataState EntityComponentMap::state() const
{
    return m_state;
}

void EntityComponentMap::setState(EntityComponentMap::DataState t_dataState)
{
    m_state = t_dataState;
    if(m_state == DataState::ECM_PENDING)
        loadEntityData();
}

int EntityComponentMap::entityId() const
{
    return m_entityId;
}

bool EntityComponentMap::hasComponent(const QString &componentName) const
{
    if(m_state != DataState::ECM_READY)
        return false;
    return contains(componentName);
}

int EntityComponentMap::propertyCount() const
{
    return count();
}

QUuid EntityComponentMap::invokeRPC(const QString &procedureName, const QVariantMap &parameters)
{
    QUuid rpcIdentifier;
    //check if a component exists that is callable
    if(m_introspectionRpcNames.contains(procedureName)) {
        do {
            rpcIdentifier = QUuid::createUuid();
        } while(m_pendingRPCCallbacks.contains(rpcIdentifier)); //should only run once
        m_pendingRPCCallbacks.insert(rpcIdentifier, procedureName);
        QVariantMap rpcParamData;
        rpcParamData.insert(RemoteProcedureData::s_callIdString, rpcIdentifier);
        rpcParamData.insert(RemoteProcedureData::s_parameterString, parameters);

        RemoteProcedureData *rpcData = new RemoteProcedureData();
        rpcData->setEntityId(m_entityId);
        rpcData->setCommand(RemoteProcedureData::Command::RPCMD_CALL);
        rpcData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
        rpcData->setEventTarget(ComponentData::EventTarget::ET_ALL);
        rpcData->setProcedureName(procedureName);
        rpcData->setInvokationData(rpcParamData);
        CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, rpcData);
        vCDebug(VEIN_API_QML_VERBOSE) << "Sending RPC to entity:" << m_entityId << "procedure:" << procedureName << "data:" << rpcParamData << "event:" << cEvent;

        emit sigSendEvent(cEvent);
    }
    else {
        if(!m_introspectionRpcNames.isEmpty()) {
            qWarning() << "No rpc with procedure name:" << procedureName;
            qWarning() << "Available:";
            for(const auto &procedure : m_introspectionRpcNames)
                qWarning() << procedure;
        }
        else {
            qWarning() << "No rpc procedures found";
        }
    }
    return rpcIdentifier;
}

void EntityComponentMap::cancelRPCInvokation(QUuid identifier)
{
    if(m_pendingRPCCallbacks.contains(identifier)) {
        QVariantMap rpcParamData;
        rpcParamData.insert(RemoteProcedureData::s_callIdString, identifier);

        RemoteProcedureData *rpcData = new RemoteProcedureData();
        rpcData->setEntityId(m_entityId);
        rpcData->setCommand(RemoteProcedureData::Command::RPCMD_CALL); // <- cancelRPCInvokation is not used - shouldn't it be RPCMD_CANCELLATION ??
        rpcData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
        rpcData->setEventTarget(ComponentData::EventTarget::ET_ALL);
        rpcData->setProcedureName(m_pendingRPCCallbacks.value(identifier));
        rpcData->setInvokationData(rpcParamData);
        CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, rpcData);
        vCDebug(VEIN_API_QML_VERBOSE) << "Sending RPC cancellation to entity:" << m_entityId << "procedure:" << rpcData->procedureName() << "data:" << rpcParamData << "event:" << cEvent;

        emit sigSendEvent(cEvent);
    }
}

QList<QString> EntityComponentMap::getRemoteProcedureList() const
{
    return m_introspectionRpcNames;
}

QVariant EntityComponentMap::updateValue(const QString &t_key, const QVariant &t_newValue)
{
    const QVariant retVal = value(t_key);
    if(Q_UNLIKELY(t_newValue.isValid() == false)) {
        vCDebug(VEIN_API_QML) << QString("Invalid value for entity: %1 component: %2 value: ").arg(m_entityId).arg(t_key) << t_newValue;
        VF_ASSERT(t_newValue.isValid(), "Invalid value set from QML");
    }
    else if(retVal != t_newValue)
        emit sigSendEvent(VfClientComponentSetter::generateEvent(m_entityId, t_key, retVal, t_newValue));
    return retVal;
}

void EntityComponentMap::loadEntityData()
{
    for (auto iter=m_introspectionComponentValues.cbegin(); iter!=m_introspectionComponentValues.cend(); ++iter) {
        const QString componentName = iter.key();
        insert(componentName, iter.value()); // bypasses the function updateValue(...)
    }
    m_state = DataState::ECM_READY;
    emit sigEntityComplete(m_entityId);
}
} // end namespace VeinApiQml
