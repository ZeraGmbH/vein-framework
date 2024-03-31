#include "vn_introspectionsystem.h"
#include <ve_commandevent.h>
#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>
#include <vcmp_introspectiondata.h>
#include <vcmp_remoteproceduredata.h>
#include <vcmp_errordata.h>
#include <vcmp_errordatasender.h>
#include <QEvent>
#include <QJsonArray>

Q_LOGGING_CATEGORY(VEIN_NET_INTRO, VEIN_DEBUGNAME_NET_INTRO)
Q_LOGGING_CATEGORY(VEIN_NET_INTRO_VERBOSE, VEIN_DEBUGNAME_NET_INTRO_VERBOSE)

using namespace VeinEvent;
using namespace VeinComponent;

namespace VeinNet
{
struct EntityIntrospection
{
public:
    QMap<QString, int> m_components;
    QMap<QString, int> m_procedures;
};

IntrospectionSystem::IntrospectionSystem(QObject *parent) :
    VeinEvent::EventSystem(parent)
{
    const auto listToClean = m_introspectionData.values();
    for(EntityIntrospection *toDelete : qAsConst(listToClean))
        delete toDelete;
    m_introspectionData.clear();
}

const QString IntrospectionSystem::s_nameComponent = QLatin1String("EntityName");

void IntrospectionSystem::processEvent(QEvent *event)
{
    Q_ASSERT(event != nullptr);
    if(event->type() == CommandEvent::getQEventType()) {
        CommandEvent *cEvent = static_cast<CommandEvent *>(event);
        EventData *evData = cEvent->eventData();
        const int entityId = evData->entityId();
        if(cEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION) {
            switch(evData->type())
            {
            case EntityData::dataType():
            {
                EntityData *eData = static_cast<EntityData *>(evData);
                switch(eData->eventCommand())
                {
                case EntityData::Command::ECMD_ADD:
                    if(eData->eventOrigin() == VeinEvent::EventData::EventOrigin::EO_LOCAL) {
                        if(m_introspectionData.contains(entityId)) {
                            //remove the old entry to prevent leaking
                            delete m_introspectionData.value(entityId);
                        }
                        m_introspectionData.insert(entityId, new EntityIntrospection());
                    }
                    break;
                case EntityData::Command::ECMD_SUBSCRIBE:
                {
                    vCDebug(VEIN_NET_INTRO_VERBOSE) << "Processing command event:" << cEvent << "with command ECMD_SUBSCRIBE, entityId:" << entityId;
                    QJsonObject tmpObject = getJsonIntrospection(entityId);
                    if(tmpObject.isEmpty() == false) {
                        IntrospectionData *newData = new IntrospectionData();
                        newData->setEntityId(entityId);
                        newData->setJsonData(tmpObject);
                        newData->setEventOrigin(IntrospectionData::EventOrigin::EO_LOCAL);
                        newData->setEventTarget(IntrospectionData::EventTarget::ET_ALL);

                        CommandEvent *newEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, newData);
                        /// @note sets the peer id to be the sender peer id, used for unicasting the message
                        newEvent->setPeerId(cEvent->peerId());

                        vCDebug(VEIN_NET_INTRO_VERBOSE) << "Sending introspection event:" << newEvent;

                        emit sigSendEvent(newEvent);
                    }
                    else {
                        QString tmpErrorString = tr("No introspection available for requested entity, entity id: %1").arg(entityId);
                        event->accept();
                        qCWarning(VEIN_NET_INTRO) << tmpErrorString;

                        ErrorData *errData = new ErrorData();

                        errData->setEntityId(entityId);
                        errData->setOriginalData(eData);
                        errData->setEventOrigin(EventData::EventOrigin::EO_LOCAL);
                        errData->setEventTarget(eData->eventTarget());
                        errData->setErrorDescription(tmpErrorString);

                        CommandEvent *tmpCommandEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, errData);
                        tmpCommandEvent->setPeerId(cEvent->peerId());
                        emit sigSendEvent(tmpCommandEvent);
                    }
                    break;
                }
                default:
                    break;
                }
                break;
            }
            case ComponentData::dataType():
            {
                ComponentData *cData = static_cast<ComponentData *>(evData);
                if(cData->eventOrigin() == VeinEvent::EventData::EventOrigin::EO_LOCAL) {
                    switch(cData->eventCommand())
                    {
                    case ComponentData::Command::CCMD_ADD:
                        if(m_introspectionData.contains(entityId))
                            m_introspectionData.value(entityId)->m_components.insert(cData->componentName(), 0);
                        else
                            ErrorDataSender::errorOut(QString("Cannot add value for invalid entity id: %1").arg(entityId), event, this);
                        break;
                    case ComponentData::Command::CCMD_REMOVE:
                        if(m_introspectionData.contains(entityId))
                            m_introspectionData.value(entityId)->m_components.remove(cData->componentName());
                        else
                            ErrorDataSender::errorOut(QString("Cannot remove value for invalid entity id: %1").arg(entityId), event, this);
                        break;
                    default:
                        break;
                    }
                }
                break;
            }
            case RemoteProcedureData::dataType():
            {
                RemoteProcedureData *rpcData = static_cast<RemoteProcedureData *>(evData);
                if(rpcData->eventOrigin() == VeinEvent::EventData::EventOrigin::EO_LOCAL) {
                    if(rpcData->command() == RemoteProcedureData::Command::RPCMD_REGISTER) {
                        Q_ASSERT(m_introspectionData.contains(entityId));
                        m_introspectionData.value(entityId)->m_procedures.insert(rpcData->procedureName(), 0);
                    }
                }
            }
            default:
                break;
            }
        }
    }
}

QJsonObject IntrospectionSystem::getJsonIntrospection(int entityId) const
{
    QJsonObject retVal;
    if(m_introspectionData.contains(entityId)) {
        retVal.insert(QString("components"), QJsonArray::fromStringList(m_introspectionData.value(entityId)->m_components.keys()));
        retVal.insert(QString("procedures"), QJsonArray::fromStringList(m_introspectionData.value(entityId)->m_procedures.keys()));
    }
    return retVal;
}

} // namespace VeinNet
