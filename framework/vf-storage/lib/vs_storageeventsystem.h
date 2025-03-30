#ifndef VS_STORAGEEVENTSYSTEM_H
#define VS_STORAGEEVENTSYSTEM_H

#include "vfstorage_export.h"
#include "vs_databasehash.h"
#include <vs_abstracteventsystem.h>
#include <vcmp_componentdata.h>
#include <ve_eventdata.h>

namespace VeinStorage
{
class VFSTORAGE_EXPORT StorageEventSystem : public AbstractEventSystem
{
    Q_OBJECT
public:
    explicit StorageEventSystem(QObject *parent = nullptr);
    virtual ~StorageEventSystem();

    void processEvent(QEvent *event) override;
    AbstractDatabase* getDb() const override;
    QMap<int,QStringList> getRpcs() const override;

private:
    void processComponentData(QEvent *event);
    void processEntityData(QEvent *event);
    void processRpcData(QEvent *event);

    DatabaseHash *m_privHash;
    QMap<int, QStringList> m_entityRpcNames;

    QList<VeinEvent::EventData::EventOrigin> m_acceptableOrigins = // Hmmm
        {
            VeinEvent::EventData::EventOrigin::EO_LOCAL,
            VeinEvent::EventData::EventOrigin::EO_FOREIGN
        };
};
}

#endif // VS_STORAGEEVENTSYSTEM_H
