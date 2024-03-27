#include "test_task_client_component_setter.h"
#include "task_client_component_setter.h"
#include "task_client_component_fetcher.h"
#include "task_client_entity_subscribe.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithnet.h"
#include "vtcp_workerfactorymethodstest.h"
#include "timerfactoryqtfortest.h"
#include "timemachinefortest.h"
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_task_client_component_setter)

static constexpr int serverPort = 4242;
static constexpr int testId = 2;
static constexpr int stdTimeout = 1000;

void test_task_client_component_setter::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_task_client_component_setter::setValidValueSubscribed()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    
    TestVeinServerWithNet serverNet(serverPort);
    VfCpp::VfCppEntity serverAdditionalEntity(testId);
    serverNet.getEventHandler()->addSubsystem(&serverAdditionalEntity);
    serverAdditionalEntity.initModule();
    serverAdditionalEntity.createComponent("foo", 42);
    TimeMachineObject::feedEventLoop();

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(testId);
    clientStack.cmdEventHandlerSystem->addItem(entityItem);
    TimeMachineObject::feedEventLoop();

    std::shared_ptr<QStringList> components = std::make_shared<QStringList>();
    TaskClientEntitySubscribe taskSubscribe(testId, clientStack.cmdEventHandlerSystem, components);
    taskSubscribe.start();
    TimeMachineObject::feedEventLoop();

    TaskTemplatePtr setterTask = TaskClientComponentSetter::create(entityItem, "foo", 42, 40, stdTimeout);
    QSignalSpy spy(setterTask.get(), &TaskTemplate::sigFinish);
    setterTask->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0].toBool(), true);

    std::shared_ptr<QVariant> value = std::make_shared<QVariant>();
    TaskTemplatePtr fetcherTask = TaskClientComponentFetcher::create("foo", entityItem, value, stdTimeout);
    fetcherTask->start();
    TimeMachineObject::feedEventLoop();
    QCOMPARE(*value, QVariant(40));
}

void test_task_client_component_setter::timeout()
{
    VfCoreStackClient clientStack;
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(testId);
    clientStack.cmdEventHandlerSystem->addItem(entityItem);
    TimeMachineObject::feedEventLoop();

    TaskTemplatePtr setterTask = TaskClientComponentSetter::create(entityItem, "foo", 42, 40, stdTimeout);

    bool receivedOk = true;
    int timeout = 0;
    connect(setterTask.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOk = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });
    setterTask->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);
    QVERIFY(!receivedOk);
    QCOMPARE(timeout, stdTimeout);
}
