#include "vs_storagecomponent.h"
#include <timerfactoryqt.h>

namespace VeinStorage
{
int StorageComponent::m_instanceCount = 0;

StorageComponent::StorageComponent(QVariant value) :
    m_value(value)
{
    m_instanceCount ++;
}

StorageComponent::~StorageComponent()
{
    m_instanceCount --;
}

const QVariant &StorageComponent::getValue() const
{
    return m_value;
}

void StorageComponent::setValue(const QVariant &value)
{
    emit sigValueSet(value);
    if(value != m_value) {
        m_value = value;
        emit sigValueChange(value);
    }
}

int StorageComponent::getInstanceCount()
{
    return m_instanceCount;
}

}
