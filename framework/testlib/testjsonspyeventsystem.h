#ifndef TESTJSONSPYEVENTSYSTEM_H
#define TESTJSONSPYEVENTSYSTEM_H

#include <ve_eventsystem.h>
#include <ve_eventdata.h>
#include "ve_commandevent.h"
#include <vn_protocolevent.h>
#include "QJsonObject"

class TestJsonSpyEventSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    TestJsonSpyEventSystem(QJsonObject *jsonEvents, QString roleName, bool handleProtocolEvents = false);
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
    void handleProtocolEvent(VeinNet::ProtocolEvent *pEvent, QJsonObject &jsonEventInfo);

    void baseInfoFromEventData(VeinEvent::EventData *evData, QJsonObject &jsonEventInfo);
    void extendByCommandEventInfo(VeinEvent::CommandEvent *cEvent, QJsonObject &jsonEventInfo);
    void addJsonInfo(const QJsonObject& jsonEventInfo);
    enum OldNewType {
        OLDVALUE,
        NEWVALUE
    };
    void addValue(QJsonObject &jsonEventInfo, OldNewType oldNewType, const QVariant &value);

    QJsonObject *m_jsonEvents = nullptr;
    QString m_roleName;
    bool m_handleProtocolEvents;
};

#endif // TESTJSONSPYEVENTSYSTEM_H
