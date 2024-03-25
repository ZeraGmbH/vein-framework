#ifndef VEINHASH_H
#define VEINHASH_H

#include "vfstoragehash_export.h"
#include <ve_storagesystem.h>
#include <ve_eventdata.h>
#include <QHash>

Q_DECLARE_LOGGING_CATEGORY(VEIN_STORAGE_HASH)
Q_DECLARE_LOGGING_CATEGORY(VEIN_STORAGE_HASH_VERBOSE)

namespace VeinComponent
{
class ComponentData;
class EntityData;
}

namespace VeinStorage
{
class VFSTORAGEHASH_EXPORT VeinHash : public VeinEvent::StorageSystem
{
    Q_OBJECT
public:
    explicit VeinHash(QObject *parent = nullptr);
    void setAcceptableOrigin(QList<VeinEvent::EventData::EventOrigin> origins);
    const QList<VeinEvent::EventData::EventOrigin> &getAcceptableOrigin() const;

    //stands for QHash<"entity descriptor", QHash<"component name", "component data">*>
    template <typename T>
    using ComponentStorage = QHash<T, QHash<QString, QVariant>*>;

    //VeinEvent::StorageSystem interface
public:
    void processEvent(QEvent *event) override;
    void dumpToFile(QIODevice *outputFileDevice, QList<int> entityFilter = QList<int>()) const  override;
    Q_INVOKABLE QVariant getStoredValue(int entityId, const QString &componentName) const override;
    Q_INVOKABLE bool hasStoredValue(int entityId, const QString &componentName) const override;
    QList<QString> getEntityComponents(int entityId) const override;
    bool hasEntity(int entityId) const override;
    QList<int> getEntityList() const override;
private:
    void processComponentData(VeinComponent::ComponentData *cData);
    void processEntityData(VeinComponent::EntityData *eData);
    void sendError(const QString &errorString, VeinEvent::EventData *data);

    QHash<int, QHash<QString, QVariant>> m_entityComponentData;
    QList<VeinEvent::EventData::EventOrigin> m_acceptableOrigins = {VeinEvent::EventData::EventOrigin::EO_LOCAL, VeinEvent::EventData::EventOrigin::EO_FOREIGN};
};
}

#endif // VEINHASH_H
