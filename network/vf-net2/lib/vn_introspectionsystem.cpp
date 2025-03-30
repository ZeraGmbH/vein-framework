#include "vn_introspectionsystem.h"
#include <ve_commandevent.h>
#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>
#include <vcmp_introspectiondata.h>
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
IntrospectionSystem::IntrospectionSystem(VeinStorage::AbstractEventSystem *storageSystem, QObject *parent) :
    VeinEvent::EventSystem(parent),
    m_storageSystem(storageSystem)
{
}

const QString IntrospectionSystem::s_nameComponent = QLatin1String("EntityName");

void IntrospectionSystem::processEvent(QEvent *event)
{
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
                case EntityData::Command::ECMD_SUBSCRIBE:
                {
                    vCDebug(VEIN_NET_INTRO_VERBOSE) << "Processing command event:" << cEvent << "with command ECMD_SUBSCRIBE, entityId:" << entityId;
                    VeinStorage::AbstractDatabase* storageDb = m_storageSystem->getDb();
                    const QStringList componentList = storageDb->getComponentList(entityId);
                    if (!componentList.isEmpty()) {
                        IntrospectionData *newData = new IntrospectionData();
                        newData->setEntityId(entityId);

                        QVariantMap componentValues;
                        for (const QString &component : componentList)
                            componentValues[component] = storageDb->getStoredValue(entityId, component);
                        newData->setComponentValues(componentValues);
                        newData->setRpcNames(getRpcNames(entityId));

                        newData->setEventOrigin(IntrospectionData::EventOrigin::EO_LOCAL);
                        newData->setEventTarget(IntrospectionData::EventTarget::ET_ALL);

                        CommandEvent *newEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, newData);
                        /// @note sets the peer id to be the sender peer id, used for unicasting the message
                        newEvent->setPeerId(cEvent->peerId());

                        vCDebug(VEIN_NET_INTRO_VERBOSE) << "Sending introspection event:" << newEvent;

                        emit sigSendEvent(newEvent);
                    }
                    else
                        ErrorDataSender::errorOut(QString("No introspection available for requested entity, entity id: %1").arg(entityId), event, this);
                    break;
                }
                default:
                    break;
                }
                break;
            }
            default:
                break;
            }
        }
    }
}

QStringList IntrospectionSystem::getRpcNames(int entityId) const
{
    const QMap<int, QStringList> rpcs = m_storageSystem->getRpcs();
    if (!rpcs.contains(entityId))
        return QStringList();
    return rpcs[entityId];
}

} // namespace VeinNet
