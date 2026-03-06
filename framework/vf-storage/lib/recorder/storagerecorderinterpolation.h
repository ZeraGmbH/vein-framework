#ifndef STORAGERECORDERINTERPOLATION_H
#define STORAGERECORDERINTERPOLATION_H

#include "abstractstoragerecorder.h"

class StorageRecorderInterpolation : public AbstractStorageRecorder
{
public:
    StorageRecorderInterpolation(const RecordEntityComponents &componentsRecorded, std::shared_ptr<AbstractStorageRecorder> recorder, int factor = 4);
    StorageRecordDataPtr getRecordedData() const override;

private slots:
    void onNewValueRecorded(int count);
private:
    void calculateJsonAverage(StorageRecordDataPtr storageRecorder, int startIdx, int endIdx);
    int calculateTimeDiffAverage(const QList<int> &timeDiffList);
    QVector<float> calculateValuesAverage(const QList<QVector<float>> &valuesList);
    void appendToRecordData(int timeDiff, const QVector<float> &values);

    std::shared_ptr<AbstractStorageRecorder> m_recorder;
    std::shared_ptr<StorageRecordData> m_recordData;

    int m_factor;
    int m_count = 0;
};

#endif // STORAGERECORDERINTERPOLATION_H
