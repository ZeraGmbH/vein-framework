#include "vf-cpp-entity.h"
#include "vcmp_errordatasender.h"
#include "vf_server_entity_add.h"
#include "vf_server_entity_remove.h"
#include "vf_server_component_remove.h"

using namespace VfCpp;

VfCppEntity::VfCppEntity(int entityId):
    m_entityId(entityId)
{
}

VfCppEntity::~VfCppEntity()
{
    emit sigSendEvent(VfServerEntityRemove::generateEvent(m_entityId));
}

bool VfCppEntity::hasComponent(const QString &name)
{
    return m_componentList.contains(name);
}

QMap<QString, VfCppComponent::Ptr> VfCppEntity::getComponents() const
{
    return m_componentList;
}

VfCppComponent::Ptr VfCppEntity::createComponent(const QString &name, const QVariant &initval, bool readOnly)
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

void VfCppEntity::removeEntity()
{
    m_componentList.clear();
    emit sigSendEvent(VfServerEntityRemove::generateEvent(m_entityId));
}

void VfCppEntity::removeComponent(const QString &componentName)
{
    if(m_componentList.contains(componentName))
        emit sigSendEvent(VfServerComponentRemove::generateEvent(m_entityId, componentName));
}

int VfCppEntity::getEntityId() const
{
    return m_entityId;
}

cVeinModuleRpc::Ptr VfCppEntity::createRpc(QObject *object,
                                           const QString &funcName,
                                           const QMap<QString, QString> &parameter,
                                           bool thread)
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
    emit sigSendEvent(VfServerEntityAdd::generateEvent(m_entityId));
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
                m_componentList[cName]->setValueFromVein(cData->newValue());
                cmdEvent->accept();
            }
        }
    }
}

void VfCpp::VfCppEntity::handleRpcs(VeinEvent::CommandEvent *cmdEvent)
{
    VeinComponent::RemoteProcedureData *rpcData = static_cast<VeinComponent::RemoteProcedureData *>(cmdEvent->eventData());
    bool rpcFound = false;
    QString procedureName;
    if(rpcData->command() == VeinComponent::RemoteProcedureData::Command::RPCMD_CALL) {
        for (auto it = m_rpcList.constBegin(); it != m_rpcList.constEnd(); ++it) {
            procedureName = it.key();
            if(procedureName.contains(rpcData->procedureName(), Qt::CaseInsensitive)) {
                rpcFound = true;
                break ;
            }
        }
        if(rpcFound) {
            const QUuid callId = rpcData->invokationData().value(VeinComponent::RemoteProcedureData::s_callIdString).toUuid();
            Q_ASSERT(!callId.isNull());
            m_rpcList[procedureName]->callFunction(callId, cmdEvent->peerId(), rpcData->invokationData());
            cmdEvent->accept();
        }
        else {
            cmdEvent->eventData()->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
            ErrorDataSender::errorOut(QString("No RPC with entityId: %1 name: %2").arg(m_entityId).arg(procedureName),
                                      cmdEvent,
                                      this);
        }
    }
}
