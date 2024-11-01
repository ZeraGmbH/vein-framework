#include "vs_storagefilter.h"

namespace VeinStorage
{

StorageFilter::Settings::Settings(bool fireCurrentValueOnAddFilter, bool fireOnChangesOnly) :
    m_fireCurrentValueOnAddFilter(fireCurrentValueOnAddFilter),
    m_fireOnChangesOnly(fireOnChangesOnly)
{
}

StorageFilter::StorageFilter(AbstractEventSystem* storage, Settings settings) :
    m_storage{storage},
    m_settings(settings)
{
}

StorageFilter::~StorageFilter()
{
    clear();
}

bool StorageFilter::add(int entityId, QString componentName)
{
    AbstractComponentPtr actualComponent = m_storage->getComponent(entityId, componentName);
    if(!m_filteredEntityComponents.contains(entityId) || !m_filteredEntityComponents[entityId].contains(componentName)) {
        if(actualComponent) {
            m_filteredEntityComponents[entityId].insert(componentName);
            if(m_settings.m_fireCurrentValueOnAddFilter)
                fireActual(entityId, componentName, actualComponent);

            QMetaObject::Connection conn;
            if(m_settings.m_fireOnChangesOnly)
                conn = connect(actualComponent.get(), &AbstractComponent::sigValueChange, this, [=](QVariant newValue) {
                    emit sigComponentValue(entityId, componentName, newValue, actualComponent->getTimestamp());
                });
            else
                conn = connect(actualComponent.get(), &AbstractComponent::sigValueSet, this, [=](QVariant setValue) {
                    emit sigComponentValue(entityId, componentName, setValue, actualComponent->getTimestamp());
                });
            m_componentChangeConnections.append(conn);
            return true;
        }
    }
    return false;
}

void StorageFilter::clear()
{
    for(const auto &connection : qAsConst(m_componentChangeConnections))
        disconnect(connection);
    m_componentChangeConnections.clear();
    m_filteredEntityComponents.clear();
}

void StorageFilter::fireActual(int entityId, QString componentName, AbstractComponentPtr actualComponent)
{
    const QVariant currValue = actualComponent->getValue();
    if(currValue.isValid())
        emit sigComponentValue(entityId, componentName, currValue, actualComponent->getTimestamp());
}

}
