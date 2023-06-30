#ifndef CPPENTITYCOMPONENTMAP_H
#define CPPENTITYCOMPONENTMAP_H

#include "vcmp_componentdata.h"
#include <QObject>
#include <memory>

class CppEntityComponentMap : public QObject
{
    Q_OBJECT
public:
    explicit CppEntityComponentMap(int entityId, const QVariantHash &entityIntrospection);
    enum class DataState : int {
        ECM_NONE = -1, /**< uninitialized */
        ECM_PENDING = 0, /**< introspection is available but values are not initialized */
        ECM_READY = 1, /**< everything is available */
        ECM_REMOVED = 2, /**< the entity has been removed from the remote end */
    };
    DataState state() const;
    void setState(DataState t_dataState);
    void processComponentData(VeinComponent::ComponentData *cData);

signals:
    void sigSendEvent(QEvent *event);
    void sigEntityComplete(int entityId);
    void sigStateChanged(CppEntityComponentMap::DataState state);
};

typedef std::shared_ptr<CppEntityComponentMap> CppEntityComponentMapPtr;

#endif // CPPENTITYCOMPONENTMAP_H
