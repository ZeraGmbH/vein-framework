#ifndef MODULEMANAGERSETTINGS_H
#define MODULEMANAGERSETTINGS_H

#include "lxdmsessionchangeparam.h"
#include "lxdmconfigfile.h"
#include "validatorinterface.h"
#include <ve_eventsystem.h>
#include <vs_abstracteventsystem.h>
#include <vcmp_componentdata.h>
#include <scpiveincomponentinfo.h>
#include <QJsonDocument>

class SystemModuleEventSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit SystemModuleEventSystem(bool devMode = false,
                                     const LxdmSessionChangeParam& lxdmParam = LxdmSessionChangeParam());
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
    QString getJsonSessionName(const QString &displayedSessionName);
    QString getDisplayedSessionName(const QString &jsonSessionName);
    bool handleVeinSessionSet(const VeinComponent::ComponentData *cData);
    bool handleXSessionSet(const QString &xSession);
    void sendNotificationForScpiModule(const VeinComponent::ComponentData *cData);
    void sendSessionNotificationForScpiModule(const VeinComponent::ComponentData *cData);
    void setScpiInfo();

    VeinStorage::AbstractEventSystem *m_storageSystem = nullptr;
    QJsonDocument m_notificationMessages;
    QString m_currentSession;
    QStringList m_availableSessions;
    QStringList m_availableSessionsDisplayed;
    QString m_configFileName;
    struct TVeinParam
    {
        QString m_description;
        VeinComponent::ComponentData* m_veinComponentData = nullptr;
        void setScpiInfo(const QString &model,
                         const QString &cmd,
                         int cmdTypeMask, // e.g SCPI::isQuery|SCPI::isCmdwP
                         const QString &veinComponentName,
                         SCPI::eSCPIEntryType entryType);
        void setValidator(ValidatorInterface* validator);

        void exportSCPIInfo(QJsonArray &jsArr);
        void exportMetaData(QJsonObject &jsonObj) const;
    private:
        std::shared_ptr<ScpiVeinComponentInfo> m_scpiinfo;
        ValidatorInterface* m_pValidator = nullptr;
    };
    QMap<QString, TVeinParam> m_veinParameterMap;
    QList<ScpiVeinComponentInfo*> m_scpiCatalogCmdList;
    LxdmConfigFile m_lxdmConfFile;
    std::function<bool ()> m_restartService;
    bool m_initDone=false;
    bool m_sessionReady=false;
    bool m_modulesPaused=false;
    bool m_devMode = false;
};

#endif // MODULEMANAGERSETTINGS_H
