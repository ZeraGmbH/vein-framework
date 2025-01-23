#include "lxdmconfigfile.h"
#include <QFile>
#include <QSaveFile>

static const char* sessionLead = "last_session=";

LxdmConfigFile::LxdmConfigFile(const LxdmConfigFileParam &param) :
    m_configFileName(param.getConfigFileName())
{
    if(QFile::exists(m_configFileName)) {
        const QList<LxdmConfigFileParam::XSession> availableXSessions = param.getAvailableXSessions();
        QStringList sessionFilesFound;
        for(const LxdmConfigFileParam::XSession &session : availableXSessions) {
            if(QFile::exists(session.m_sessionFileName)) {
                m_availableXSessions.append(session);
                sessionFilesFound.append(session.m_sessionFileName);
            }
            else
                qWarning("XSession file '%s' does not exist. Skip session '%s'!",
                         qPrintable(session.m_sessionFileName), qPrintable(session.m_sessionName));
        }
        qInfo("XSession manager config file: %s", qPrintable(m_configFileName));
        qInfo("XSession files: %s", qPrintable(sessionFilesFound.join(" / ")));
        qInfo("XSession selected: %s", qPrintable(getConfiguredXSessionName()));
    }
    else
        qWarning("Lxdm config '%s' not found - XSession change will not work!", qPrintable(m_configFileName));
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
    if(!m_availableXSessions.isEmpty())
        qWarning("Cannot find '%s' in %s!", sessionLead, qPrintable(m_configFileName));
    return QString();
}

const QStringList LxdmConfigFile::getAvailableXSessionNames()
{
    QStringList names;
    for(const auto  &session : m_availableXSessions)
        names.append(session.m_sessionName);
    return names;
}

bool LxdmConfigFile::setCurrentXSession(const QString &sessionName)
{
    for(const auto &session : m_availableXSessions)
        if(session.m_sessionName == sessionName)
            return writeConfig(session.m_sessionFileName);
    qWarning("XSession %s to set is unknown!", qPrintable(sessionName));
    return false;
}

QStringList LxdmConfigFile::readLxdmConfig()
{
    QStringList lines;
    QFile configFileRead(m_configFileName);
    if(!configFileRead.open(QFile::ReadOnly)) {
        if(!m_availableXSessions.isEmpty())
            qWarning("Cannot read %s", qPrintable(m_configFileName));
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
    qWarning("Unknown XSession file '%s' in %s!",
             qPrintable(sessionFileName), qPrintable(m_configFileName));
    return QString();
}

bool LxdmConfigFile::writeConfig(const QString &sessionFileName)
{
    if(!QFile::exists(sessionFileName)) {
        qWarning("XSession file '%s' does not exist!", qPrintable(sessionFileName));
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
        qWarning("Cannot not write %s", qPrintable(m_configFileName));
        return false;
    }
    for(const QString &line : linesWrite)
        configFileWrite.write(line.toLatin1() + "\n");
    configFileWrite.commit();
    return true;
}
