#include "vs_storagecomponent.h"

namespace VeinStorage
{

StorageComponent::StorageComponent(QVariant value) :
    m_value(value)
{
}

QVariant StorageComponent::getValue() const
{
    return m_value;
}

QDateTime StorageComponent::getTimestamp() const
{
    return m_timestamp;
}

void StorageComponent::setValue(QVariant value)
{
    emit sigValueSet(value);
    if(value != m_value) {
        m_value = value;
        emit sigValueChange(value);
    }
}

void StorageComponent::setTimestamp(QDateTime timestamp)
{
    m_timestamp = timestamp;
}

}
