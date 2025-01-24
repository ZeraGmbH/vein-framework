#ifndef LXDMCONFIGFILE_H
#define LXDMCONFIGFILE_H

#include "lxdmconfigfileparam.h"

class LxdmConfigFile
{
public:
    explicit LxdmConfigFile(const LxdmConfigFileParam &param = LxdmConfigFileParam());

    const QString getConfiguredXSessionName();
    const QStringList getAvailableXSessionNames();
    bool setCurrentXSession(const QString &sessionName);

private:
    QStringList readLxdmConfig();
    const QString sessionNameFromFile(const QString& sessionFileName);
    bool writeConfig(const QString &sessionFileName);

    const QString m_configFileName;
    QList<LxdmConfigFileParam::XSession> m_availableXSessions;
};

#endif // LXDMCONFIGFILE_H
