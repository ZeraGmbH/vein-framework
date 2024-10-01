#include "systemmoduleeventsystem.h"
#include "stringvalidator.h"
#include <ve_eventdata.h>
#include <ve_commandevent.h>
#include <vcmp_entitydata.h>
#include <vcmp_introspectiondata.h>
#include <vf_server_component_setter.h>
#include <zera-jsonfileloader.h>
#include <QJsonArray>
#include <QDateTime>

static const char *entityName =                        "_System";
static const char *entityNameComponentName =           "EntityName";
static const char *entitiesComponentName =             "Entities";
static const char *sessionComponentName =              "Session";
static const char *sessionsAvailableComponentName =    "SessionsAvailable";
static const char *notificationMessagesComponentName = "Error_Messages";
static const char *modulesPausedComponentName =        "ModulesPaused";
static const char *devModeComponentName =              "DevMode";
static const char *moduleInterface =                   "INF_ModuleInterface";

SystemModuleEventSystem::SystemModuleEventSystem(QObject *t_parent, bool devMode) :
    VeinEvent::EventSystem(t_parent),
    m_devMode(devMode)
{
}

constexpr int SystemModuleEventSystem::getEntityId()
{
    return 0;
}

VeinEvent::StorageSystem *SystemModuleEventSystem::getStorageSystem() const
{
    return m_storageSystem;
}

void SystemModuleEventSystem::setStorage(VeinEvent::StorageSystem *t_storageSystem)
{
    m_storageSystem = t_storageSystem;
}

