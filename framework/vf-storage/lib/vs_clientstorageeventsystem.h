#ifndef VS_CLIENTSTORAGEEVENTSYSTEM_H
#define VS_CLIENTSTORAGEEVENTSYSTEM_H

#include "vs_databasehash.h"
#include <vs_abstracteventsystem.h>
#include <ve_eventdata.h>
#include <ve_commandevent.h>

namespace VeinStorage
{
class ClientStorageEventSystem : public AbstractEventSystem
{
    Q_OBJECT
public:
    explicit ClientStorageEventSystem(QObject *parent = nullptr);
    virtual ~ClientStorageEventSystem();

    AbstractDatabase* getDb() const override;
    QMap<int, QStringList> getRpcs() const override;
    void processEvent(QEvent *event) override;

private:
    void insertRpc(const int entityId, QStringList rpcs);
    void processIntrospectionData(QEvent *event);
    void processComponentData(VeinEvent::CommandEvent *cEvent);
    void processRpcData(VeinEvent::CommandEvent *cEvent);

    DatabaseHash *m_privHash;
    QMap<int, QStringList> m_entityRpcNames;

    QList<VeinEvent::EventData::EventOrigin> m_acceptableOrigins = // Hmmm
        {
            VeinEvent::EventData::EventOrigin::EO_LOCAL,
            VeinEvent::EventData::EventOrigin::EO_FOREIGN
        };
};
}

#endif // VS_CLIENTSTORAGEEVENTSYSTEM_H
