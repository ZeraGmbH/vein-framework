#ifndef VS_STORAGECOMPONENT_H
#define VS_STORAGECOMPONENT_H

#include "vs_abstractcomponent.h"

namespace VeinStorage
{

class StorageComponent : public AbstractComponent
{
    Q_OBJECT
public:
    StorageComponent(QVariant value);
    QVariant getValue() const override;
private:
    friend class VeinHashPrivate;
    void setValue(QVariant value);
    QVariant m_value;
};

typedef std::shared_ptr<StorageComponent> StorageComponentPtr;
typedef QHash<QString, StorageComponentPtr> EntityMap;

}
#endif // VS_STORAGECOMPONENT_H
