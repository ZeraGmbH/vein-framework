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

    QVariant getValue() const override;
    static int getInstanceCount();
private:
    friend class DatabaseHash;
    void setValue(QVariant value);
    QVariant m_value;
    static int m_instanceCount;
};

typedef std::shared_ptr<StorageComponent> StorageComponentPtr;
typedef QHash<QString, StorageComponentPtr> EntityMap;

}
#endif // VS_STORAGECOMPONENT_H
