#ifndef MOCKLXDMCONFIGFILEGENERATOR_H
#define MOCKLXDMCONFIGFILEGENERATOR_H

#include "lxdmconfigfileparam.h"

class MockLxdmConfigFileGenerator
{
public:
    MockLxdmConfigFileGenerator(const QStringList &linesInLxdmConfig,
                                const QList<LxdmConfigFileParam::XSession> &availableXSessions);
    ~MockLxdmConfigFileGenerator();

    static QString getLxdmConfFileNameFull();
    // file names just basenames
    static QString genLastSessionLine(const QString xSessionFileName);
    static void createLxdmConfigFile(const QStringList &lines);
    static void removeLxdmConfigFile();
    static void removeXsessionFile(const QString xSessionFileName);

    LxdmConfigFileParam getConfigParam();
private:
    static QList<LxdmConfigFileParam::XSession> toFullPath(const QList<LxdmConfigFileParam::XSession> &availableXSessions);
    void createTestSessionFile(const QString &fileName);

    LxdmConfigFileParam m_configParam;
};

#endif // MOCKLXDMCONFIGFILEGENERATOR_H
