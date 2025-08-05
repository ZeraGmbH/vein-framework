#include "systemmoduleeventsystem.h"
#include "stringvalidator.h"
#include <ve_eventdata.h>
#include <ve_commandevent.h>
#include <vcmp_entitydata.h>
#include <vcmp_introspectiondata.h>
#include <vf_server_entity_add.h>
#include <vf_server_component_setter.h>
#include <vf_server_component_add.h>
#include <zera-jsonfileloader.h>
#include <scpi.h>
#include <QJsonArray>
#include <QDateTime>

static const char *entityName =                        "_System";
static const char *entityNameComponentName =           "EntityName";
static const char *entitiesComponentName =             "Entities";
static const char *sessionComponentName =              "Session";
static const char *sessionsAvailableComponentName =    "SessionsAvailable";
static const char *xSessionComponentName =             "XSession";
static const char *modulesPausedComponentName =        "ModulesPaused";
static const char *devModeComponentName =              "DevMode";
static const char *moduleInterface =                   "INF_ModuleInterface";

SystemModuleEventSystem::SystemModuleEventSystem(bool devMode, const LxdmSessionChangeParam &lxdmParam) :
    m_lxdmConfFile(lxdmParam.getConfigParam()),
    m_restartService(lxdmParam.getRestartFunc()),
    m_devMode(devMode)
{
}

constexpr int SystemModuleEventSystem::getEntityId()
{
    return 0;
}

void SystemModuleEventSystem::setStorage(VeinStorage::AbstractEventSystem *t_storageSystem)
{
    m_storageSystem = t_storageSystem;
}

void SystemModuleEventSystem::processEvent(QEvent *t_event)
{
    if(t_event->type() == VeinEvent::CommandEvent::getQEventType()) {
        bool validated = false;
        VeinEvent::CommandEvent *cEvent = static_cast<VeinEvent::CommandEvent *>(t_event);
        if(cEvent->eventSubtype() != VeinEvent::CommandEvent::EventSubtype::NOTIFICATION) { // we do not need to process notifications
            if(cEvent->eventData()->type() == VeinComponent::IntrospectionData::dataType()) // introspection requests are always valid
                validated = true;
            else if(cEvent->eventData()->type() == VeinComponent::EntityData::dataType()) { // validate subscription requests
                const VeinComponent::EntityData *eData = static_cast<VeinComponent::EntityData *>(cEvent->eventData());
                if(eData->eventCommand() == VeinComponent::EntityData::Command::ECMD_SUBSCRIBE ||
                   eData->eventCommand() == VeinComponent::EntityData::Command::ECMD_UNSUBSCRIBE)
                    validated = true;
            }
            else if(cEvent->eventData()->type() == VeinComponent::ComponentData::dataType()) {
                const VeinComponent::ComponentData *cData = static_cast<VeinComponent::ComponentData *>(cEvent->eventData());
                if(cData->eventCommand() == VeinComponent::ComponentData::Command::CCMD_FETCH) {
                    // validate fetch requests
                    validated = true;
                    if(cData->entityId() == getEntityId())
                        sendNotificationForScpiModule(cData);
                }
                else if(cData->eventCommand() == VeinComponent::ComponentData::Command::CCMD_SET &&
                        cData->entityId() == getEntityId()) {
                    const QString componentName = cData->componentName();
                    if(componentName == sessionComponentName) {
                        if(handleVeinSessionSet(cData))
                            t_event->accept();
                    }
                    else if(componentName == xSessionComponentName) {
                        if(handleXSessionSet(cData->newValue().toString()))
                            validated = true;
                        else {
                            t_event->accept();
                            // swap old <-> new
                            emit sigSendEvent(VfServerComponentSetter::generateEvent(getEntityId(), cData->componentName(), cData->newValue(), cData->oldValue()));
                        }
                    }
                    else if(componentName == modulesPausedComponentName) {
                        validated = true;
                        setModulesPaused(cData->newValue().toBool());
                    }
                    else if(componentName == devModeComponentName)
                        validated = true;
                }
            }
        }
        if(validated) {
            ///@todo @bug remove inconsistent behavior by sending a new event instead of rewriting the current event
            cEvent->setEventSubtype(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION);
            cEvent->eventData()->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL); // the validated answer is authored from the system that runs the validator (aka. this system)
            cEvent->eventData()->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL); // inform all users (may or may not result in network messages)
        }
    }
}

