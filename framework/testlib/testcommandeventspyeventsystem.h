#ifndef TESTCOMMANDEVENTSPYEVENTSYSTEM_H
#define TESTCOMMANDEVENTSPYEVENTSYSTEM_H

#include <ve_eventsystem.h>
#include <ve_eventdata.h>
#include "ve_commandevent.h"
#include <vcmp_entitydata.h>
#include "QJsonObject"

class TestCommandEventSpyEventSystem : public VeinEvent::EventSystem
{
public:
    void processEvent(QEvent *event) override;
    void clear();
    bool isEmpty() const;
    QByteArray dumpToJson();
private:
    void handleEntityData(VeinEvent::EventData *evData, QJsonObject& jsonEventInfo);
    void handleComponentData(VeinEvent::EventData *evData, QJsonObject& jsonEventInfo);
    void handleIntrospectionData(VeinEvent::EventData *evData, QJsonObject& jsonEventInfo);
    void handleRpcData(VeinEvent::EventData *evData, QJsonObject& jsonEventInfo);
    void handleErrorData(VeinEvent::EventData *evData, QJsonObject& jsonEventInfo);

    QJsonObject commonInfo(VeinEvent::CommandEvent *cEvent, VeinEvent::EventData *evData);
    void addJsonInfo(const QJsonObject& jsonEventInfo);
    QString strSubtype(VeinEvent::CommandEvent::EventSubtype subtype);
    QString strOrigin(qint8 origin);
    QString strTarget(qint8 target);
    QString strEntityCommand(VeinComponent::EntityData::Command cmd);

    QJsonObject m_jsonEvents;
};

#endif // TESTCOMMANDEVENTSPYEVENTSYSTEM_H
