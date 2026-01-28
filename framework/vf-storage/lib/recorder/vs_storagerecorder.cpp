#include "vs_storagerecorder.h"
#include <timerfactoryqt.h>

namespace VeinStorage
{

StorageRecorder::StorageRecorder(const RecordEntityComponents &componentsRecorded,
                                 AbstractDatabase *storage,
                                 int entityIdTrigger, const QString &componentTrigger, const QVariant &triggerVal) :
    m_recordData(std::make_shared<StorageRecordData>(componentsRecorded)),
    m_triggerComponent(storage->getFutureComponent(entityIdTrigger, componentTrigger)),
    m_triggerVal(triggerVal)
{
    setupVeinLoggedComponents(componentsRecorded, storage);
}

void StorageRecorder::setupVeinLoggedComponents(const RecordEntityComponents &componentsRecorded, AbstractDatabase *storage)
{
    const RecordEntityComponentSequence sequence = RecordParamSequencer::toSequence(componentsRecorded);
    for (const RecordEntityComponent &entry : sequence)
        m_veinValuesRecorded.append(storage->getFutureComponent(entry.m_entityId,
                                                                entry.m_component.m_componentName));
}

void StorageRecorder::startLogging()
{
    stopLogging();
    connect(m_triggerComponent.get(), &AbstractComponent::sigValueChange,
            this, &StorageRecorder::onTrigger);

    if (m_recordData->m_recordedData.count() > 0) {
        m_recordData->resetRecords();
        emit sigRecordCountChanged(0);
    }
}

void StorageRecorder::stopLogging()
{
    disconnect(m_triggerComponent.get(), &AbstractComponent::sigValueChange,
               this, &StorageRecorder::onTrigger);
}

StorageRecordDataPtr StorageRecorder::getRecordedData() const
{
    return m_recordData;
}

RecordEntryStored StorageRecorder::createRecordEntry()
{
    RecordEntryStored entry;

    qint64 msFirst = m_recordData->m_timeOfFirstRecord.toMSecsSinceEpoch();
    qint64 msNow = TimerFactoryQt::getCurrentTime().toMSecsSinceEpoch();
    qint64 msDiff = msNow - msFirst;
    entry.m_timeDiffToFirstInMs = msDiff;

    QVector<float> values(m_veinValuesRecorded.count());
    for (int i=0; i<m_veinValuesRecorded.count(); ++i)
        values[i] = m_veinValuesRecorded[i]->getValue().toFloat();
    entry.m_values = values;

    return entry;
}

void StorageRecorder::onTrigger(const QVariant &value)
{
    if (value == m_triggerVal) {
        if(m_recordData->m_timeOfFirstRecord == QDateTime())
            m_recordData->m_timeOfFirstRecord = TimerFactoryQt::getCurrentTime();

        m_recordData->m_recordedData.append(createRecordEntry());
        emit sigRecordCountChanged(m_recordData->m_recordedData.count());
    }
}

}
