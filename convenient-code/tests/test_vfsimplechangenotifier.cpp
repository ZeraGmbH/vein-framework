#include "test_vfsimplechangenotifier.h"
#include "vfsimplechangenotifier.h"
#include "vfcommandeventhandler.h"
#include "vftestserverstack.h"
#include "vftestclientstack.h"
#include "vtcp_workerfactorymethodstest.h"
#include <vcmp_componentdata.h>
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_vfsimplechangenotifier)

using namespace VeinEvent;
using namespace VeinComponent;

static constexpr int systemEntityId = 0;
static constexpr int testEntityId = 1;
static const char* componentName = "foo";
static constexpr int serverPort = 4242;

void test_vfsimplechangenotifier::ignoreOtherCommandsThanSet()
{
    VfCommandEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(testEntityId);
    commandEventHandler.addItem(entityItem);
    VfSimpleChangeNotifierPtr changeNotifier = VfSimpleChangeNotifier::create(componentName, entityItem);
    entityItem->addItem(changeNotifier);

    ComponentData* cData1 = new ComponentData(testEntityId, ComponentData::Command::CCMD_ADD);
    cData1->setComponentName(componentName);
    CommandEvent *commandEvent1 = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData1);

    QSignalSpy spy1(changeNotifier.get(), &VfSimpleChangeNotifier::sigValueChanged);
    commandEventHandler.processEvent(commandEvent1);
    QCOMPARE(spy1.count(), 0);
    delete commandEvent1;

    ComponentData* cData2 = new ComponentData(testEntityId, ComponentData::Command::CCMD_FETCH);
    cData2->setComponentName(componentName);
    CommandEvent *commandEvent2 = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData2);

    QSignalSpy spy2(changeNotifier.get(), &VfSimpleChangeNotifier::sigValueChanged);
    commandEventHandler.processEvent(commandEvent2);
    QCOMPARE(spy2.count(), 0);
    delete commandEvent2;
}

void test_vfsimplechangenotifier::notifySet()
{
    VfCommandEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(testEntityId);
    commandEventHandler.addItem(entityItem);
    VfSimpleChangeNotifierPtr changeNotifier = VfSimpleChangeNotifier::create(componentName, entityItem);
    entityItem->addItem(changeNotifier);

    ComponentData* cData = new ComponentData(testEntityId, ComponentData::Command::CCMD_SET);
    cData->setComponentName(componentName);
    CommandEvent *commandEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData);

    QSignalSpy spy(changeNotifier.get(), &VfSimpleChangeNotifier::sigValueChanged);
    commandEventHandler.processEvent(commandEvent);
    QCOMPARE(spy.count(), 1);
    delete commandEvent;
}

void test_vfsimplechangenotifier::inClientServerStack()
{
    // server
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);
    VfTestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);

    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    VfCpp::VfCppEntity serverAdditionalEntity(testEntityId);
    serverStack.eventHandler.addSubsystem(&serverAdditionalEntity);
    serverAdditionalEntity.initModule();
    serverAdditionalEntity.createComponent(componentName, 42);
    feedEventLoop();

    // client
    clientStack.subscribeEntityId(systemEntityId, &cmdEventHandlerSystem);
    clientStack.subscribeEntityId(testEntityId, &cmdEventHandlerSystem);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(testEntityId);
    cmdEventHandlerSystem.addItem(entityItem);
    VfSimpleChangeNotifierPtr changeNotifier = VfSimpleChangeNotifier::create(componentName, entityItem);
    entityItem->addItem(changeNotifier);
    QSignalSpy spy(changeNotifier.get(), &VfSimpleChangeNotifier::sigValueChanged);

    // Send set event manually to reduce dependencies
    ComponentData* cData = new ComponentData(testEntityId, ComponentData::Command::CCMD_SET);
    cData->setComponentName(componentName);
    cData->setOldValue(42);
    cData->setNewValue(44);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    CommandEvent *commandEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
    QCoreApplication::instance()->postEvent(&clientStack.eventHandler, commandEvent);

    feedEventLoop();
    QCOMPARE(spy.count(), 1);
    QVariant notifierVal = changeNotifier->getValue();
    QCOMPARE(notifierVal, QVariant(44));
}

void test_vfsimplechangenotifier::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
