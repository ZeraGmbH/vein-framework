#include "test_storage_recorder.h"
#include "testveinserverwithmocknet.h"
#include "vs_databasehash.h"
#include "vs_storagerecorder.h"
#include <timerfactoryqtfortest.h>
#include <timemachineobject.h>
#include <timemachinefortest.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_storage_recorder)

static constexpr int serverPort = 4242;

static constexpr int entityIdWithSignal = 10;
static constexpr int entityIdWithValue1 = 20;
static constexpr int entityIdWithValue2 = 30;
static const char* componentName1 = "component1";
static const char* componentName2 = "component2";
static const char* labelName1 = "label1";
static const char* labelName2 = "label2";

void test_storage_recorder::initTestCase()
{
    TimerFactoryQtForTest::enableTest();
}

void test_storage_recorder::init()
{
    TimeMachineForTest::reset();
    m_server = std::make_unique<TestVeinServerWithMockNet>(serverPort);
    createEntity(entityIdWithSignal);
    createEntity(entityIdWithValue1);
    createEntity(entityIdWithValue2);
}

void test_storage_recorder::cleanup()
{
    m_server.reset();
    TimeMachineObject::feedEventLoop();
}


void test_storage_recorder::checkValidComponentInfo()
{
    RecordEntityComponents components = createStandardRecorderComponents();
    DatabaseHash db;
    StorageRecorder recorder(components, &db,
                             entityIdWithSignal, "Signal");

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_componentInfo, components);
}

void test_storage_recorder::oneRecordValidComponentInfo()
{
    createSignalVeinComponent();

    RecordEntityComponents components = createStandardRecorderComponents();
    StorageRecorder recorder(components, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    triggerMeasSignal();
    recorder.stopLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_componentInfo, components);
}

void test_storage_recorder::serializeComponentInfo()
{
    RecordEntityComponents components = createStandardRecorderComponents();
    RecordEntityComponentSequence sequence = RecordParamSequencer::toSequence(components);

    QCOMPARE(sequence.count(), 4);
    QCOMPARE(sequence[0].m_entityId, entityIdWithValue1);
    QCOMPARE(sequence[0].m_component.m_componentName, componentName1);
    QCOMPARE(sequence[0].m_component.m_label, labelName1);

    QCOMPARE(sequence[1].m_entityId, entityIdWithValue1);
    QCOMPARE(sequence[1].m_component.m_componentName, componentName2);
    QCOMPARE(sequence[1].m_component.m_label, labelName2);

    QCOMPARE(sequence[2].m_entityId, entityIdWithValue2);
    QCOMPARE(sequence[2].m_component.m_componentName, componentName1);
    QCOMPARE(sequence[2].m_component.m_label, labelName1);

    QCOMPARE(sequence[3].m_entityId, entityIdWithValue2);
    QCOMPARE(sequence[3].m_component.m_componentName, componentName2);
    QCOMPARE(sequence[3].m_component.m_label, labelName2);
}

void test_storage_recorder::emptyCheckCount()
{
    RecordEntityComponents emptyComponents;
    DatabaseHash db;
    StorageRecorder recorder(emptyComponents, &db,
                             entityIdWithSignal, "Signal");

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_recordedData.count(), 0);
}

void test_storage_recorder::oneRecordCheckCount()
{
    createSignalVeinComponent();

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    triggerMeasSignal();
    recorder.stopLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_recordedData.count(), 1);
}

void test_storage_recorder::oneRecordPerTwoStartStopCheckCount()
{
    createSignalVeinComponent();

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    triggerMeasSignal();
    recorder.stopLogging();

    recorder.startLogging();
    triggerMeasSignal();
    recorder.stopLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_recordedData.count(), 1);
}

void test_storage_recorder::twoRecordCheckCount()
{
    createSignalVeinComponent();

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    triggerMeasSignal();
    triggerMeasSignal();
    recorder.stopLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_recordedData.count(), 2);
}

void test_storage_recorder::twoRecordRestartCheckCount()
{
    createSignalVeinComponent();

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    recorder.startLogging();
    triggerMeasSignal();
    triggerMeasSignal();
    QCOMPARE(recorded->m_recordedData.count(), 2);
    // no stop

    recorder.startLogging();
    QCOMPARE(recorded->m_recordedData.count(), 0);
    triggerMeasSignal();
    triggerMeasSignal();
    QCOMPARE(recorded->m_recordedData.count(), 2);
}

