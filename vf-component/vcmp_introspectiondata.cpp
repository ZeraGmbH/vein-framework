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
    {
      m_jsonData = t_jsonData;
    }
  }

  bool IntrospectionData::isValid() const
  {
    return (m_jsonData.isEmpty() == false && entityId() >= 0);
  }

  QByteArray IntrospectionData::serialize() const
  {
    QByteArray tmpData;
    QBuffer dataBuffer(&tmpData);

    dataBuffer.open(QIODevice::WriteOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);
    QByteArray binaryJson;
    QJsonDocument dummyDoc;

    dummyDoc.setObject(m_jsonData);
    binaryJson = dummyDoc.toBinaryData();
    dataStream << entityId();
    dataStream << binaryJson;

    dataBuffer.close();

    return tmpData;
  }

  void IntrospectionData::deserialize(const QByteArray &t_data)
  {
    QByteArray tmpData=t_data;
    QBuffer dataBuffer(&tmpData);

    dataBuffer.open(QIODevice::ReadOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);
    QByteArray binaryJson;
    QJsonDocument dummyDoc;
    int tmpEntityId;

    dataStream >> tmpEntityId;
    dataStream >> binaryJson;

    dataBuffer.close();

    dummyDoc = QJsonDocument::fromBinaryData(binaryJson);

    m_jsonData = dummyDoc.object();

    setEntityId(tmpEntityId);
  }
} // namespace VeinComponent

