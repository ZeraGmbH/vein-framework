#include "test_component_change_notifier.h"
#include "vf_component_change_notifier.h"
#include "vf_cmd_event_handler.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithnet.h"
#include "vtcp_workerfactorymethodstest.h"
#include <vcmp_componentdata.h>
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_component_change_notifier)

using namespace VeinEvent;
using namespace VeinComponent;

static constexpr int systemEntityId = 0;
static constexpr int testEntityId = 1;
static const char* componentName = "foo";
static constexpr int serverPort = 4242;

void test_component_change_notifier::ignoreOtherCommandsThanSet()
{
    VfCmdEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(testEntityId);
    commandEventHandler.addItem(entityItem);
    VfSimpleChangeNotifierPtr changeNotifier = VfComponentChangeNotifier::create(componentName, entityItem);
    entityItem->addItem(changeNotifier);

    ComponentData* cData1 = new ComponentData(testEntityId, ComponentData::Command::CCMD_ADD);
    cData1->setComponentName(componentName);
    CommandEvent *commandEvent1 = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData1);
    
    QSignalSpy spy1(changeNotifier.get(), &VfComponentChangeNotifier::sigValueChanged);
    commandEventHandler.processEvent(commandEvent1);
    QCOMPARE(spy1.count(), 0);
    delete commandEvent1;

    ComponentData* cData2 = new ComponentData(testEntityId, ComponentData::Command::CCMD_FETCH);
    cData2->setComponentName(componentName);
    CommandEvent *commandEvent2 = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData2);
    
    QSignalSpy spy2(changeNotifier.get(), &VfComponentChangeNotifier::sigValueChanged);
    commandEventHandler.processEvent(commandEvent2);
    QCOMPARE(spy2.count(), 0);
    delete commandEvent2;
}

void test_component_change_notifier::notifySet()
{
    VfCmdEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(testEntityId);
    commandEventHandler.addItem(entityItem);
    VfSimpleChangeNotifierPtr changeNotifier = VfComponentChangeNotifier::create(componentName, entityItem);
    entityItem->addItem(changeNotifier);

    ComponentData* cData = new ComponentData(testEntityId, ComponentData::Command::CCMD_SET);
    cData->setComponentName(componentName);
    CommandEvent *commandEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData);
    
    QSignalSpy spy(changeNotifier.get(), &VfComponentChangeNotifier::sigValueChanged);
    commandEventHandler.processEvent(commandEvent);
    QCOMPARE(spy.count(), 1);
    delete commandEvent;
}

void test_component_change_notifier::inClientServerStack()
{
    // server
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    TestVeinServerWithNet serverNet(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfCpp::VfCppEntity serverAdditionalEntity(testEntityId);
    serverNet.getEventHandler()->addSubsystem(&serverAdditionalEntity);
    serverAdditionalEntity.initModule();
    serverAdditionalEntity.createComponent(componentName, 42);
    TimeMachineObject::feedEventLoop();

    // client
    clientStack.subscribeEntity(systemEntityId);
    clientStack.subscribeEntity(testEntityId);
    TimeMachineObject::feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(testEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityItem);
    VfSimpleChangeNotifierPtr changeNotifier = VfComponentChangeNotifier::create(componentName, entityItem);
    entityItem->addItem(changeNotifier);
    QSignalSpy spy(changeNotifier.get(), &VfComponentChangeNotifier::sigValueChanged);

    // Send set event manually to reduce dependencies
    ComponentData* cData = new ComponentData(testEntityId, ComponentData::Command::CCMD_SET);
    cData->setComponentName(componentName);
    cData->setOldValue(42);
    cData->setNewValue(44);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    CommandEvent *commandEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
    QCoreApplication::instance()->postEvent(&clientStack.eventHandler, commandEvent);

    TimeMachineObject::feedEventLoop();
    QCOMPARE(spy.count(), 1);
    QVariant notifierVal = changeNotifier->getValue();
    QCOMPARE(notifierVal, QVariant(44));
}
