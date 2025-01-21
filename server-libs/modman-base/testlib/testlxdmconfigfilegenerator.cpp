#include "testlxdmconfigfilegenerator.h"
#include <QDir>

static const char* lxdmTestDir = "/tmp/lxdmtest/";
static const char* lxdmTestConfigFile = "lxdm.conf";

TestLxdmConfigFileGenerator::TestLxdmConfigFileGenerator(const QStringList &linesInLxdmConfig,
                                                         const QList<LxdmConfigFileParam::XSession> &availableXSessions) :
    m_configParam(getLxdmConfFileNameFull(), toFullPath(availableXSessions))
{
    QDir dir;
    dir.mkdir(lxdmTestDir);
    createLxdmConfigFile(linesInLxdmConfig);
    for(const auto session : availableXSessions)
        createTestSessionFile(session.m_sessionFileName);
}

TestLxdmConfigFileGenerator::~TestLxdmConfigFileGenerator()
{
    QDir dir(lxdmTestDir);
    dir.removeRecursively();
}

QString TestLxdmConfigFileGenerator::getLxdmConfFileNameFull()
{
    return QString(lxdmTestDir) + lxdmTestConfigFile;
}

QString TestLxdmConfigFileGenerator::genLastSessionLine(const QString xSessionFileName)
{
    return QString("last_session=%1").arg(lxdmTestDir + xSessionFileName);
}

void TestLxdmConfigFileGenerator::createLxdmConfigFile(const QStringList &lines)
{
    QFile file(getLxdmConfFileNameFull());
    file.open(QFile::WriteOnly);
    for(const auto &line : lines)
        file.write(line.toLatin1() + "\n");
}

void TestLxdmConfigFileGenerator::removeLxdmConfigFile()
{
    QFile::remove(getLxdmConfFileNameFull());
}

void TestLxdmConfigFileGenerator::removeXsessionFile(const QString xSessionFileName)
{
    QFile::remove(lxdmTestDir + xSessionFileName);
}

LxdmConfigFileParam TestLxdmConfigFileGenerator::getConfigParam()
{
    return m_configParam;
}

QList<LxdmConfigFileParam::XSession> TestLxdmConfigFileGenerator::toFullPath(const QList<LxdmConfigFileParam::XSession> &availableXSessions)
{
    QList<LxdmConfigFileParam::XSession> availableXSessionsFullPath;
    for(const auto &session : availableXSessions) {
        LxdmConfigFileParam::XSession sessionFullPath(session.m_sessionName, lxdmTestDir + session.m_sessionFileName);
        availableXSessionsFullPath.append(sessionFullPath);
    }
    return availableXSessionsFullPath;
}


void TestLxdmConfigFileGenerator::createTestSessionFile(const QString &fileName)
{
    QFile file(lxdmTestDir + fileName);
    file.open(QFile::WriteOnly);
}
