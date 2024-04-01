#ifndef TESTJSONSPYEVENTSYSTEM_H
#define TESTJSONSPYEVENTSYSTEM_H

#include <ve_eventsystem.h>
#include <ve_eventdata.h>
#include "ve_commandevent.h"
#include "QJsonObject"

class TestJsonSpyEventSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    TestJsonSpyEventSystem(QJsonObject *jsonEvents, QString roleName);
    void processEvent(QEvent *event) override;
    void clear();
    bool isEmpty() const;
public slots:
    void onEventAccepted(VeinEvent::EventSystem* eventSystem, QEvent *event);
private:
    QJsonObject eventToJsonInfo(QEvent *event);
    void handleEntityData(VeinEvent::EventData *evData, QJsonObject& jsonEventInfo);
    void handleComponentData(VeinEvent::EventData *evData, QJsonObject& jsonEventInfo);
    void handleIntrospectionData(VeinEvent::EventData *evData, QJsonObject& jsonEventInfo);
    void handleRpcData(VeinEvent::EventData *evData, QJsonObject& jsonEventInfo);
    void handleErrorData(VeinEvent::EventData *evData, QJsonObject& jsonEventInfo);

    QJsonObject baseInfoFromEventData(VeinEvent::EventData *evData);
    void extendByEventInfo(VeinEvent::CommandEvent *cEvent, QJsonObject &jsonInfo);
    void addJsonInfo(const QJsonObject& jsonEventInfo);

    QJsonObject *m_jsonEvents = nullptr;
    QString m_roleName;
};

#endif // TESTJSONSPYEVENTSYSTEM_H
