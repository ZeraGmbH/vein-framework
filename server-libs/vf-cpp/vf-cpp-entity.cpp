#include "vf-cpp-entity.h"
#include "vcmp_entitydata.h"

using namespace VfCpp;

VfCppEntity::VfCppEntity(int entityId):
    m_entityId(entityId)
{
}

bool VfCppEntity::hasComponent(const QString name)
{
    return m_componentList.contains(name);
}

VfCppComponent::Ptr VfCppEntity::createComponent(QString name, QVariant initval, bool readOnly)
{
    if(!hasComponent(name)) {
        VfCppComponent::Ptr tmpPtr = VfCppComponent::Ptr(new VfCppComponent(m_entityId,
                                                                            this,
                                                                            name,
                                                                            initval,
                                                                            readOnly));
        m_componentList[tmpPtr->getName()] = tmpPtr;
        return tmpPtr;
    }
    else
        qFatal("VfCppEntity::createComponent: A component %s already exists", qPrintable(name));
}


cVeinModuleRpc::Ptr VfCppEntity::createRpc(QObject *object, QString funcName, QMap<QString, QString> parameter, bool thread)
{
    cVeinModuleRpc::Ptr tmpPtr = cVeinModuleRpc::Ptr(new cVeinModuleRpc(m_entityId,
                                                                        this,
                                                                        object,
                                                                        funcName,
                                                                        parameter,
                                                                        thread),
                                                     &QObject::deleteLater);
    m_rpcList[tmpPtr->rpcName()] = tmpPtr;
    return tmpPtr;
}

void VfCppEntity::initModule()
{
    VeinComponent::EntityData *eData = new VeinComponent::EntityData();
    eData->setCommand(VeinComponent::EntityData::Command::ECMD_ADD);
    eData->setEntityId(m_entityId);
    VeinEvent::CommandEvent *tmpEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, eData);
    emit sigSendEvent(tmpEvent);
}

void VfCppEntity::processEvent(QEvent *event)
{
    if(event->type()==VeinEvent::CommandEvent::eventType()) {
        VeinEvent::CommandEvent *cEvent = static_cast<VeinEvent::CommandEvent *>(event);
        Q_ASSERT(cEvent != nullptr);
        VeinEvent::EventData *evData = cEvent->eventData();
        Q_ASSERT(evData != nullptr);
        if(evData->entityId() == m_entityId)
            processCommandEvent(cEvent);
    }
}

void VfCppEntity::watchComponent(int entityId, const QString &componentName)
{
    m_watchList[entityId].insert(componentName);
}

bool VfCppEntity::unWatchComponent(int entityId, const QString &componentName)
{
    bool retVal = false;
    if(m_watchList.contains(entityId)) {
        if(m_watchList[entityId].contains(componentName)) {
            m_watchList[entityId].remove(componentName);
            retVal = true;
        }
    }
    return retVal;
}

void VfCppEntity::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    // handle components
    if (cmdEvent->eventData()->type() == VeinComponent::ComponentData::dataType()) {
        VeinComponent::ComponentData* cData = static_cast<VeinComponent::ComponentData*> (cmdEvent->eventData());
        QString cName = cData->componentName();
        int entityId = cData->entityId();
        // managed by this entity
        if (cmdEvent->eventSubtype() == VeinEvent::CommandEvent::EventSubtype::TRANSACTION) {
            if(cData->eventCommand() == VeinComponent::ComponentData::Command::CCMD_SET) {
                if(m_componentList.contains(cName) && entityId == m_entityId) {
                    m_componentList[cName]->setValueByEvent(cData->newValue());
                    cmdEvent->accept();
                }
            }
        }
        // managed by other entites
        else if(cmdEvent->eventSubtype() == VeinEvent::CommandEvent::EventSubtype::NOTIFICATION) {
            if(m_watchList.contains(entityId)) {
                if(m_watchList[entityId].contains(cName))
                    emit sigWatchedComponentChanged(entityId,cName,cData->newValue());
            }
        }
    }
    // handle rpcs
    else if(cmdEvent->eventData()->type() == VeinComponent::RemoteProcedureData::dataType()) {
        VeinComponent::RemoteProcedureData *rpcData = static_cast<VeinComponent::RemoteProcedureData *>(cmdEvent->eventData());
        if(rpcData->command() == VeinComponent::RemoteProcedureData::Command::RPCMD_CALL) {
            if(m_rpcList.contains(rpcData->procedureName())) {
                const QUuid callId = rpcData->invokationData().value(VeinComponent::RemoteProcedureData::s_callIdString).toUuid();
                Q_ASSERT(callId.isNull() == false);
                m_rpcList[rpcData->procedureName()]->callFunction(callId,cmdEvent->peerId(),rpcData->invokationData());
                cmdEvent->accept();
            }
            else
                handleUnknownRpc(cmdEvent);
        }
    }
}

void VfCppEntity::handleUnknownRpc(VeinEvent::CommandEvent *cmdEvent)
{
    VeinComponent::RemoteProcedureData *rpcData = static_cast<VeinComponent::RemoteProcedureData *>(cmdEvent->eventData());
    qWarning() << "No remote procedure with entityId:" << m_entityId << "name:" << rpcData->procedureName();
    VF_ASSERT(false, QStringC(QString("No remote procedure with entityId: %1 name: %2").arg(m_entityId).arg(rpcData->procedureName())));
    VeinComponent::ErrorData *eData = new VeinComponent::ErrorData();
    eData->setEntityId(m_entityId);
    eData->setErrorDescription(QString("No remote procedure with name: %1").arg(rpcData->procedureName()));
    eData->setOriginalData(rpcData);
    eData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    eData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    VeinEvent::CommandEvent *errorEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, eData);
    errorEvent->setPeerId(cmdEvent->peerId());
    cmdEvent->accept();
    emit sigSendEvent(errorEvent);
}
