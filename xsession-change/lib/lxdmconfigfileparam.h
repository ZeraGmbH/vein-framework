#ifndef LXDMCONFIGFILEPARAM_H
#define LXDMCONFIGFILEPARAM_H

#include <QString>
#include <QList>

class LxdmConfigFileParam
{
public:
    struct XSession
    {
        XSession(const QString& sessionName, const QString &sessionFileName);
        QString m_sessionName;
        QString m_sessionFileName;
    };

    explicit LxdmConfigFileParam(
        const QString &configFileName = "/var/lib/lxdm/lxdm.conf",
        const QList<XSession> &availableXSessions = QList<LxdmConfigFileParam::XSession>() <<
            LxdmConfigFileParam::XSession("Desktop", "/usr/share/xsessions/xfce.desktop") <<
            LxdmConfigFileParam::XSession("Fullscreen", "/usr/share/xsessions/zenux-gui.desktop")
        );
    QString getConfigFileName() const;
    QList<XSession> getAvailableXSessions() const;
private:
    const QString m_configFileName;
    const QList<XSession> m_availableXSessions;
};

#endif // LXDMCONFIGFILEPARAM_H