void test_storage_recorder::oneRecordCheckData()
{
    createStandardVeinComponents();

    RecordEntityComponents components = createStandardRecorderComponents();
    StorageRecorder recorder(components, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    setValueComponentsByVein(42);
    triggerMeasSignal();
    recorder.stopLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    const RecordListStored &recordedData = recorded->m_recordedData;
    QCOMPARE(recordedData.count(), 1);
    QCOMPARE(recordedData[0].m_values.count(), 4); // 2 entities * 2 components
    QCOMPARE(recordedData[0].m_values[0], 42);
    QCOMPARE(recordedData[0].m_values[1], 43);
    QCOMPARE(recordedData[0].m_values[2], 44);
    QCOMPARE(recordedData[0].m_values[3], 45);
}

void test_storage_recorder::oneRecordPerTwoStartStopCheckData()
{
    createStandardVeinComponents();

    RecordEntityComponents components = createStandardRecorderComponents();
    StorageRecorder recorder(components, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    setValueComponentsByVein(42);
    triggerMeasSignal();
    recorder.stopLogging();

    recorder.startLogging();
    setValueComponentsByVein(37);
    triggerMeasSignal();
    recorder.stopLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    const RecordListStored &recordedData = recorded->m_recordedData;
    QCOMPARE(recordedData.count(), 1);
    QCOMPARE(recordedData[0].m_values.count(), 4); // 2 entities * 2 components
    QCOMPARE(recordedData[0].m_values[0], 37);
    QCOMPARE(recordedData[0].m_values[1], 38);
    QCOMPARE(recordedData[0].m_values[2], 39);
    QCOMPARE(recordedData[0].m_values[3], 40);
}

void test_storage_recorder::initialTimestampFirst()
{
    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");
    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_timeOfFirstRecord, QDateTime());
}

void test_storage_recorder::oneRecordTimestampFirst()
{
    QVariantMap veinComponentsSignal = {{"Signal", QVariant(1)}};
    createComponents(entityIdWithSignal, veinComponentsSignal);

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    TimeMachineForTest::getInstance()->processTimers(500);
    triggerMeasSignal();
    recorder.stopLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_timeOfFirstRecord.toMSecsSinceEpoch(), 500);
}

void test_storage_recorder::oneRecordStartNoSigTimestampFirst()
{
    QVariantMap veinComponentsSignal = {{"Signal", QVariant(1)}};
    createComponents(entityIdWithSignal, veinComponentsSignal);

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    TimeMachineForTest::getInstance()->processTimers(500);
    triggerMeasSignal();
    recorder.stopLogging();
    recorder.startLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_timeOfFirstRecord, QDateTime());
}

void test_storage_recorder::twoRecordTimestampFirst()
{
    QVariantMap veinComponentsSignal = {{"Signal", QVariant(1)}};
    createComponents(entityIdWithSignal, veinComponentsSignal);

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    TimeMachineForTest::getInstance()->processTimers(500);
    triggerMeasSignal();
    TimeMachineForTest::getInstance()->processTimers(500);
    triggerMeasSignal();
    recorder.stopLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_timeOfFirstRecord.toMSecsSinceEpoch(), 500);
}

void test_storage_recorder::oneRecordStartSigTimestampFirst()
{
    QVariantMap veinComponentsSignal = {{"Signal", QVariant(1)}};
    createComponents(entityIdWithSignal, veinComponentsSignal);

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    TimeMachineForTest::getInstance()->processTimers(500);
    triggerMeasSignal();
    recorder.stopLogging();

    recorder.startLogging();
    TimeMachineForTest::getInstance()->processTimers(500);
    triggerMeasSignal();
    recorder.stopLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_timeOfFirstRecord.toMSecsSinceEpoch(), 1000);
}

void test_storage_recorder::oneRecordCheckTimestamp()
{
    QVariantMap veinComponents = {{"Signal", QVariant(1)}};
    createComponents(entityIdWithSignal, veinComponents);

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    triggerMeasSignal();
    recorder.stopLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_recordedData.count(), 1);
    QCOMPARE(recorded->m_recordedData[0].m_timeDiffToFirstInMs, 0);
}

