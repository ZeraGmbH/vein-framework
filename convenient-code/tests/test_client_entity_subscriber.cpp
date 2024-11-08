#include "test_client_entity_subscriber.h"
#include "vcmp_introspectiondata.h"
#include "vf_client_entity_subscriber.h"
#include "vf_cmd_event_handler_system.h"
#include "testveinserver.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpnetworkfactory.h"
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_client_entity_subscriber)

static constexpr int systemEntityId = 0;
static constexpr int noneExistentEnitityId = 42;
static constexpr int serverPort = 4242;

void test_client_entity_subscriber::intropectSystemEntitySignalReceived()
{
    TestVeinServer testServer;
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    testServer.appendEventSystem(&cmdEventHandlerSystem);
    TimeMachineObject::feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);
    
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1).toInt(), systemEntityId);
}

void test_client_entity_subscriber::intropectSystemEntitySignalReceivedNetwork()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);
    clientStack.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1).toInt(), systemEntityId);
}

void test_client_entity_subscriber::intropectSystemEntityTwiceNetwork()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);
    clientStack.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    entityToSubscribe->sendSubscription();
    TimeMachineObject::feedEventLoop();

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
    TestVeinServer testServer;
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    testServer.appendEventSystem(&cmdEventHandlerSystem);
    TimeMachineObject::feedEventLoop();
    
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(noneExistentEnitityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1).toInt(), noneExistentEnitityId);
}

using namespace VeinEvent;
using namespace VeinComponent;

void test_client_entity_subscriber::trySubscribeOnNonExistantEntityTogetherwithOtherError()
{
    TestVeinServer testServer;
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    testServer.appendEventSystem(&cmdEventHandlerSystem);
    TimeMachineObject::feedEventLoop();

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
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1).toInt(), noneExistentEnitityId);
}

void test_client_entity_subscriber::introspectComponentNames()
{
    TestVeinServer testServer;
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    testServer.appendEventSystem(&cmdEventHandlerSystem);
    TimeMachineObject::feedEventLoop();
    
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    TimeMachineObject::feedEventLoop();

    QStringList componentNames = entityToSubscribe->getComponentNames();
    QVERIFY(componentNames.contains("EntityName"));
    QVERIFY(componentNames.contains("Session"));
    QVERIFY(componentNames.contains("SessionsAvailable"));
    QVERIFY(componentNames.contains("Entities"));
    QVERIFY(componentNames.contains("ModulesPaused"));
    QVERIFY(componentNames.contains("DevMode"));
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
