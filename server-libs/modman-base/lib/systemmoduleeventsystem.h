#ifndef MODULEMANAGERSETTINGS_H
#define MODULEMANAGERSETTINGS_H


#include "modman_util.h"

#include <ve_eventsystem.h>

#include <QJsonDocument>

namespace VeinEvent {
class StorageSystem;
}

class SystemModuleEventSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit SystemModuleEventSystem(QObject *t_parent = nullptr, bool demo = false);
    static constexpr int getEntityId();
    VeinEvent::StorageSystem *getStorageSystem() const;
    /**
   * @brief sets the storage system that is used to get the list of entity ids that the storage holds
   * @param t_storageSystem
   */
    void setStorage(VeinEvent::StorageSystem *t_storageSystem);

    // EventSystem interface
public:
    void processEvent(QEvent *t_event) override;

signals:
    /**
   * @brief emitted if the session is switched via the "Session" component
   * @param t_newSession
   */
    void sigChangeSession(const QString &t_newSession);
    /**
   * @brief emitted if the modules are paused via the "ModulesPaused" component
   * @param t_paused
   */
    void sigModulesPausedChanged(bool t_paused);

public slots:
    /**
   * @brief connected to the signal ModuleManager::sigModulesLoaded, loads the list of entities from the m_storageSystem then sets up the data of all components
   * @param t_sessionPath
   * @param t_sessionList
   */
    void initializeEntity(const QString &sessionPath, const QStringList &sessionList);
    /**
   * @brief sends add events for the entity and all components
   */
    void initOnce();
    /**
   * @brief emits sigModulesPausedChanged
   * @param t_paused
   */
    void setModulesPaused(bool t_paused);

private:
    /**
   * @brief the old fashioned way of writing error messages
   * @deprecated errors and warnings now are communicated via the system logging facility (probably systemd journal)
   * @param t_message
   */
    void handleNotificationMessage(QJsonObject t_message);

    static constexpr int s_entityId = 0;


    VeinEvent::StorageSystem *m_storageSystem = nullptr;
    QJsonDocument m_notificationMessages;
    QString m_currentSession;
    QStringList m_availableSessions;
    bool m_initDone=false;
    bool m_sessionReady=false;
    bool m_modulesPaused=false;
    bool m_devMode = false;
};

#endif // MODULEMANAGERSETTINGS_H
