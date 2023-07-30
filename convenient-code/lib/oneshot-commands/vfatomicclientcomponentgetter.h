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
signals:
    void sigGetFinish(bool ok, QVariant value);
private:
    void processComponentEventData(const VeinComponent::ComponentData *cData) override;
};

typedef std::shared_ptr<VfAtomicClientComponentGetter> VfAtomicClientComponentGetterPtr;

#endif // VFATOMICCLIENTCOMPONENTGETTER_H
