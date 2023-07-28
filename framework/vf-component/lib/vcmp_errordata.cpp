#include "vcmp_errordata.h"
#include <QBuffer>
#include <QDataStream>

namespace VeinComponent
{
ErrorData::ErrorData() : VeinEvent::EventData()
{
}

int ErrorData::originalDataType() const
{
    return m_originalDataType;
}

const QByteArray &ErrorData::originalData() const
{
    return m_originalData;
}

void ErrorData::setOriginalData(EventData *t_originalData)
{
    Q_ASSERT(t_originalData != nullptr);
    /// @note do not cascade error data, e.g. do not send an error data event with error data event as content
    if(t_originalData->type() != type()) {
        m_originalDataType = t_originalData->type();
        m_originalData = t_originalData->serialize();
    }
}

QString ErrorData::errorDescription() const
{
    return m_errorDescription;
}

void ErrorData::setErrorDescription(const QString &t_errorDescription)
{
    m_errorDescription = t_errorDescription;
}

QByteArray ErrorData::serialize() const
{
    QByteArray tmpData;
    QBuffer dataBuffer(&tmpData);
    dataBuffer.open(QIODevice::WriteOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << m_originalDataType;
    dataStream << m_originalData;
    dataStream << m_errorDescription;
    dataStream << entityId();

    dataBuffer.close();
    return tmpData;
}

void ErrorData::deserialize(const QByteArray &t_data)
{
    QByteArray tmpData=t_data;
    QBuffer dataBuffer(&tmpData);
    dataBuffer.open(QIODevice::ReadOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);

    dataStream >> m_originalDataType;
    dataStream >> m_originalData;
    dataStream >> m_errorDescription;
    int tmpEntityId;
    dataStream >> tmpEntityId;

    dataBuffer.close();
    setEntityId(tmpEntityId);
}

bool ErrorData::isValid() const
{
    return (m_originalData.isEmpty() == false && m_errorDescription.isEmpty() == false);
}
}

