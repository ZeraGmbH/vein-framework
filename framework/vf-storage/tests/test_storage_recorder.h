#ifndef TEST_STORAGE_RECORDER_H
#define TEST_STORAGE_RECORDER_H

#include "testveinserverwithmocknet.h"
#include "vs_storagerecordertypes.h"
#include <memory>

using namespace VfCpp;
using namespace VeinStorage;

class test_storage_recorder : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void init();
    void cleanup();

    void checkValidComponentInfo();
    void oneRecordValidComponentInfo();
    void serializeComponentInfo();

    void emptyCheckCount();
    void oneRecordCheckCount();
    void oneRecordPerTwoStartStopCheckCount();
    void twoRecordCheckCount();
    void twoRecordRestartCheckCount();

    void oneRecordCheckData();
    void oneRecordPerTwoStartStopCheckData();

    void initialTimestampFirst();
    void oneRecordTimestampFirst();
    void oneRecordStartNoSigTimestampFirst();
    void twoRecordTimestampFirst();
    void oneRecordStartSigTimestampFirst();

    void oneRecordCheckTimestamp();
    void threeSigCheckTimestamps();

    void oneRecordCheckSignal();
    void recordSequenceCheckSignal();

    // restart

private:
    void createEntity(int entityId);
    void createStandardVeinComponents();
    void createComponents(int entityId, QMap<QString, QVariant> components);
    void setComponent(int entityId, const QString &componentName, const QVariant &newValue);
    void triggerMeasSignal();
    void setValueComponentsByVein(int startValue);
    RecordEntityComponents createStandardRecorderComponents() const;

    std::shared_ptr<TestVeinServerWithMockNet> m_server;
    void createSignalVeinComponent();
};

#endif // TEST_STORAGE_RECORDER_H
