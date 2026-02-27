#include "storagerecorderinterpolation.h"
#include "vs_storagerecorder.h"
#include <timerfactoryqt.h>

StorageRecorderInterpolation::StorageRecorderInterpolation(const RecordEntityComponents &componentsRecorded, std::shared_ptr<AbstractStorageRecorder> recorder, int factor) :
    m_recordData(std::make_shared<StorageRecordData>(componentsRecorded)),
    m_recorder(recorder),
    m_factor(factor)
{
    connect(m_recorder.get(), &StorageRecorder::sigRecordCountChanged, this, &StorageRecorderInterpolation::onNewValueRecorded);
}

StorageRecordDataPtr StorageRecorderInterpolation::getRecordedData() const
{
    return m_recordData;
}

void StorageRecorderInterpolation::onNewValueRecorded(int count)
{
    if(count == 0) {
        m_recordData->resetRecords();
        m_count = 0;
    }

    else if(count % m_factor == 0) {
        if (count <= m_count)
            return;  // ignore backwards or duplicate signals
        StorageRecordDataPtr storageRecorder= m_recorder.get()->getRecordedData();
        calculateJsonAverage(storageRecorder, m_count, count);
        m_count = count;
    }
}

void StorageRecorderInterpolation::calculateJsonAverage(StorageRecordDataPtr storageRecorder, int startIdx, int endIdx)
{
    QList<int> timeDiffList;
    QList<QVector<float>> valuesList;

    for(int i=startIdx; i<endIdx; i++) {
        RecordEntryStored entryStored = storageRecorder->m_recordedData.at(i);
        timeDiffList.append(entryStored.m_timeDiffToFirstInMs);
        valuesList.append(entryStored.m_values);
    }
    int timeDiffAverage = calculateTimeDiffAverage(timeDiffList);
    QVector<float> valuesAverage = calculateValuesAverage(valuesList);
    appendToRecordData(timeDiffAverage, valuesAverage);
}

int StorageRecorderInterpolation::calculateTimeDiffAverage(QList<int> timeDiffList)
{
    int addedVals = 0;
    for(int timeDiffToFirstInMs : timeDiffList)
        addedVals+= timeDiffToFirstInMs;
    return static_cast<int>(addedVals/m_factor);
}

QVector<float> StorageRecorderInterpolation::calculateValuesAverage(QList<QVector<float> > valuesList)
{
    QVector<float> result;
    if(valuesList.isEmpty())
        return result;
    int size = valuesList.first().size();
    result = QVector<float>(size, 0.0f);

    for(const QVector<float> &valuesVector : valuesList) {
        for (int i = 0; i <size; ++i)
            result[i] += valuesVector[i];
    }
    for (int i = 0; i<result.count(); ++i)
        result[i] /= valuesList.size();

    return result;
}

void StorageRecorderInterpolation::appendToRecordData(int timeDiff, QVector<float> values)
{
    RecordEntryStored entryStored;
    entryStored.m_timeDiffToFirstInMs = timeDiff;
    entryStored.m_values = values;

    m_recordData->m_recordedData.append(entryStored);
    if(m_recordData->m_timeOfFirstRecord == QDateTime())
        m_recordData->m_timeOfFirstRecord = TimerFactoryQt::getCurrentTime();

    emit sigRecordCountChanged(m_recordData->m_recordedData.count());
}
