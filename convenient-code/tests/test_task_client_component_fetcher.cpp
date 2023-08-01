#include "test_task_client_component_fetcher.h"
#include "task_client_component_fetcher.h"
#include "task_client_entity_subscribe.h"
#include "vf_core_stack_client.h"
#include "vf_test_server_stack.h"
#include "vtcp_workerfactorymethodstest.h"
#include "timerfactoryqtfortest.h"
#include "timemachinefortest.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_task_client_component_fetcher)

static constexpr int serverPort = 4242;
static constexpr int systemEntityId = 0;
static constexpr int stdTimeout = 1000;

void test_task_client_component_fetcher::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_task_client_component_fetcher::fetchSubscribed()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityItem);
    feedEventLoop();

    std::shared_ptr<QStringList> components = std::make_shared<QStringList>();
    TaskClientEntitySubscribe taskSubscribe(systemEntityId, clientStack.cmdEventHandlerSystem, components);
    taskSubscribe.start();
    feedEventLoop();

    std::shared_ptr<QVariant> value = std::make_shared<QVariant>();
    TaskTemplatePtr fetcherTask = TaskClientComponentFetcher::create("EntityName", entityItem, value, stdTimeout);
    QSignalSpy spy(fetcherTask.get(), &TaskTemplate::sigFinish);
    fetcherTask->start();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(*value, QVariant("_System"));
}

void test_task_client_component_fetcher::timeout()
{
    VfCoreStackClient clientStack;
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityItem);
    feedEventLoop();

    std::shared_ptr<QVariant> value = std::make_shared<QVariant>();
    TaskTemplatePtr fetcherTask = TaskClientComponentFetcher::create("EntityName", entityItem, value, stdTimeout);

    bool receivedOk = true;
    int timeout = 0;
    connect(fetcherTask.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOk = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });
    fetcherTask->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);
    QVERIFY(!receivedOk);
    QCOMPARE(timeout, stdTimeout);
}

void test_task_client_component_fetcher::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
