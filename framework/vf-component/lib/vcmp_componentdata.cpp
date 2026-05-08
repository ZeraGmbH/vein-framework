#include "vcmp_componentdata.h"

#include <QBuffer>
#include <QDataStream>

namespace VeinComponent
{
ComponentData::ComponentData() :
    VeinEvent::EventData()
{
}

ComponentData::ComponentData(int entityId, ComponentData::Command cDataCommand) :
    VeinEvent::EventData(),
    m_command(cDataCommand)
{
    setEntityId(entityId);
}

ComponentData::ComponentData(const ComponentData &other) :
    VeinEvent::EventData(),
    m_command(other.eventCommand()),
    m_componentName(other.componentName()),
    m_newValue(other.newValue()),
    m_oldValue(other.oldValue())
{
    setEntityId(other.entityId());
}

ComponentData::Command ComponentData::eventCommand() const
{
    return m_command;
}

void ComponentData::setCommand(ComponentData::Command cDataCommand)
{
    if(m_command == Command::CCMD_INVALID)
        m_command = cDataCommand;
    else
        qCWarning(VEIN_COMPONENT) << "Component command already set:" << this;
}

const QString &ComponentData::componentName() const
{
    return m_componentName;
}

void ComponentData::setComponentName(const QString &componentName)
{
    if(componentName.isEmpty() == false)
        m_componentName = componentName;
}

const QVariant &ComponentData::newValue() const
{
    return m_newValue;
}

void ComponentData::setNewValue(const QVariant &newValue)
{
    Q_ASSERT(newValue.typeName() != QLatin1String("QJsonValue"));
    if(newValue.isValid())
        m_newValue=newValue;
}

const QVariant &ComponentData::oldValue() const
{
    return m_oldValue;
}

void ComponentData::setOldValue(const QVariant &oldValue)
{
    if(m_oldValue != oldValue)
        m_oldValue=oldValue;
}

bool ComponentData::isValid() const
{
    bool retVal = false;
    switch(m_command)
    {
    case Command::CCMD_ADD:
    case Command::CCMD_REMOVE:
    case Command::CCMD_SET:
    case Command::CCMD_FETCH:
        retVal = true; // no special requirements
        break;
    default:
        break;
    }
    return retVal;
}

QByteArray ComponentData::serialize() const
{
    QByteArray tmpData;
    QBuffer dataBuffer(&tmpData);
    dataBuffer.open(QIODevice::WriteOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);

    dataStream << static_cast<qint8>(m_command);
    dataStream << m_componentName;
    dataStream << entityId();
    dataStream << m_newValue;
    dataStream << m_oldValue;

    dataBuffer.close();
    return tmpData;
}

void ComponentData::deserialize(const QByteArray &data)
{
    QByteArray tmpData=data;
    QBuffer dataBuffer(&tmpData);
    dataBuffer.open(QIODevice::ReadOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);
    qint8 tmpCommand;
    int tmpEntityId;

    dataStream >> tmpCommand;
    m_command = static_cast<ComponentData::Command>(tmpCommand);
    dataStream >> m_componentName;
    dataStream >> tmpEntityId;
    dataStream >> m_newValue;
    dataStream >> m_oldValue;

    dataBuffer.close();
    setEntityId(tmpEntityId);
}
}


