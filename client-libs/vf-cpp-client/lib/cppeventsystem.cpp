#include "cppeventsystem.h"
#include <ve_commandevent.h>
#include <ve_eventdata.h>
#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>
#include <vcmp_errordata.h>
#include <vcmp_introspectiondata.h>
#include <vcmp_remoteproceduredata.h>

using namespace VeinEvent;
using namespace VeinComponent;

CppEventSystem::CppEventSystem()
{
}

CppEventSystem::ConnectionState CppEventSystem::state() const
{
    return m_state;
}

CppEntityComponentMapPtr CppEventSystem::getEntity(const QString &entityName) const
{

}

bool CppEventSystem::hasEntity(const QString &entityName) const
{

}

CppEntityComponentMapPtr CppEventSystem::getEntityById(int id) const
{
    CppEntityComponentMapPtr ret;
    if(m_entities.contains(id))
        ret = m_entities[id];
    return ret;
}

QList<int> CppEventSystem::getEntityList() const
{

}

void CppEventSystem::entitySubscribeById(int entityId)
{

}

void CppEventSystem::entityUnsubscribeById(int entityId)
{

}

bool CppEventSystem::processEvent(QEvent *event)
{
    bool retVal = false;

    if(event->type()==CommandEvent::eventType()) {
        CommandEvent *cEvent = static_cast<CommandEvent *>(event);
        Q_ASSERT(cEvent != nullptr);
        EventData *evData = cEvent->eventData();
        Q_ASSERT(evData != nullptr);

        if(cEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION) {
            /// @todo add support for network events (connected / disconnected / error)
            switch(evData->type()) {
            case ComponentData::dataType(): {
                ComponentData *cData=nullptr;
                cData = static_cast<ComponentData *>(evData);
                Q_ASSERT(cData != nullptr);
                retVal = true;
                if(m_entities.contains(cData->entityId())) /// @note component data is only processed after the introspection has been processed
                    m_entities.value(cData->entityId())->processComponentData(cData);
                break;
            }
            case EntityData::dataType(): {
                EntityData *eData = static_cast<EntityData *>(evData);
                retVal = true;
                int entityId = eData->entityId();
                switch(eData->eventCommand()) {
                case EntityData::Command::ECMD_REMOVE: {
                    if(m_entities.contains(entityId)) {
                        CppEntityComponentMapPtr eMap = m_entities.value(entityId);
                        eMap->setState(CppEntityComponentMap::DataState::ECM_REMOVED);
                        m_entities.remove(entityId);
                        //eMap->deleteLater();
                        if(m_entitySubscriptionReferenceTables.contains(entityId)) {
                            m_resolvedIds.remove(entityId);
                            qCritical("Required entity was removed remotely, entity id: %i", entityId);
                            m_state = ConnectionState::VQ_ERROR;
                            emit sigStateChanged(m_state);
                        }
                    }
                    break;
                }
                default:
                    break;
                }
                break;
            }
            case ErrorData::dataType():  { /// @todo add message queue and check if the error belongs to actions taken from this client
                ErrorData *errData = nullptr;
                errData = static_cast<ErrorData *>(evData);
                qWarning("Received error: %s", qPrintable(errData->errorDescription()));
                break;
            }
            case IntrospectionData::dataType(): {
                IntrospectionData *iData = static_cast<IntrospectionData *>(evData);
                retVal = true;
                int entityId = iData->entityId();
                if(m_entities.contains(entityId) == false) {
                    CppEntityComponentMapPtr eMap = std::make_shared<CppEntityComponentMap>(entityId, iData->jsonData().toVariantHash());
                    m_entities.insert(entityId, eMap);
                    connect(eMap.get(), &CppEntityComponentMap::sigSendEvent, this, &CppEventSystem::sigSendEvent);
                    connect(eMap.get(), &CppEntityComponentMap::sigEntityComplete, this, &CppEventSystem::onEntityLoaded);
                    eMap->setState(CppEntityComponentMap::DataState::ECM_PENDING);
                }
                break;
            }
            /*case RemoteProcedureData::dataType():
            {
                RemoteProcedureData *rpcData=nullptr;
                rpcData = static_cast<RemoteProcedureData *>(evData);
                Q_ASSERT(rpcData != nullptr);
                retVal = true;

                if(m_entities.contains(rpcData->entityId())) /// @note component data is only processed after the introspection has been processed
                {
                    m_entities.value(rpcData->entityId())->processRemoteProcedureData(rpcData);
                }
                break;
            }*/
            default:
                break;
            }
        }
    }
    return retVal;

}

void CppEventSystem::onEntityLoaded(int entityId)
{

}

void CppEventSystem::addEntity(CppEntityComponentMapPtr entity, int id, const QString &name)
{
    m_entities[id] = entity;
    m_entitiesStr[name] = entity;
}
