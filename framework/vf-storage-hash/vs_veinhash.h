#ifndef VEINHASH_H
#define VEINHASH_H

#include "vfstoragehash_export.h"
#include "vs_veinhashprivate.h"
#include <ve_storagesystem.h>
#include <vcmp_componentdata.h>
#include <ve_eventdata.h>

namespace VeinStorage
{
class VFSTORAGEHASH_EXPORT VeinHash : public VeinEvent::StorageSystem
{
    Q_OBJECT
public:
    explicit VeinHash(QObject *parent = nullptr);
    virtual ~VeinHash();

    void processEvent(QEvent *event) override;

    bool hasEntity(int entityId) const override;
    QList<int> getEntityList() const override;

    Q_INVOKABLE bool hasStoredValue(int entityId, const QString &componentName) const override;
    Q_INVOKABLE QVariant getStoredValue(int entityId, const QString &componentName) const override;
    QList<QString> getEntityComponents(int entityId) const override;

    VeinEvent::StorageComponentInterfacePtr getComponent(int entityId, const QString &componentName) const override;
    VeinEvent::StorageComponentInterfacePtr getFutureComponent(int entityId, const QString &componentName) override;

    void dumpToFile(QIODevice *outputFileDevice, QList<int> entityFilter = QList<int>()) const  override;
    bool areFutureComponentsEmpty() override;

    void setAcceptableOrigin(QList<VeinEvent::EventData::EventOrigin> origins); // Not too many users (yet)

private:
    void processComponentData(QEvent *event);
    void processEntityData(QEvent *event);

    VeinHashPrivate *m_privHash;

    QList<VeinEvent::EventData::EventOrigin> m_acceptableOrigins =
        {
            VeinEvent::EventData::EventOrigin::EO_LOCAL,
            VeinEvent::EventData::EventOrigin::EO_FOREIGN
        };
};
}

#endif // VEINHASH_H
