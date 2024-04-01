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
    if(event->type()==VeinEvent::CommandEvent::getQEventType()) {
        VeinEvent::CommandEvent *cmdEvent = static_cast<VeinEvent::CommandEvent *>(event);
        VeinEvent::EventData *evData = cmdEvent->eventData();
        if(evData->entityId() == m_entityId) {
            if (cmdEvent->eventData()->type() == VeinComponent::ComponentData::dataType())
                handleComponents(cmdEvent);
            else if(cmdEvent->eventData()->type() == VeinComponent::RemoteProcedureData::dataType())
                handleRpcs(cmdEvent);
        }
    }
}

void VfCpp::VfCppEntity::handleComponents(VeinEvent::CommandEvent *cmdEvent)
{
    VeinComponent::ComponentData* cData = static_cast<VeinComponent::ComponentData*> (cmdEvent->eventData());
    if (cmdEvent->eventSubtype() == VeinEvent::CommandEvent::EventSubtype::TRANSACTION) {
        if(cData->eventCommand() == VeinComponent::ComponentData::Command::CCMD_SET) {
            QString cName = cData->componentName();
            if(m_componentList.contains(cName)) {
                m_componentList[cName]->setValueByEvent(cData->newValue());
                cmdEvent->accept();
            }
        }
    }
}

void VfCpp::VfCppEntity::handleRpcs(VeinEvent::CommandEvent *cmdEvent)
{
    VeinComponent::RemoteProcedureData *rpcData = static_cast<VeinComponent::RemoteProcedureData *>(cmdEvent->eventData());
    if(rpcData->command() == VeinComponent::RemoteProcedureData::Command::RPCMD_CALL) {
        if(m_rpcList.contains(rpcData->procedureName())) {
            const QUuid callId = rpcData->invokationData().value(VeinComponent::RemoteProcedureData::s_callIdString).toUuid();
            Q_ASSERT(!callId.isNull());
            m_rpcList[rpcData->procedureName()]->callFunction(callId,cmdEvent->peerId(),rpcData->invokationData());
            cmdEvent->accept();
        }
        else
            handleUnknownRpc(cmdEvent);
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
