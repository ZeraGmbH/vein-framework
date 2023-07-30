#ifndef VFATOMICCLIENTCOMPONENTFETCHER_H
#define VFATOMICCLIENTCOMPONENTFETCHER_H

#include <vfcmdeventitemcomponent.h>
#include <memory>

// * Client fetcher
// * Servers have VfStorageHash
class VfAtomicClientComponentFetcher : public QObject, public VfCmdEventItemComponent
{
    Q_OBJECT
public:
    static std::shared_ptr<VfAtomicClientComponentFetcher> create(QString componentName, VfCmdEventItemEntityPtr entityItem);
    VfAtomicClientComponentFetcher(QString componentName, VfCmdEventItemEntityPtr entityItem);

    void startGetComponent();
    void processComponentEventData(const VeinComponent::ComponentData *componentData) override;
    void processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData) override;
signals:
    void sigGetFinish(bool ok, QVariant value);
};

typedef std::shared_ptr<VfAtomicClientComponentFetcher> VfAtomicClientComponentFetcherPtr;

#endif // VFATOMICCLIENTCOMPONENTFETCHER_H
