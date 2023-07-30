#ifndef VFATOMICCLIENTCOMPONENTGETTER_H
#define VFATOMICCLIENTCOMPONENTGETTER_H

#include <vfcmdeventitemcomponent.h>
#include <memory>

// * Client getter
// * Servers have VfStorageHash
class VfAtomicClientComponentGetter : public QObject, public VfCmdEventItemComponent
{
    Q_OBJECT
public:
    static std::shared_ptr<VfAtomicClientComponentGetter> create(QString componentName, VfCmdEventItemEntityPtr entityItem);
    VfAtomicClientComponentGetter(QString componentName, VfCmdEventItemEntityPtr entityItem);

    void startGetComponent();
    void processComponentEventData(const VeinComponent::ComponentData *componentData) override;
    void processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData) override;
signals:
    void sigGetFinish(bool ok, QVariant value);
};

typedef std::shared_ptr<VfAtomicClientComponentGetter> VfAtomicClientComponentGetterPtr;

#endif // VFATOMICCLIENTCOMPONENTGETTER_H
