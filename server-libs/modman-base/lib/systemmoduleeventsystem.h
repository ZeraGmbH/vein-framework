#ifndef MODULEMANAGERSETTINGS_H
#define MODULEMANAGERSETTINGS_H

#include <ve_eventsystem.h>
#include <ve_storagesystem.h>
#include <vcmp_componentdata.h>
#include <QJsonDocument>

class SystemModuleEventSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit SystemModuleEventSystem(QObject *t_parent = nullptr, bool devMode = false);
    static constexpr int getEntityId();
    VeinEvent::StorageSystem *getStorageSystem() const;
    void setStorage(VeinEvent::StorageSystem *t_storageSystem);
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
    /**
   * @brief the old fashioned way of writing error messages
   * @deprecated errors and warnings now are communicated via the system logging facility (probably systemd journal)
   * @param t_message
   */
    void handleNotificationMessage(QJsonObject t_message);
    QByteArray setModuleInterface();
    QString fromSessionNameToJsonName(QString sessionName);
    QString fromJsonNameToSessionName(QString jsonName);

    VeinEvent::StorageSystem *m_storageSystem = nullptr;
    QJsonDocument m_notificationMessages;
    QString m_currentSession;
    QStringList m_availableSessions;
    QStringList m_availableSessionsDisplayed;
    QString m_configFileName;
    QMap<QString, VeinComponent::ComponentData*> m_veinSystemParameterMap;
    bool m_initDone=false;
    bool m_sessionReady=false;
    bool m_modulesPaused=false;
    bool m_devMode = false;
};

#endif // MODULEMANAGERSETTINGS_H
