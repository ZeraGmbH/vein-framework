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


}
