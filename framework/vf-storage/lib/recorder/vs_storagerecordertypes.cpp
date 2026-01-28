#include "vs_storagerecordertypes.h"

namespace VeinStorage {


bool RecordComponent::operator == (const RecordComponent& other) const {
    return
        m_componentName == other.m_componentName &&
        m_label == other.m_label;
}


StorageRecordData::StorageRecordData(const RecordEntityComponents componentInfo) :
    m_componentInfo(componentInfo)
{
}

void StorageRecordData::resetRecords() {
    m_timeOfFirstRecord = QDateTime();
    m_recordedData.clear();
}

RecordEntityComponentSequence RecordParamSequencer::toSequence(const RecordEntityComponents &entityComponentsParam)
{
    QList<RecordEntityComponent> entityComponentSequence;
    for(auto iter = entityComponentsParam.constBegin(); iter != entityComponentsParam.constEnd(); ++iter) {
        int entityId = iter.key();
        const QList<RecordComponent> &components = iter.value();
        for (const RecordComponent &component : components) {
            entityComponentSequence.append({entityId, {component.m_componentName, component.m_label}});
        }
    }
    return entityComponentSequence;
}

}
