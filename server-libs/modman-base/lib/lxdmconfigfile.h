#ifndef LXDMCONFIGFILE_H
#define LXDMCONFIGFILE_H

#include <QString>
#include <QList>

class LxdmConfigFile
{
public:
    struct XSession
    {
        XSession(const QString& sessionName, const QString &sessionFileName);
        QString m_sessionName;
        QString m_sessionFileName;
    };
    explicit LxdmConfigFile(const QString &configFileName = "/var/lib/lxdm/lxdm.conf",
                            const QList<XSession> &availableXSessions =
                            QList<XSession>() <<
                                XSession("Desktop", "/usr/share/xsessions/xfce.desktop") <<
                                XSession("Fullscreen", "/usr/share/xsessions/zenux-gui.desktop"));

    const QString getConfiguredXSessionName();
    const QStringList getAvailableXSessionNames();
    bool setCurrentXSession(const QString &sessionName);

private:
    QStringList readLxdmConfig();
    const QString sessionNameFromFile(const QString& sessionFileName);
    bool writeConfig(const QString &sessionFileName);

    const QString m_configFileName;
    QList<XSession> m_availableXSessions;
};

#endif // LXDMCONFIGFILE_H
