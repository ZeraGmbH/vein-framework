#include "lxdmconfigfile.h"
#include <QFile>
#include <QSaveFile>

static const char* sessionLead = "last_session=";

LxdmConfigFile::LxdmConfigFile(const QString &configFileName,
                               const QList<XSession> &availableXSessions) :
    m_configFileName(configFileName)
{
    if(QFile::exists(configFileName)) {
        for(const XSession &session : availableXSessions) {
            if(QFile::exists(session.m_sessionFileName))
                m_availableXSessions.append(session);
            else
                qWarning("Session file '%s' does not exist. Skip session '%s'!",
                         qPrintable(session.m_sessionFileName), qPrintable(session.m_sessionName));
        }
    }
}

const QString LxdmConfigFile::getConfiguredXSessionName()
{
    QStringList lines = readLxdmConfig();
    for(const QString &line : lines) {
        if(line.startsWith(sessionLead)) {
            QStringList labelValue = line.split("=");
            if(labelValue.size() == 2)
                return sessionNameFromFile(labelValue[1]);
        }
    }
    qWarning("Cannot not find '%s' in %s!", sessionLead, qPrintable(m_configFileName));
    return QString();
}

const QStringList LxdmConfigFile::getAvailableXSessionNames()
{
    QStringList names;
    for(const XSession &session : m_availableXSessions)
        names.append(session.m_sessionName);
    return names;
}

bool LxdmConfigFile::setCurrentXSession(const QString &sessionName)
{
    for(const auto &session : m_availableXSessions)
        if(session.m_sessionName == sessionName)
            return writeConfig(session.m_sessionFileName);
    qWarning("Session %s to set is unknown!", qPrintable(sessionName));
    return false;
}

QStringList LxdmConfigFile::readLxdmConfig()
{
    QStringList lines;
    QFile configFileRead(m_configFileName);
    if(!configFileRead.open(QFile::ReadOnly)) {
        qWarning("Cannot not read %s", qPrintable(m_configFileName));
        return lines;
    }
    QString content = configFileRead.readAll();
    content.replace("\r", "");
    lines = content.split("\n");
    configFileRead.close();
    return lines;
}

const QString LxdmConfigFile::sessionNameFromFile(const QString &sessionFileName)
{
    for(const auto &session : m_availableXSessions)
        if(session.m_sessionFileName == sessionFileName)
            return session.m_sessionName;
    qWarning("Unknown session file '%s' in %s!",
             qPrintable(sessionFileName), qPrintable(m_configFileName));
    return QString();
}

bool LxdmConfigFile::writeConfig(const QString &sessionFileName)
{
    if(!QFile::exists(sessionFileName)) {
        qWarning("Session file '%s' does not exist!", qPrintable(sessionFileName));
        return false;
    }
    const QStringList linesRead = readLxdmConfig();
    bool lastSessionFound = false;
    QStringList linesWrite;
    QString lineContentToSet = QString(sessionLead) + sessionFileName;
    for(const QString &line : linesRead) {
        QString lineWrite = line;
        if(line.startsWith(sessionLead)) {
            if(lastSessionFound)
                continue;
            lineWrite = lineContentToSet;
            lastSessionFound = true;
        }
        if(!lineWrite.isEmpty())
            linesWrite.append(lineWrite);
    }
    if(!lastSessionFound)
        linesWrite.append(lineContentToSet);

    QSaveFile configFileWrite(m_configFileName);
    if(!configFileWrite.open(QFile::WriteOnly)) {
        qWarning("Cannot not write %s", qPrintable(sessionFileName));
        return false;
    }
    for(const QString &line : linesWrite)
        configFileWrite.write(line.toLatin1() + "\n");
    configFileWrite.commit();
    return true;
}

LxdmConfigFile::XSession::XSession(const QString &sessionName, const QString &sessionFileName) :
    m_sessionName(sessionName),
    m_sessionFileName(sessionFileName)
{
}
