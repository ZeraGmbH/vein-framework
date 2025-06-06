#include "veinqml.h"
#include "entitycomponentmap.h"
#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>
#include <vcmp_errordata.h>
#include <vcmp_introspectiondata.h>
#include <vcmp_remoteproceduredata.h>
#include <vf_client_entity_subscriber.h>
#include <vf_client_entity_unsubscriber.h>
#include <QQmlEngine>

Q_LOGGING_CATEGORY(VEIN_API_QML, VEIN_DEBUGNAME_QML)
Q_LOGGING_CATEGORY(VEIN_API_QML_VERBOSE, VEIN_DEBUGNAME_QML_VERBOSE)

using namespace VeinEvent;
using namespace VeinComponent;

namespace VeinApiQml
{

VeinQml::VeinQml(QObject *t_parent) : EventSystem(t_parent)
{
}

VeinQml::ConnectionState VeinQml::state() const
{
    return m_state;
}

EntityComponentMap *VeinQml::getEntity(const QString &t_entityName) const
{
    EntityComponentMap *retVal = m_entityDict.findByName(t_entityName);
    if(!retVal)
        qWarning() << "No entity found with name:" << t_entityName;

    // we take care
    QQmlEngine::setObjectOwnership(retVal, QQmlEngine::CppOwnership);
    return retVal;
}

bool VeinQml::hasEntity(const QString &t_entityName) const
{
    return m_entityDict.findByName(t_entityName) != nullptr;
}

EntityComponentMap *VeinQml::getEntityById(int t_id) const
{
    EntityComponentMap *retVal = m_entityDict.findById(t_id);
    if(retVal)
        // we take care
        QQmlEngine::setObjectOwnership(retVal, QQmlEngine::CppOwnership);
    return retVal;
}

QList<int> VeinQml::getEntityList() const
{
    return m_entityDict.getEntityList();
}

VeinQml *VeinQml::getStaticInstance()
{
    return s_staticInstance;
}

void VeinQml::setStaticInstance(VeinQml *t_instance)
{
    if(t_instance)
        s_staticInstance = t_instance;
}

void VeinQml::entitySubscribeById(int t_entityId)
{
    if(!m_entityDict.findById(t_entityId))
        emit sigSendEvent(VfClientEntitySubscriber::generateEvent(t_entityId));
    quint32 subscriptionCount = m_entitySubscriptionReferenceTables.value(t_entityId, 0);
    subscriptionCount++;
    m_entitySubscriptionReferenceTables.insert(t_entityId, subscriptionCount);
    vCDebug(VEIN_API_QML_VERBOSE) << "Subscription added for entity:" << t_entityId << "new subscriptionCount:" << subscriptionCount;
}

void VeinQml::entityUnsubscribeById(int t_entityId)
{
    Q_ASSERT(m_entityDict.findById(t_entityId));//unsubscribe for unknown entity?
    quint32 subscriptionCount = m_entitySubscriptionReferenceTables.value(t_entityId, 0);
    Q_ASSERT(subscriptionCount>0); //unsubscribe when never subscribed?
    if(subscriptionCount > 0) {
        subscriptionCount--;
        m_entitySubscriptionReferenceTables.insert(t_entityId, subscriptionCount);
    }
    if(subscriptionCount == 0) {
        removeEntity(t_entityId);
        emit sigSendEvent(VfClientEntityUnsubscriber::generateEvent(t_entityId));
    }
    vCDebug(VEIN_API_QML_VERBOSE) << "Subscription removed for entity:" << t_entityId << "new subscriptionCount:" << subscriptionCount;
}

void VeinQml::processEvent(QEvent *t_event)
{
    if(t_event->type()==CommandEvent::getQEventType()) {
        const CommandEvent *cEvent = static_cast<CommandEvent *>(t_event);
        EventData *evData = cEvent->eventData();
        if(cEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION) {
            /// @todo add support for network events (connected / disconnected / error)
            switch(evData->type())
            {
            case ComponentData::dataType():
            {
                const ComponentData *cData = static_cast<ComponentData *>(evData);
                int entityId = cData->entityId();
                EntityComponentMap *map = m_entityDict.findById(entityId);
                if(map) { /// @note component data is only processed after the introspection has been processed
                    map->processComponentData(cData);
                }
                break;
            }
            case EntityData::dataType(): // Never received!!!
            {
                EntityData *eData = static_cast<EntityData *>(evData);
                int entityId = eData->entityId();
                if(eData->eventCommand() == EntityData::Command::ECMD_REMOVE)
                    removeEntity(entityId);
                break;
            }
            case ErrorData::dataType(): /// @todo add message queue and check if the error belongs to actions taken from this client
            {
                ErrorData *errData = static_cast<ErrorData *>(evData);
                qCWarning(VEIN_API_QML_INTROSPECTION) << "Received error:" <<errData->errorDescription();
                break;
            }
            case IntrospectionData::dataType():
            {
                IntrospectionData *iData = static_cast<IntrospectionData *>(evData);
                int entityId = iData->entityId();
                vCDebug(VEIN_API_QML_VERBOSE) << "Received introspection data for entity:" << entityId;
                if(!m_entityDict.findById(entityId)) {
                    QVariantMap componentValues = iData->componentValues();
                    EntityComponentMap *eMap = new EntityComponentMap(entityId,
                                                                      componentValues,
                                                                      iData->rpcNames(),
                                                                      this);
                    m_entityDict.insert(entityId, eMap);
                    if (componentValues.contains("EntityName"))
                        m_entityDict.setEntityName(entityId, componentValues["EntityName"].toString());
                    else
                        qCritical("Entity %i has no component 'EntityName'!", entityId);
                    connect(eMap, &EntityComponentMap::sigSendEvent, this, &VeinQml::sigSendEvent);
                    connect(eMap, &EntityComponentMap::sigEntityComplete, this, &VeinQml::onEntityLoaded);
                    eMap->setState(EntityComponentMap::DataState::ECM_PENDING);
                }
                break;
            }
            case RemoteProcedureData::dataType():
            {
                RemoteProcedureData *rpcData = static_cast<RemoteProcedureData *>(evData);
                Q_ASSERT(rpcData != nullptr);
                EntityComponentMap *eMap = m_entityDict.findById(rpcData->entityId());
                if(eMap) /// @note component data is only processed after the introspection has been processed
                    eMap->processRemoteProcedureData(rpcData);
                break;
            }
            default:
                break;
            }
        }
    }
}

void VeinQml::onEntityLoaded(int t_entityId)
{
    if(m_entitySubscriptionReferenceTables.contains(t_entityId)) {
        vCDebug(VEIN_API_QML) << "Fetched required entity:" << t_entityId;
        m_resolvedIds.insert(t_entityId);
        emit sigEntityAvailable(m_entityDict.nameFromId(t_entityId));
        if(m_state != ConnectionState::VQ_LOADED) {
            QList<int> entitySubscriptionReferenceTableList = m_entitySubscriptionReferenceTables.keys();
            if(m_resolvedIds.contains(QSet<int>(entitySubscriptionReferenceTableList.begin(), entitySubscriptionReferenceTableList.end()))) {
                vCDebug(VEIN_API_QML) << "All required entities resolved";
                m_state = ConnectionState::VQ_LOADED;
                emit sigStateChanged(m_state);
            }
        }
    }
}

void VeinQml::removeEntity(int t_entityId)
{
    m_entitySubscriptionReferenceTables.remove(t_entityId);
    m_resolvedIds.remove(t_entityId);
    // Hack: As soon as an entity is removed we assume there will be
    // * others to leave
    // * new set coming back
    // That matches what session change does
    // Note: On session change two entities remain _SYSTEM and _LOGGER
    if(m_state != ConnectionState::VQ_IDLE) {
        m_state = ConnectionState::VQ_IDLE;
        emit sigStateChanged(m_state);
    }
    EntityComponentMap *toDelete = m_entityDict.remove(t_entityId);
    if(toDelete) {
        toDelete->setState(EntityComponentMap::DataState::ECM_REMOVED);
        toDelete->deleteLater();
    }
}

VeinQml *VeinQml::s_staticInstance = nullptr;
}