bool SystemModuleEventSystem::handleVeinSessionSet(const VeinComponent::ComponentData *cData)
{
    QString newSession;
    if(cData->newValue().toString().endsWith(".json"))
        newSession = cData->newValue().toString();
    else
        newSession = getJsonSessionName(cData->newValue().toString());
    if(m_availableSessions.contains(newSession) && newSession != m_currentSession) {
        if(m_sessionReady == true) {
            m_currentSession = newSession;
            emit sigSendEvent(VfServerComponentSetter::generateEvent(
                getEntityId(),
                sessionComponentName,
                QVariant(),
                QVariant("")) );
            emit sigChangeSession(m_currentSession);
            m_sessionReady = false;
        }
        return true;
    }
    return false;
}

bool SystemModuleEventSystem::handleXSessionSet(const QString &xSession)
{
    if(m_lxdmConfFile.getConfiguredXSessionName() == xSession)
        return true;
    if(m_lxdmConfFile.setCurrentXSession(xSession)) {
        if(m_restartService())
            return true;
    }
    return false;
}

void SystemModuleEventSystem::setConfigFileName(QString configFileName)
{
    m_configFileName = configFileName;
}

void SystemModuleEventSystem::setAvailableSessionList(QStringList availableSessionList)
{
    QStringList setter;
    for(int i = 0; i < availableSessionList.count(); i++) {
        QString sessionName = getDisplayedSessionName(availableSessionList[i]);
        setter.append(sessionName);
    }
    m_availableSessionsDisplayed = setter;
}

void SystemModuleEventSystem::initializeEntity(const QString &sessionPath, const QStringList &sessionList)
{
    if(m_storageSystem) {
        m_sessionReady = true;
        m_currentSession = sessionPath;
        m_availableSessions = sessionList;
        emit sigSendEvent(VfServerComponentSetter::generateEvent(
            getEntityId(),
            entitiesComponentName,
            QVariant(),
            QVariant::fromValue<QList<int> >(m_storageSystem->getDb()->getEntityList())));

        emit sigSendEvent(VfServerComponentSetter::generateEvent(
            getEntityId(),
            sessionComponentName,
            QVariant(),
            m_currentSession) );

        emit sigSendEvent(VfServerComponentSetter::generateEvent(
            getEntityId(),
            xSessionComponentName,
            QVariant(),
            m_lxdmConfFile.getConfiguredXSessionName()) );

        emit sigSendEvent(VfServerComponentSetter::generateEvent(
            getEntityId(),
            sessionsAvailableComponentName,
            QVariant(),
            m_availableSessions) );

        emit sigSendEvent(VfServerComponentSetter::generateEvent(
            getEntityId(),
            devModeComponentName,
            QVariant(),
            m_devMode) );

        emit sigSendEvent(VfServerComponentSetter::generateEvent(
            getEntityId(),
            moduleInterface,
            QVariant(),
            setModuleInterface()) );
    }
    else
        qCritical() << "[SystemModuleEventSystem] AbstractEventSystem required to call initializeEntities";
}

void SystemModuleEventSystem::initOnce()
{
    if(m_initDone == false) {
        emit sigSendEvent(VfServerEntityAdd::generateEvent(getEntityId()));
        setScpiInfo();

        QHash<QString, QVariant> componentData;
        componentData.insert(entityNameComponentName, entityName);
        componentData.insert(entitiesComponentName, QVariant());
        componentData.insert(sessionComponentName, QVariant(m_currentSession));
        componentData.insert(sessionsAvailableComponentName, QVariant(m_availableSessions));
        componentData.insert(xSessionComponentName, m_lxdmConfFile.getConfiguredXSessionName());
        componentData.insert(modulesPausedComponentName, QVariant(false));
        componentData.insert(devModeComponentName, QVariant(false));
        componentData.insert(moduleInterface, QVariant());

        const auto componentNames = componentData.keys();
        for(const QString &compName : componentNames) {
            QVariant initialValue;
            if(compName == moduleInterface)
                initialValue = QVariant(setModuleInterface());
            else
                initialValue = componentData.value(compName);
            emit sigSendEvent(VfServerComponentAdd::generateEvent(getEntityId(), compName, initialValue));
        }
        m_initDone = true;
    }
}

