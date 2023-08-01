#include "test_client_entity_subscriber.h"
#include "vcmp_introspectiondata.h"
#include "vf_client_entity_subscriber.h"
#include "vf_cmd_event_handler_system.h"
#include "veintestserver.h"
#include "ve_eventhandler.h"
#include "vf_core_stack_client.h"
#include "vf_test_server_stack.h"
#include "vtcp_workerfactorymethodstest.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_client_entity_subscriber)

static constexpr int systemEntityId = 0;
static constexpr int noneExistentEnitityId = 42;
static constexpr int serverPort = 4242;

void test_client_entity_subscriber::intropectSystemEntitySignalReceived()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);
    
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1).toInt(), systemEntityId);
}

void test_client_entity_subscriber::intropectSystemEntitySignalReceivedNetwork()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1).toInt(), systemEntityId);
}

void test_client_entity_subscriber::intropectSystemEntityTwiceNetwork()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    entityToSubscribe->sendSubscription();
    feedEventLoop();

    // Just for the record:
    // vfnet2 seems not to add additional peer data for multile subscriptions
    QCOMPARE(spy.count(), 2);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1).toInt(), systemEntityId);
    arguments = spy[1];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1).toInt(), systemEntityId);
}

void test_client_entity_subscriber::trySubscribeOnNonExistantEntity()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(noneExistentEnitityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1).toInt(), noneExistentEnitityId);
}

using namespace VeinEvent;
using namespace VeinComponent;

void test_client_entity_subscriber::trySubscribeOnNonExistantEntityTogetherwithOtherError()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();

    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(noneExistentEnitityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();

    // Provoke a component error to same entity
    ComponentData* cData = new ComponentData(noneExistentEnitityId, ComponentData::Command::CCMD_SET);
    cData->setComponentName("foo");
    cData->setCommand(ComponentData::Command::CCMD_SET);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    cData->setNewValue(42);
    cData->setOldValue(44);
    CommandEvent *commandEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData);
    emit cmdEventHandlerSystem.sigSendEvent(commandEvent);
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1).toInt(), noneExistentEnitityId);
}

void test_client_entity_subscriber::introspectComponentNames()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    feedEventLoop();

    QStringList componentNames = entityToSubscribe->getComponentNames();
    QCOMPARE(componentNames.size(), 7);
    QVERIFY(componentNames.contains("EntityName"));
    QVERIFY(componentNames.contains("Session"));
    QVERIFY(componentNames.contains("SessionsAvailable"));
    QVERIFY(componentNames.contains("Entities"));
    QVERIFY(componentNames.contains("ModulesPaused"));
    QVERIFY(componentNames.contains("Error_Messages"));
    QVERIFY(componentNames.contains("LoggedComponents"));

}

using namespace VeinEvent;
using namespace VeinComponent;

void test_client_entity_subscriber::invalidIntrospectionData()
{
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);

    IntrospectionData *iData = new IntrospectionData();
    iData->setEntityId(systemEntityId);
    QJsonObject dummyJsonData;
    iData->setJsonData(dummyJsonData);

    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, iData);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->processCommandEvent(cEvent);
    delete cEvent;

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1).toInt(), systemEntityId);
}

void test_client_entity_subscriber::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}