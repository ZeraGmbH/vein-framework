#include "test_storage_recorder_qvariant_wrappper.h"
#include "vs_storagerecorderqvariantwrappper.h"
#include "testveinserverwithmocknet.h"
#include "vf_core_stack_client.h"
#include <QTest>

using namespace VeinStorage;

QTEST_MAIN(test_storage_recorder_qvariant_wrappper)

void test_storage_recorder_qvariant_wrappper::emptyMap()
{
    StorageRecordDataPtr recordData = std::make_shared<StorageRecordData>();
    StorageRecorderQVariantWrappper wrapper(recordData);
    QVariant variant(wrapper);
    QCOMPARE(variant, QVariantMap());
    QCOMPARE(variant.toMap(), QVariantMap());
}

void test_storage_recorder_qvariant_wrappper::emptyMapToWrapperChangeCopyVariant()
{
    StorageRecordDataPtr recordData = std::make_shared<StorageRecordData>();
    StorageRecorderQVariantWrappper wrapper(recordData);

    QVariant variant(wrapper);
    recordData->m_recordedData.append(StorageRecordEntry{0, QVector<float>() << 42});

    QVariant variantCopy = variant;
    QCOMPARE(variantCopy, QVariantMap());
    QCOMPARE(variantCopy.toMap(), QVariantMap());
}

static constexpr int serverPort = 4242;
static constexpr int testEntityId = 37;
static const char* entityName = "TestEntity";
static const char* testComponentName = "TestComponent";

void test_storage_recorder_qvariant_wrappper::emptyMapInStorage()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);

    VeinStorage::AbstractStorageWritable* storage = serverNet.getStorageWritable();
    AbstractDatabaseDirectWrite *writableDb = storage->getDbWritable();
    StorageComponentPtr future = writableDb->getFutureComponentForWrite(testEntityId, testComponentName);

    StorageRecordDataPtr recordData = std::make_shared<StorageRecordData>();
    StorageRecorderQVariantWrappper wrapper(recordData);
    future->getValueForWrite() = QVariant(wrapper);

    QCOMPARE(writableDb->getFutureComponent(testEntityId, testComponentName)->getValue(), QVariantMap());
    QCOMPARE(writableDb->getFutureComponent(testEntityId, testComponentName)->getValue().toMap(), QVariantMap());
}

void test_storage_recorder_qvariant_wrappper::emptyMapToStorageAddValueFetchVein()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);

    VeinStorage::AbstractStorageWritable* storage = serverNet.getStorageWritable();
    AbstractDatabaseDirectWrite *writableDb = storage->getDbWritable();
    StorageComponentPtr future = writableDb->getFutureComponentForWrite(testEntityId, testComponentName);

    StorageRecordDataPtr recordData = std::make_shared<StorageRecordData>();
    StorageRecorderQVariantWrappper wrapper(recordData);
    future->getValueForWrite() = QVariant(wrapper);

    QCOMPARE(writableDb->getFutureComponent(testEntityId, testComponentName)->getValue(), QVariantMap());
    QCOMPARE(writableDb->getFutureComponent(testEntityId, testComponentName)->getValue().toMap(), QVariantMap());

    /*VfCoreStackClient clientNet(VeinTcp::MockTcpNetworkFactory::create());
    clientNet.connectToServer("127.0.0.1", serverPort);
    clientNet.subscribeEntity(testEntityId);
    TimeMachineObject::feedEventLoop();

    QJsonObject jsonEvents;
    setupSpy(serverNet, clientNet, jsonEvents);

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(testEntityId);
    clientNet.addItem(entityItem);
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create(testComponentName, entityItem);
    entityItem->addItem(fetcher);
    fetcher->startGetComponent();
    TimeMachineObject::feedEventLoop();

    QByteArray expected = TestLogHelpers::loadFile(":/vein-storage-events/add-future-add-vein-fetch.json");
    QByteArray dumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(expected, dumped));*/
}
