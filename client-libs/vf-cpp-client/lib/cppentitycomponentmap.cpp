#include "cppentitycomponentmap.h"

#include <ve_commandevent.h>



CppEntityComponentMap::CppEntityComponentMap(int entityId, const QVariantHash &entityIntrospection) :
    m_entityId(entityId),
    m_entityIntrospection(entityIntrospection)
{

}

CppEntityComponentMap::DataState CppEntityComponentMap::state() const
{
    return m_state;
}

void CppEntityComponentMap::setState(DataState dataState)
{
    m_state = dataState;
    if(m_state == DataState::ECM_PENDING) {

    }
    emit sigStateChanged(m_state);
}

void CppEntityComponentMap::processComponentData(VeinComponent::ComponentData *cData)
{
    switch(cData->eventCommand()) {
    case VeinComponent::ComponentData::Command::CCMD_ADD:
    case VeinComponent::ComponentData::Command::CCMD_SET:
        qDebug("Updated value for %s: %s", qPrintable(cData->componentName()), qPrintable(cData->newValue().toString()));
        //TODO: is this entityComponentMap a QVariantMap or has it a QVariantMap?
        //insert(cData->componentName(), cData->newValue()); // bypasses the function updateValue(...)
        break;
    case VeinComponent::ComponentData::Command::CCMD_REMOVE:
        /// @note It is not possible to remove keys from the map; once a key has been added, you can only modify or clear its associated value.
        /// @note Keys that have been cleared will still appear in this list, even though their associated values are invalid
        //TODO: is this entityComponentMap a QVariantMap or has it a QVariantMap?
        //clear(cData->componentName());
        break;
    case VeinComponent::ComponentData::Command::CCMD_FETCH:
        if(m_pendingValues.contains(cData->componentName())) {
            qDebug("Fetched value for %s: %s", qPrintable(cData->componentName()), qPrintable(cData->newValue().toString()));
            //TODO: is this entityComponentMap a QVariantMap or has it a QVariantMap?
            //insert(cData->componentName(), cData->newValue()); // bypasses the function updateValue(...)
            m_pendingValues.removeAll(cData->componentName());
            if(m_pendingValues.isEmpty()) {
                emit sigEntityComplete(m_entityId);
            }
        }
        break;
    default:
        break;
    }

}

int CppEntityComponentMap::entityId() const
{
    return m_entityId;
}

bool CppEntityComponentMap::hasComponent(const QString &componentName) const
{
    //TODO: is this entityComponentMap a QVariantMap or has it a QVariantMap?
    //return contains(componentName);
}

int CppEntityComponentMap::propertyCount() const
{
    //TODO: is this entityComponentMap a QVariantMap or has it a QVariantMap?
    //return count();
}

void CppEntityComponentMap::loadEntityData()
{
    VeinEvent::CommandEvent *cEvent = nullptr;
    VeinComponent::ComponentData *cData = nullptr;

    const QList<QString> tmpComponentList = m_entityIntrospection.value(QString("components")).toStringList();
    //m_registeredRemoteProcedures = m_entityIntrospection.value(QString("procedures")).toStringList();
    m_pendingValues.append(tmpComponentList);
    for(const QString &tmpKey : tmpComponentList) {
        //TODO: is this entityComponentMap a QVariantMap or has it a QVariantMap? Why is this implicid insert?
        //insert(tmpKey, QVariant()); // bypasses the function updateValue(...)

        cData = new VeinComponent::ComponentData();
        cData->setEntityId(m_entityId);
        cData->setCommand(VeinComponent::ComponentData::Command::CCMD_FETCH);
        cData->setEventOrigin(VeinComponent::ComponentData::EventOrigin::EO_LOCAL);
        cData->setEventTarget(VeinComponent::ComponentData::EventTarget::ET_ALL);
        cData->setComponentName(tmpKey);
        cEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::TRANSACTION, cData);
        // TODO: add event back into debug message? How did this work prior with a pointer?
        qDebug("Fetching entity data for entityId: %i component: %s event: tbd", m_entityId, qPrintable(tmpKey));

        emit sigSendEvent(cEvent);
    }
}

