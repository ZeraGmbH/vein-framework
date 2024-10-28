#include "test_task_client_component_fetcher.h"
#include "task_client_component_fetcher.h"
#include "task_client_entity_subscribe.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpnetworkfactory.h"
#include "timerfactoryqtfortest.h"
#include "timemachinefortest.h"
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
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(systemEntityId);
    clientStack.addItem(entityItem);
    TimeMachineObject::feedEventLoop();

    std::shared_ptr<QStringList> components = std::make_shared<QStringList>();
    TaskClientEntitySubscribe taskSubscribe(systemEntityId, clientStack.getCmdEventHandlerSystem(), components);
    taskSubscribe.start();
    TimeMachineObject::feedEventLoop();

    std::shared_ptr<QVariant> value = std::make_shared<QVariant>();
    TaskTemplatePtr fetcherTask = TaskClientComponentFetcher::create("EntityName", entityItem, value, stdTimeout);
    QSignalSpy spy(fetcherTask.get(), &TaskTemplate::sigFinish);
    fetcherTask->start();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(*value, QVariant("_System"));
}

void test_task_client_component_fetcher::timeout()
{
    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(systemEntityId);
    clientStack.addItem(entityItem);
    TimeMachineObject::feedEventLoop();

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
