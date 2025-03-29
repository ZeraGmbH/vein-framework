#include "vcmp_introspectiondata.h"
#include <QBuffer>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

namespace VeinComponent
{
IntrospectionData::IntrospectionData() :
    VeinEvent::EventData()
{
}

QVariantMap IntrospectionData::componentValues() const
{
    return m_componentValues;
}

void IntrospectionData::setComponentValues(const QVariantMap &componentValues)
{
    m_componentValues = componentValues;
}

QStringList IntrospectionData::rpcNames() const
{
    return m_rpcNames;
}

void IntrospectionData::setRpcNames(const QStringList &rpcNames)
{
    m_rpcNames = rpcNames;
}

bool IntrospectionData::isValid() const
{
    return (m_componentValues.isEmpty() == false && entityId() >= 0);
}

QByteArray IntrospectionData::serialize() const
{
    QByteArray tmpData;
    QBuffer dataBuffer(&tmpData);
    dataBuffer.open(QIODevice::WriteOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);

    QJsonDocument dummyDoc;
    dataStream << entityId();
    dataStream << m_componentValues;
    dataStream << m_rpcNames;

    dataBuffer.close();
    return tmpData;
}

void IntrospectionData::deserialize(const QByteArray &t_data)
{
    QByteArray tmpData = t_data;
    QBuffer dataBuffer(&tmpData);
    dataBuffer.open(QIODevice::ReadOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);
    int tmpEntityId;

    dataStream >> tmpEntityId;
    dataStream >> m_componentValues;
    dataStream >> m_rpcNames;

    dataBuffer.close();
    setEntityId(tmpEntityId);
}
}

