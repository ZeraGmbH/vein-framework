#ifndef STORAGETYPES_H
#define STORAGETYPES_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QList>
#include <QDateTime>
#include <memory>

namespace VeinStorage {

// Parameter definitions
struct RecordComponent {
    QString m_componentName;
    QString m_label;
    bool operator == (const RecordComponent& other) const;
};
typedef QMap<int, QList<RecordComponent>> RecordEntityComponents;

// Extract squence from Parameter definitions => RecordEntityComponentSequence::toSequence
struct RecordEntityComponent {
    int m_entityId;
    RecordComponent m_component;
};
typedef QList<RecordEntityComponent> RecordEntityComponentSequence;

namespace RecordParamSequencer {
    RecordEntityComponentSequence toSequence(const RecordEntityComponents &entityComponentsParam);
};


// Result definitions
struct RecordEntryStored {
    int m_timeDiffToFirstInMs = 0;
    QVector<float> m_values;
};
typedef QList<RecordEntryStored> RecordListStored;

struct StorageRecordData {
    StorageRecordData(const RecordEntityComponents componentInfo);
    void resetRecords();

    const RecordEntityComponents m_componentInfo;
    QDateTime m_timeOfFirstRecord;
    RecordListStored m_recordedData;
};

typedef std::shared_ptr<const StorageRecordData> StorageRecordDataPtr;

}

#endif // STORAGETYPES_H
