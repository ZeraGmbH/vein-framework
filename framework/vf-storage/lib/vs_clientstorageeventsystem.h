#ifndef VS_CLIENTSTORAGEEVENTSYSTEM_H
#define VS_CLIENTSTORAGEEVENTSYSTEM_H

#include "vs_databasehash.h"
#include <vs_abstracteventsystem.h>
#include <ve_eventdata.h>
#include <QObject>

namespace VeinStorage
{
class ClientStorageEventSystem : public AbstractEventSystem
{
    Q_OBJECT
public:
    explicit ClientStorageEventSystem(QObject *parent = nullptr);

    AbstractDatabase* getDb() const override;
    void processEvent(QEvent *event) override;

private:
    void processIntrospectionData(QEvent *event);
    void processComponentData(QEvent *event);
    void processEntityData(QEvent *event);

    DatabaseHash *m_privHash;

    QList<VeinEvent::EventData::EventOrigin> m_acceptableOrigins =
        {
            VeinEvent::EventData::EventOrigin::EO_LOCAL,
            VeinEvent::EventData::EventOrigin::EO_FOREIGN
        };
};
}

#endif // VS_CLIENTSTORAGEEVENTSYSTEM_H
