#include "mocklxdmconfigfilegenerator.h"
#include <QDir>
#include <QUuid>

static const char* lxdmTestConfigFile = "lxdm.conf";

QString MockLxdmConfigFileGenerator::getLxdmTestDir()
{
    static QString lxdmTestDir;
    if (lxdmTestDir.isEmpty())
        lxdmTestDir = QString("/tmp/lxdmtest-%1/").arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
    return lxdmTestDir;
}

MockLxdmConfigFileGenerator::MockLxdmConfigFileGenerator(const QStringList &linesInLxdmConfig,
                                                         const QList<LxdmConfigFileParam::XSession> &availableXSessions) :
    m_configParam(getLxdmConfFileNameFull(), toFullPath(availableXSessions))
{
    QDir dir;
    dir.mkdir(getLxdmTestDir());
    createLxdmConfigFile(linesInLxdmConfig);
    for(const auto &session : availableXSessions)
        createTestSessionFile(session.m_sessionFileName);
}

void MockLxdmConfigFileGenerator::cleanup()
{
    QDir dir(getLxdmTestDir());
    dir.removeRecursively();
}

QString MockLxdmConfigFileGenerator::getLxdmConfFileNameFull()
{
    return QString(getLxdmTestDir()) + lxdmTestConfigFile;
}

QString MockLxdmConfigFileGenerator::genLastSessionLine(const QString &xSessionFileName)
{
    return QString("last_session=%1").arg(getLxdmTestDir() + xSessionFileName);
}

void MockLxdmConfigFileGenerator::createLxdmConfigFile(const QStringList &lines)
{
    QFile file(getLxdmConfFileNameFull());
    file.open(QFile::WriteOnly);
    for(const auto &line : lines)
        file.write(line.toLatin1() + "\n");
}

void MockLxdmConfigFileGenerator::removeLxdmConfigFile()
{
    QFile::remove(getLxdmConfFileNameFull());
}

void MockLxdmConfigFileGenerator::removeXsessionFile(const QString &xSessionFileName)
{
    QFile::remove(getLxdmTestDir() + xSessionFileName);
}

LxdmConfigFileParam MockLxdmConfigFileGenerator::getConfigParam()
{
    return m_configParam;
}

QList<LxdmConfigFileParam::XSession> MockLxdmConfigFileGenerator::toFullPath(const QList<LxdmConfigFileParam::XSession> &availableXSessions)
{
    QList<LxdmConfigFileParam::XSession> availableXSessionsFullPath;
    for(const auto &session : availableXSessions) {
        LxdmConfigFileParam::XSession sessionFullPath(session.m_sessionName, getLxdmTestDir() + session.m_sessionFileName);
        availableXSessionsFullPath.append(sessionFullPath);
    }
    return availableXSessionsFullPath;
}


void MockLxdmConfigFileGenerator::createTestSessionFile(const QString &fileName)
{
    QFile file(getLxdmTestDir() + fileName);
    file.open(QFile::WriteOnly);
}