void SystemModuleEventSystem::setModulesPaused(bool t_paused)
{
    if(t_paused != m_modulesPaused) {
        m_modulesPaused = t_paused;
        emit sigModulesPausedChanged(m_modulesPaused);
    }
}

QByteArray SystemModuleEventSystem::setModuleInterface()
{
    QJsonObject jsonObject;
    QJsonObject jsonObj2;
    QJsonObject jsonObj3;

    //------------------------ "Module Info"
    jsonObj2.insert("Description", "This module gives general information about the device");
    jsonObj2.insert("Name", entityName);
    jsonObject.insert("ModuleInfo", jsonObj2);

    //------------------------ "ComponentInfo"
    jsonObj2 = QJsonObject();
    jsonObj2.insert("Description", "Module's interface details");
    jsonObj3.insert(moduleInterface, jsonObj2);

    jsonObj2 = QJsonObject();
    jsonObj2.insert("Description", "Module's name");
    jsonObj3.insert(entityNameComponentName, jsonObj2);

    QList<QString> keyList;
    keyList = m_veinParameterMap.keys();
    for (int i = 0; i < keyList.count(); i++) {
        const QString &componentName = keyList[i];
        QJsonObject componentMetaData;
        const TVeinParam &param = m_veinParameterMap[componentName];
        componentMetaData.insert("Description", param.m_description);
        param.exportMetaData(componentMetaData);
        jsonObj3.insert(componentName, componentMetaData);
    }
    jsonObject.insert("ComponentInfo", jsonObj3);

    //------------------------ "SCPIInfo"
    QJsonArray jsonArr;
    for (int i = 0; i < keyList.count(); i++) {
        m_veinParameterMap[keyList.at(i)].exportSCPIInfo(jsonArr);
    }
    for (int i = 0; i < m_scpiCatalogCmdList.count(); i++) {
        m_scpiCatalogCmdList.at(i)->appendSCPIInfo(jsonArr);
    }
    jsonObj2 = QJsonObject();
    jsonObj2.insert("Name", "SYST");
    jsonObj2.insert("Cmd", jsonArr);
    jsonObject.insert("SCPIInfo", jsonObj2);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);
    QByteArray ba;
    ba = jsonDoc.toJson();
    return ba;
}

QString SystemModuleEventSystem::deduceDeviceName(const QString &sessionString)
{
    if(sessionString.contains("mt310s2"))
        return "mt310s2";
    else if(sessionString.contains("com5003"))
        return "com5003";
    return "unknown";
}

QString SystemModuleEventSystem::getJsonSessionName(const QString &displayedSessionName)
{
    QString device = deduceDeviceName(m_availableSessions[0]); // ??? Really what about COM5003 ???
    QJsonObject jsonConfig = cJsonFileLoader::loadJsonFile(m_configFileName).value(device).toObject();
    QJsonArray availableSessions = jsonConfig["availableSessions"].toArray();
    QJsonArray sessionDisplayStrings = jsonConfig["sessionDisplayStrings"].toArray();
    for(int i = 0; i < sessionDisplayStrings.count(); i++) {
        if(displayedSessionName == sessionDisplayStrings[i].toString())
            return availableSessions[i].toString();
    }
    return QString();
}

