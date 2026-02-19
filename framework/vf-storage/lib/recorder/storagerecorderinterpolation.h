#ifndef STORAGERECORDERINTERPOLATION_H
#define STORAGERECORDERINTERPOLATION_H

#include "abstractstoragerecorder.h"
#include "vs_storagerecorder.h"

class StorageRecorderInterpolation : public AbstractStorageRecorder
{
public:
    StorageRecorderInterpolation(const RecordEntityComponents &componentsRecorded, std::shared_ptr<StorageRecorder> recorder);
    StorageRecordDataPtr getRecordedData() const override;

private slots:
    void onNewValueRecorded(int count);
private:
    void calculateJsonAverage(StorageRecordDataPtr storageRecorder, int startIdx, int endIdx);
    int calculateTimeDiffAverage(QList<int> timeDiffList);
    QVector<float> calculateValuesAverage(QList<QVector<float>> valuesList);
    void appendToRecordData(int timeDiff, QVector<float> values);

    std::shared_ptr<StorageRecorder> m_recorder;
    std::shared_ptr<StorageRecordData> m_recordData;

    const int m_factor = 4;
    int m_count = 0;
};

#endif // STORAGERECORDERINTERPOLATION_H
