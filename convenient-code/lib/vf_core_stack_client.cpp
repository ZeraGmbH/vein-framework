#include "vf_core_stack_client.h"
#include "vf_client_entity_subscriber.h"

VfCoreStackClient::VfCoreStackClient()
{
    init();
}

VfCoreStackClient::VfCoreStackClient(VeinTcp::AbstractTcpWorkerFactoryPtr tcpWorkerFactory) :
    m_tcpSystem(tcpWorkerFactory)
{
    init();
}

void VfCoreStackClient::init()
{
    m_cmdEventHandlerSystem = VfCmdEventHandlerSystem::create();
    m_netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
    m_eventHandler.addSubsystem(&m_netSystem);
    m_eventHandler.addSubsystem(&m_tcpSystem);
    m_eventHandler.addSubsystem(m_cmdEventHandlerSystem.get());
    connect(&m_tcpSystem, &VeinNet::TcpSystem::sigConnnectionEstablished,
            this, &VfCoreStackClient::sigConnnectionEstablished);
}

void VfCoreStackClient::connectToServer(const QString &host, quint16 port)
{
    m_tcpSystem.connectToServer(host, port);
}

void VfCoreStackClient::subscribeEntity(int entityId)
{
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(entityId);
    m_pendingCommandEventItems[entityToSubscribe.get()] = entityToSubscribe;
    m_cmdEventHandlerSystem->addItem(entityToSubscribe);
    connect(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed,
            this, &VfCoreStackClient::onSubscribed);
    entityToSubscribe->sendSubscription();
}

void VfCoreStackClient::appendEventSystem(VeinEvent::EventSystem *system)
{
    m_eventHandler.addSubsystem(system);
}

void VfCoreStackClient::prependEventSystem(VeinEvent::EventSystem *system)
{
    m_eventHandler.prependSubsystem(system);
}

void VfCoreStackClient::addItem(VfCmdEventItemPtr item)
{
    m_cmdEventHandlerSystem->addItem(item);
}

VeinEvent::EventHandler *VfCoreStackClient::getEventHandler()
{
    return &m_eventHandler;
}

VfCmdEventHandlerSystemPtr VfCoreStackClient::getCmdEventHandlerSystem()
{
    return m_cmdEventHandlerSystem;
}

void VfCoreStackClient::onSubscribed(bool ok)
{
    VfCmdEventItemPtr item = m_pendingCommandEventItems.take(sender());
    m_cmdEventHandlerSystem->removeItem(item);
    emit sigSubscribed(ok, item->getEntityId());
}
