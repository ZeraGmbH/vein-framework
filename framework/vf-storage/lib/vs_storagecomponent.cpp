#include "vs_storagecomponent.h"
#include <timerfactoryqt.h>

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

void StorageComponent::setTimestamp()
{
    m_timestamp = TimerFactoryQt::getCurrentTime();
}

}
