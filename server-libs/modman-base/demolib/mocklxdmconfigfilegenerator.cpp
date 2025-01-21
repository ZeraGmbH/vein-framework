#include "mocklxdmconfigfilegenerator.h"
#include <QDir>

static const char* lxdmTestDir = "/tmp/lxdmtest/";
static const char* lxdmTestConfigFile = "lxdm.conf";

MockLxdmConfigFileGenerator::MockLxdmConfigFileGenerator(const QStringList &linesInLxdmConfig,
                                                         const QList<LxdmConfigFileParam::XSession> &availableXSessions) :
    m_configParam(getLxdmConfFileNameFull(), toFullPath(availableXSessions))
{
    QDir dir;
    dir.mkdir(lxdmTestDir);
    createLxdmConfigFile(linesInLxdmConfig);
    for(const auto session : availableXSessions)
        createTestSessionFile(session.m_sessionFileName);
}

MockLxdmConfigFileGenerator::~MockLxdmConfigFileGenerator()
{
    QDir dir(lxdmTestDir);
    dir.removeRecursively();
}

QString MockLxdmConfigFileGenerator::getLxdmConfFileNameFull()
{
    return QString(lxdmTestDir) + lxdmTestConfigFile;
}

QString MockLxdmConfigFileGenerator::genLastSessionLine(const QString xSessionFileName)
{
    return QString("last_session=%1").arg(lxdmTestDir + xSessionFileName);
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

void MockLxdmConfigFileGenerator::removeXsessionFile(const QString xSessionFileName)
{
    QFile::remove(lxdmTestDir + xSessionFileName);
}

LxdmConfigFileParam MockLxdmConfigFileGenerator::getConfigParam()
{
    return m_configParam;
}

QList<LxdmConfigFileParam::XSession> MockLxdmConfigFileGenerator::toFullPath(const QList<LxdmConfigFileParam::XSession> &availableXSessions)
{
    QList<LxdmConfigFileParam::XSession> availableXSessionsFullPath;
    for(const auto &session : availableXSessions) {
        LxdmConfigFileParam::XSession sessionFullPath(session.m_sessionName, lxdmTestDir + session.m_sessionFileName);
        availableXSessionsFullPath.append(sessionFullPath);
    }
    return availableXSessionsFullPath;
}


void MockLxdmConfigFileGenerator::createTestSessionFile(const QString &fileName)
{
    QFile file(lxdmTestDir + fileName);
    file.open(QFile::WriteOnly);
}
