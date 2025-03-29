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

QJsonObject IntrospectionData::jsonData() const
{
    return m_jsonData;
}

void IntrospectionData::setJsonData(const QJsonObject &t_jsonData)
{
    if(t_jsonData.isEmpty() == false)
        m_jsonData = t_jsonData;
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
    // TODO: Once transition to m_componentValues / m_rpcNames is done
    // this has to change
    return (m_jsonData.isEmpty() == false && entityId() >= 0);
}

QByteArray IntrospectionData::serialize() const
{
    QByteArray tmpData;
    QBuffer dataBuffer(&tmpData);
    dataBuffer.open(QIODevice::WriteOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);

    QJsonDocument dummyDoc;
    dummyDoc.setObject(m_jsonData);
    QByteArray binaryJson = dummyDoc.toBinaryData();
    dataStream << entityId();
    dataStream << binaryJson;
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
    QByteArray binaryJson;
    QJsonDocument dummyDoc;
    int tmpEntityId;

    dataStream >> tmpEntityId;
    dataStream >> binaryJson;
    dataStream >> m_componentValues;
    dataStream >> m_rpcNames;

    dataBuffer.close();
    dummyDoc = QJsonDocument::fromBinaryData(binaryJson);
    m_jsonData = dummyDoc.object();

    setEntityId(tmpEntityId);
}
}