void SystemModuleEventSystem::processEvent(QEvent *t_event)
{
    if(t_event->type() == VeinEvent::CommandEvent::getQEventType()) {
        bool validated = false;
        VeinEvent::CommandEvent *cEvent = static_cast<VeinEvent::CommandEvent *>(t_event);
        Q_ASSERT(cEvent != nullptr);
        if(cEvent->eventSubtype() != VeinEvent::CommandEvent::EventSubtype::NOTIFICATION) { // we do not need to process notifications
            if(cEvent->eventData()->type() == VeinComponent::IntrospectionData::dataType()) // introspection requests are always valid
                validated = true;
            else if(cEvent->eventData()->type() == VeinComponent::EntityData::dataType()) // validate subscription requests
            {
                VeinComponent::EntityData *eData = static_cast<VeinComponent::EntityData *>(cEvent->eventData());
                Q_ASSERT(eData != nullptr);
                if(eData->eventCommand() == VeinComponent::EntityData::Command::ECMD_SUBSCRIBE
                        || eData->eventCommand() == VeinComponent::EntityData::Command::ECMD_UNSUBSCRIBE) { /// @todo maybe add roles/views later
                    validated = true;
                }
            }
            else if(cEvent->eventData()->type() == VeinComponent::ComponentData::dataType()) {
                VeinComponent::ComponentData *cData = static_cast<VeinComponent::ComponentData *>(cEvent->eventData());
                Q_ASSERT(cData != nullptr);
                // validate fetch requests
                if(cData->eventCommand() == VeinComponent::ComponentData::Command::CCMD_FETCH) {
                    validated = true;
                    if(cData->entityId() == getEntityId() && cData->componentName() == sessionComponentName)
                        sendSessionNotificationForScpiModule(cData);
                }
                else if(cData->eventCommand() == VeinComponent::ComponentData::Command::CCMD_SET &&
                        cData->entityId() == getEntityId()) {
                    // validate set event for _System.Session
                    if(cData->componentName() == sessionComponentName) {
                        QString newSession;
                        if(cData->newValue().toString().endsWith(".json"))
                            newSession = cData->newValue().toString();
                        else
                            newSession = fromSessionNameToJsonName(cData->newValue().toString());
                        if(m_availableSessions.contains(newSession) && newSession != m_currentSession) {
                            if(m_sessionReady == true) {
                                m_currentSession = newSession;
                                emit sigChangeSession(m_currentSession);
                                m_sessionReady = false;
                            }
                            t_event->accept();
                        }
                    }
                    else if(cData->componentName() == notificationMessagesComponentName) {
                        handleNotificationMessage(cData->newValue().toJsonObject());
                        t_event->accept();
                    }
                    else if(cData->componentName() == modulesPausedComponentName) {
                        validated = true;
                        setModulesPaused(cData->newValue().toBool());
                    }
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

void SystemModuleEventSystem::setConfigFileName(QString configFileName)
{
    m_configFileName = configFileName;
}

void SystemModuleEventSystem::setAvailableSessionList(QStringList availableSessionList)
{
    QStringList setter;
    for(int i = 0; i < availableSessionList.count(); i++) {
        QString sessionName = fromJsonNameToSessionName(availableSessionList[i]);
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
            QVariant::fromValue<QList<int> >(m_storageSystem->getEntityList())));

        emit sigSendEvent(VfServerComponentSetter::generateEvent(
            getEntityId(),
            sessionComponentName,
            QVariant(),
            m_currentSession) );

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
        qCritical() << "[SystemModuleEventSystem] StorageSystem required to call initializeEntities";
}

void SystemModuleEventSystem::initOnce()
{
    if(m_initDone == false) {
        VeinComponent::EntityData *systemData = new VeinComponent::EntityData();
        systemData->setCommand(VeinComponent::EntityData::Command::ECMD_ADD);
        systemData->setEntityId(getEntityId());

        emit sigSendEvent(new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, systemData));
        setScpiInfo();

        QHash<QString, QVariant> componentData;
        componentData.insert(entityNameComponentName, entityName);
        componentData.insert(entitiesComponentName, QVariant());
        componentData.insert(sessionComponentName, QVariant(m_currentSession));
        componentData.insert(sessionsAvailableComponentName, QVariant(m_availableSessions));
        componentData.insert(notificationMessagesComponentName, QVariant(m_notificationMessages.toJson()));
        componentData.insert(modulesPausedComponentName, QVariant(false));
        componentData.insert(devModeComponentName, QVariant(false));
        componentData.insert(moduleInterface, QVariant());

        const auto componentNames = componentData.keys();
        for(const QString &compName : componentNames) {
            VeinComponent::ComponentData *initialData = new VeinComponent::ComponentData();
            initialData->setEntityId(getEntityId());
            initialData->setCommand(VeinComponent::ComponentData::Command::CCMD_ADD);
            initialData->setComponentName(compName);
            if(compName == moduleInterface) {
                QVariant doc = QVariant(setModuleInterface());
                initialData->setNewValue(doc);
            }
            else
                initialData->setNewValue(componentData.value(compName));
            initialData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
            initialData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
            emit sigSendEvent(new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, initialData));
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

void SystemModuleEventSystem::handleNotificationMessage(QJsonObject t_message)
{
    Q_ASSERT(t_message.isEmpty() == false);
    VeinComponent::ComponentData *notificationMessagesData = new VeinComponent::ComponentData();
    VeinEvent::CommandEvent *emDataEvent = nullptr;
    notificationMessagesData->setEntityId(getEntityId());
    notificationMessagesData->setCommand(VeinComponent::ComponentData::Command::CCMD_SET);
    notificationMessagesData->setComponentName(notificationMessagesComponentName);
    notificationMessagesData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    notificationMessagesData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);

    QJsonArray tmpArray = m_notificationMessages.array();
    tmpArray.append(t_message);
    m_notificationMessages.setArray(tmpArray);
    notificationMessagesData->setNewValue(m_notificationMessages.toJson());

    emDataEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, notificationMessagesData);
    emit sigSendEvent(emDataEvent);
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
    jsonObj2.insert("Desciption", "Module's name");
    jsonObj3.insert(entityNameComponentName, jsonObj2);

    jsonObj2 = QJsonObject();
    jsonObj2.insert("Description", "Session name");

    QList<QString> keyList;
    keyList = m_veinSystemParameterMap.keys();
    for (int i = 0; i < keyList.count(); i++)
        m_veinSystemParameterMap[keyList.at(i)]->exportMetaData(jsonObj2);

    jsonObj3.insert(sessionComponentName, jsonObj2);
    jsonObject.insert("ComponentInfo", jsonObj3);

    //------------------------ "SCPIInfo"
    QJsonArray jsonArr;
    for (int i = 0; i < keyList.count(); i++) {
        m_veinSystemParameterMap[keyList.at(i)]->exportSCPIInfo(jsonArr);
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

QString SystemModuleEventSystem::fromSessionNameToJsonName(QString sessionName)
{
    QString jsonSessionName = "";
    QString device = deduceDeviceName(m_availableSessions[0]);
    QJsonObject jsonConfig = cJsonFileLoader::loadJsonFile(m_configFileName).value(device).toObject();
    QJsonArray availableSessions = jsonConfig["availableSessions"].toArray();
    QJsonArray sessionDisplayStrings = jsonConfig["sessionDisplayStrings"].toArray();
    for(int i = 0; i < sessionDisplayStrings.count(); i++) {
        if(sessionName == sessionDisplayStrings[i].toString())
            jsonSessionName = availableSessions[i].toString();
    }
    return jsonSessionName;
}

QString SystemModuleEventSystem::fromJsonNameToSessionName(QString jsonName)
{
    QString sessionName = "";
    QString device = deduceDeviceName(jsonName);
    QJsonObject jsonConfig = cJsonFileLoader::loadJsonFile(m_configFileName).value(device).toObject();
    QJsonArray availableSessions = jsonConfig["availableSessions"].toArray();
    QJsonArray sessionDisplayStrings = jsonConfig["sessionDisplayStrings"].toArray();
    for(int i = 0; i < availableSessions.count(); i++) {
        if(jsonName == availableSessions[i].toString())
            sessionName = sessionDisplayStrings[i].toString();
    }
    return sessionName;
}

void SystemModuleEventSystem::sendSessionNotificationForScpiModule(VeinComponent::ComponentData *cData)
{
    VeinComponent::ComponentData *componentData = new VeinComponent::ComponentData();
    componentData->setEntityId(getEntityId());
    componentData->setComponentName(cData->componentName());
    QString value = fromJsonNameToSessionName(cData->oldValue().toString());
    componentData->setNewValue(value);
    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, componentData);
    emit sigSendEvent(event);
}

void SystemModuleEventSystem::setScpiInfo()
{
    VeinComponent::ComponentData *initialData = nullptr;
    initialData = new VeinComponent::ComponentData();
    cSCPIInfo* scpiInfo = new cSCPIInfo("CONFIGURATION", "NAMESESSION", "10", sessionComponentName, "0", "");
    initialData->setSCPIInfo(scpiInfo);
    cStringValidator *sValidator = new cStringValidator(m_availableSessionsDisplayed);
    initialData->setValidator(sValidator);
    m_veinSystemParameterMap[sessionComponentName] = initialData;
}
