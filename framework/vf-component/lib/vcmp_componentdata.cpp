#include "vcmp_componentdata.h"

#include <QBuffer>
#include <QDataStream>
#include <QVariant>

namespace VeinComponent
{
ComponentData::ComponentData() :
    VeinEvent::EventData()
{
}

ComponentData::ComponentData(int t_entityId, ComponentData::Command t_cDataCommand) :
    VeinEvent::EventData(),
    m_command(t_cDataCommand)
{
    setEntityId(t_entityId);
}

ComponentData::ComponentData(const ComponentData &t_other) :
    VeinEvent::EventData(),
    m_command(t_other.eventCommand()),
    m_componentName(t_other.componentName()),
    m_newValue(t_other.newValue()),
    m_oldValue(t_other.oldValue())
{
    setEntityId(t_other.entityId());
}

ComponentData::~ComponentData()
{
    if(m_scpiinfo)
        m_scpiinfo = nullptr;
    delete m_scpiinfo;
}

ComponentData::Command ComponentData::eventCommand() const
{
    return m_command;
}

void ComponentData::setCommand(ComponentData::Command t_cDataCommand)
{
    if(m_command == Command::CCMD_INVALID)
        m_command = t_cDataCommand;
    else
        qCWarning(VEIN_COMPONENT) << "Component command already set:" << this;
}

const QString &ComponentData::componentName() const
{
    return m_componentName;
}

void ComponentData::setComponentName(const QString &t_componentName)
{
    if(t_componentName.isEmpty() == false)
        m_componentName = t_componentName;
}

const QVariant &ComponentData::newValue() const
{
    return m_newValue;
}

void ComponentData::setNewValue(const QVariant &t_newValue)
{
    Q_ASSERT(t_newValue.typeName() != QLatin1String("QJsonValue"));
    if(t_newValue.isValid())
        m_newValue=t_newValue;
}

const QVariant &ComponentData::oldValue() const
{
    return m_oldValue;
}

void ComponentData::setOldValue(const QVariant &t_oldValue)
{
    if(m_oldValue != t_oldValue)
        m_oldValue=t_oldValue;
}

void ComponentData::setSCPIInfo(cSCPIInfo *scpiinfo)
{
    m_scpiinfo = scpiinfo;
}

void ComponentData::exportSCPIInfo(QJsonArray &jsArr)
{
    if (m_scpiinfo)
        m_scpiinfo->appendSCPIInfo(jsArr);
}

void ComponentData::exportMetaData(QJsonObject &jsonObj)
{
    if (m_pValidator != nullptr) {
        QJsonObject jsonObj2;
        m_pValidator->exportMetaData(jsonObj2);
        jsonObj.insert("Validation", jsonObj2);
    }
}

void ComponentData::setValidator(ValidatorInterface *validator)
{
    m_pValidator = validator;
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

void ComponentData::deserialize(const QByteArray &t_data)
{
    QByteArray tmpData=t_data;
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