void test_storage_recorder::threeSigCheckTimestamps()
{
    QVariantMap veinComponents = {{"Signal", QVariant(1)}};
    createComponents(entityIdWithSignal, veinComponents);

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");

    recorder.startLogging();
    TimeMachineForTest::getInstance()->processTimers(500);
    triggerMeasSignal();
    TimeMachineForTest::getInstance()->processTimers(400);
    triggerMeasSignal();
    TimeMachineForTest::getInstance()->processTimers(300);
    triggerMeasSignal();
    recorder.stopLogging();

    StorageRecordDataPtr recorded = recorder.getRecordedData();
    QCOMPARE(recorded->m_recordedData.count(), 3);
    QCOMPARE(recorded->m_recordedData[0].m_timeDiffToFirstInMs, 0);
    QCOMPARE(recorded->m_recordedData[1].m_timeDiffToFirstInMs, 400);
    QCOMPARE(recorded->m_recordedData[2].m_timeDiffToFirstInMs, 400+300);
}

void test_storage_recorder::oneRecordCheckSignal()
{
    QVariantMap veinComponents = {{"Signal", QVariant(1)}};
    createComponents(entityIdWithSignal, veinComponents);

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");
    QSignalSpy spy(&recorder, &StorageRecorder::sigRecordCountChanged);

    recorder.startLogging();
    QCOMPARE(spy.count(), 0);

    triggerMeasSignal();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], 1);

    recorder.stopLogging();
    QCOMPARE(spy.count(), 1);
}

void test_storage_recorder::recordSequenceCheckSignal()
{
    QVariantMap veinComponents = {{"Signal", QVariant(1)}};
    createComponents(entityIdWithSignal, veinComponents);

    RecordEntityComponents emptyComponents;
    StorageRecorder recorder(emptyComponents, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");
    QSignalSpy spy(&recorder, &StorageRecorder::sigRecordCountChanged);

    recorder.startLogging();
    QCOMPARE(spy.count(), 0);

    triggerMeasSignal();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], 1);

    recorder.stopLogging();
    QCOMPARE(spy.count(), 1);

    recorder.startLogging();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy[1][0], 0);

    triggerMeasSignal();
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy[2][0], 1);

    triggerMeasSignal();
    QCOMPARE(spy.count(), 4);
    QCOMPARE(spy[3][0], 2);
}

void test_storage_recorder::createEntity(int entityId)
{
    m_server->getServer()->addEntity(entityId, QString("Entity%1").arg(entityId));
}

void test_storage_recorder::createSignalVeinComponent()
{
    QVariantMap veinComponents = {{"Signal", QVariant(1)}};
    createComponents(entityIdWithSignal, veinComponents);
}

void test_storage_recorder::createStandardVeinComponents()
{
    createSignalVeinComponent();
    QVariantMap veinComponentsValues = {{componentName1, QVariant()}, {componentName2, QVariant()}};
    createComponents(entityIdWithValue1, veinComponentsValues);
    createComponents(entityIdWithValue2, veinComponentsValues);
}

void test_storage_recorder::createComponents(int entityId, QMap<QString, QVariant> components)
{
    for(const auto &componentName : components.keys())
        m_server->getServer()->addComponent(entityId, componentName, components[componentName], false);
    TimeMachineObject::feedEventLoop();
}

void test_storage_recorder::setComponent(int entityId, const QString &componentName, const QVariant &newValue)
{
    m_server->getServer()->setComponentServerNotification(entityId, componentName, newValue);
}

void test_storage_recorder::triggerMeasSignal()
{
    setComponent(entityIdWithSignal, "Signal", 0);
    setComponent(entityIdWithSignal, "Signal", 1);
}

void test_storage_recorder::setValueComponentsByVein(int startValue)
{
    const QList<int> entityIds = QList<int>() << entityIdWithValue1 << entityIdWithValue2;
    const QStringList componentNames = QStringList() << componentName1 << componentName2;
    int currentValue = startValue;
    for (int entityId : entityIds) {
        for (const QString &componentName : componentNames) {
            setComponent(entityId, componentName, float(currentValue));
            ++currentValue;
        }
    }
}

RecordEntityComponents test_storage_recorder::createStandardRecorderComponents() const
{
    RecordEntityComponents components;
    components.insert(entityIdWithValue1,
                      QList<RecordComponent>() <<
                          RecordComponent{componentName1, labelName1} <<
                          RecordComponent{componentName2, labelName2});
    components.insert(entityIdWithValue2,
                      QList<RecordComponent>() <<
                          RecordComponent{componentName1, labelName1} <<
                          RecordComponent{componentName2, labelName2});
    StorageRecorder recorder(components, m_server->getStorage()->getDb(),
                             entityIdWithSignal, "Signal");
    return components;
}

