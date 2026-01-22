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
    virtual ~StorageComponent();

    const QVariant &getValue() const override;
    QVariant &getValueForWrite();

    static int getInstanceCount();
private:
    friend class DatabaseHash;
    void setValue(const QVariant &value);
    QVariant m_value;
    static int m_instanceCount;
};

typedef std::shared_ptr<StorageComponent> StorageComponentPtr;
typedef QHash<QString, StorageComponentPtr> EntityMap;

}
#endif // VS_STORAGECOMPONENT_H
