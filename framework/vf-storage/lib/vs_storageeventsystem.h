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



    bool hasEntity(int entityId) const override;
    QList<int> getEntityList() const override;

    Q_INVOKABLE bool hasStoredValue(int entityId, const QString &componentName) const override;
    Q_INVOKABLE QVariant getStoredValue(int entityId, const QString &componentName) const override;
    QList<QString> getEntityComponents(int entityId) const override;

    AbstractComponentPtr getComponent(int entityId, const QString &componentName) const override;
    AbstractComponentPtr getFutureComponent(int entityId, const QString &componentName) override;

    void dumpToFile(QIODevice *outputFileDevice, QList<int> entityFilter = QList<int>()) const  override;
    bool areFutureComponentsEmpty() override;

    void setAcceptableOrigin(QList<VeinEvent::EventData::EventOrigin> origins); // Not too many users (yet)

private:
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

#endif // VS_STORAGEEVENTSYSTEM_H
