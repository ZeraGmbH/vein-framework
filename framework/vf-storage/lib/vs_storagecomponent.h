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
    void setValue(const QVariant &value) override;
    void setGetValueCustomizer(AbstractComponentGetCustomizerPtr customizer) override;

    static int getInstanceCount();

private:
    QVariant m_value;
    AbstractComponentGetCustomizerPtr m_getCustomizer;
    static int m_instanceCount;
};

}
#endif // VS_STORAGECOMPONENT_H
