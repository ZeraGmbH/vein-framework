#include "lxdmconfigfileparam.h"

LxdmConfigFileParam::XSession::XSession(const QString &sessionName, const QString &sessionFileName) :
    m_sessionName(sessionName),
    m_sessionFileName(sessionFileName)
{
}

LxdmConfigFileParam::LxdmConfigFileParam(const QString &configFileName,
                                         const QList<XSession> &availableXSessions) :
    m_configFileName(configFileName),
    m_availableXSessions(availableXSessions)
{
}

QString LxdmConfigFileParam::getConfigFileName() const
{
    return m_configFileName;
}

QList<LxdmConfigFileParam::XSession> LxdmConfigFileParam::getAvailableXSessions() const
{
    return m_availableXSessions;
}