QString SystemModuleEventSystem::getDisplayedSessionName(const QString &jsonSessionName)
{
    QString device = deduceDeviceName(jsonSessionName);
    QJsonObject jsonConfig = cJsonFileLoader::loadJsonFile(m_configFileName).value(device).toObject();
    QJsonArray availableSessions = jsonConfig["availableSessions"].toArray();
    QJsonArray sessionDisplayStrings = jsonConfig["sessionDisplayStrings"].toArray();
    for(int i = 0; i < availableSessions.count(); i++) {
        if(jsonSessionName == availableSessions[i].toString())
            return sessionDisplayStrings[i].toString();
    }
    return QString();
}

void SystemModuleEventSystem::sendNotificationForScpiModule(const VeinComponent::ComponentData *cData)
{
    if(cData->componentName() == sessionComponentName)
        sendSessionNotificationForScpiModule(cData);
    else if(cData->componentName() == xSessionComponentName)
        emit sigSendEvent(VfServerComponentSetter::generateEvent(getEntityId(),
                                                                 cData->componentName(),
                                                                 QVariant(), m_lxdmConfFile.getConfiguredXSessionName()));
}

void SystemModuleEventSystem::sendSessionNotificationForScpiModule(const VeinComponent::ComponentData *cData)
{
    VeinComponent::ComponentData *componentData = new VeinComponent::ComponentData();
    componentData->setEntityId(getEntityId());
    componentData->setComponentName(cData->componentName());
    QString value = getDisplayedSessionName(cData->oldValue().toString());
    componentData->setNewValue(value);
    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, componentData);
    emit sigSendEvent(event);
}

void SystemModuleEventSystem::setScpiInfo()
{
    struct TVeinParam param;
    VeinComponent::ComponentData *initialData = nullptr;

    initialData = new VeinComponent::ComponentData();
    param.m_description = "Session name";
    param.m_veinComponentData = initialData;
    param.setScpiInfo("CONFIGURATION", "NAMESESSION", SCPI::isQuery|SCPI::isCmdwP, sessionComponentName, SCPI::isComponent);
    cStringValidator *sValidator = new cStringValidator(m_availableSessionsDisplayed);
    param.setValidator(sValidator);
    m_veinParameterMap[sessionComponentName] = param;
    m_scpiCatalogCmdList.append(new ScpiVeinComponentInfo("CONFIGURATION", "SESSION:CATALOG", SCPI::isQuery, sessionComponentName, SCPI::isCatalog));

    initialData = new VeinComponent::ComponentData();
    param.m_description = "XSession name";
    param.m_veinComponentData = initialData;
    param.setScpiInfo("CONFIGURATION", "XSESSION", SCPI::isQuery|SCPI::isCmdwP, xSessionComponentName, SCPI::isComponent);
    sValidator = new cStringValidator(m_lxdmConfFile.getAvailableXSessionNames());
    param.setValidator(sValidator);
    m_veinParameterMap[xSessionComponentName] = param;
    m_scpiCatalogCmdList.append(new ScpiVeinComponentInfo("CONFIGURATION", "XSESSION:CATALOG", SCPI::isQuery, xSessionComponentName, SCPI::isCatalog));
}

void SystemModuleEventSystem::TVeinParam::setScpiInfo(const QString &model,
                                                      const QString &cmd,
                                                      int cmdTypeMask,
                                                      const QString &veinComponentName,
                                                      SCPI::eSCPIEntryType entryType)
{
    m_scpiinfo = std::make_shared<ScpiVeinComponentInfo>(model,
                                                         cmd,
                                                         cmdTypeMask,
                                                         veinComponentName,
                                                         entryType);
}

void SystemModuleEventSystem::TVeinParam::setValidator(ValidatorInterface *validator)
{
    m_pValidator = validator;
}

void SystemModuleEventSystem::TVeinParam::exportSCPIInfo(QJsonArray &jsArr)
{
    if (m_scpiinfo)
        m_scpiinfo->appendSCPIInfo(jsArr);
}

void SystemModuleEventSystem::TVeinParam::exportMetaData(QJsonObject &jsonObj) const
{
    if (m_pValidator != nullptr) {
        QJsonObject jsonObj2;
        m_pValidator->exportMetaData(jsonObj2);
        jsonObj.insert("Validation", jsonObj2);
    }
}
