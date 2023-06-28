#include "vcmp_remoteproceduredata.h"
#include <QBuffer>
#include <QDataStream>

namespace VeinComponent
{
  const QLatin1String RemoteProcedureData::s_callIdString = QLatin1String("RemoteProcedureData::callID");
  const QLatin1String RemoteProcedureData::s_resultCodeString = QLatin1String("RemoteProcedureData::resultCode");
  const QLatin1String RemoteProcedureData::s_errorMessageString = QLatin1String("RemoteProcedureData::errorMessage");
  const QLatin1String RemoteProcedureData::s_parameterString = QLatin1String("RemoteProcedureData::callParameters");


  RemoteProcedureData::RemoteProcedureData() : VeinEvent::EventData()
  {
  }

  RemoteProcedureData::Command RemoteProcedureData::command() const
  {
    return m_command;
  }

  void RemoteProcedureData::setCommand(RemoteProcedureData::Command t_command)
  {
    m_command = t_command;
  }

  QString RemoteProcedureData::procedureName() const
  {
    return m_procedureName;
  }

  void RemoteProcedureData::setProcedureName(const QString &t_procedureName)
  {
    m_procedureName = t_procedureName;
  }

  QVariantMap RemoteProcedureData::invokationData() const
  {
    return m_invokationData;
  }

  void RemoteProcedureData::setInvokationData(QVariantMap t_invokationData)
  {
    m_invokationData = t_invokationData;
  }

  bool RemoteProcedureData::isValid() const
  {
    return (m_procedureName.isEmpty() == false
            && entityId() >= 0
            && m_command != Command::RPCMD_INVALID);
  }

  QByteArray RemoteProcedureData::serialize() const
  {
    QByteArray tmpData;
    QBuffer dataBuffer(&tmpData);

    dataBuffer.open(QIODevice::WriteOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);

    dataStream << static_cast<qint8>(m_command);
    dataStream << m_procedureName;
    dataStream << entityId();
    dataStream << m_invokationData;

    dataBuffer.close();
    return tmpData;
  }

  void RemoteProcedureData::deserialize(const QByteArray &t_data)
  {
    QByteArray tmpData=t_data;
    QBuffer dataBuffer(&tmpData);

    dataBuffer.open(QIODevice::ReadOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);
    qint8 tmpCommand;
    int tmpEntityId;

    dataStream >> tmpCommand;
    m_command = static_cast<RemoteProcedureData::Command>(tmpCommand);
    dataStream >> m_procedureName;
    dataStream >> tmpEntityId;
    dataStream >> m_invokationData;

    dataBuffer.close();

    setEntityId(tmpEntityId);
  }

} // namespace VeinComponent
