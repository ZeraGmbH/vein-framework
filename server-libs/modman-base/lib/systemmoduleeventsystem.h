#ifndef MODULEMANAGERSETTINGS_H
#define MODULEMANAGERSETTINGS_H

#include <ve_eventsystem.h>
#include <vs_abstracteventsystem.h>
#include <vcmp_componentdata.h>
#include <QJsonDocument>

class SystemModuleEventSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit SystemModuleEventSystem(QObject *t_parent = nullptr, bool devMode = false);
    static constexpr int getEntityId();
    void setStorage(VeinStorage::AbstractEventSystem *t_storageSystem);
    void processEvent(QEvent *t_event) override;
    void setConfigFileName(QString configFileName);
    void setAvailableSessionList(QStringList availableSessionList);
signals:
    void sigChangeSession(const QString &t_newSession);
    void sigModulesPausedChanged(bool t_paused);

public slots:
    void initializeEntity(const QString &sessionPath, const QStringList &sessionList);
    void initOnce();
    void setModulesPaused(bool t_paused);

private:
    QByteArray setModuleInterface();
    static QString deduceDeviceName(const QString& sessionString);
    QString getJsonSessionName(QString displayedSessionName);
    QString getDisplayedSessionName(QString jsonSessionName);
    void sendSessionNotificationForScpiModule(const VeinComponent::ComponentData *cData);
    void setScpiInfo();

    VeinStorage::AbstractEventSystem *m_storageSystem = nullptr;
    QJsonDocument m_notificationMessages;
    QString m_currentSession;
    QStringList m_availableSessions;
    QStringList m_availableSessionsDisplayed;
    QString m_configFileName;
    QMap<QString, VeinComponent::ComponentData*> m_veinSystemParameterMap;
    QList<cSCPIInfo*> m_scpiCmdList;
    bool m_initDone=false;
    bool m_sessionReady=false;
    bool m_modulesPaused=false;
    bool m_devMode = false;
};

#endif // MODULEMANAGERSETTINGS_H
