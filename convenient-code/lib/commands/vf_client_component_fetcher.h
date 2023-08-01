#ifndef VFATOMICCLIENTCOMPONENTFETCHER_H
#define VFATOMICCLIENTCOMPONENTFETCHER_H

#include <vf_cmd_event_item_component.h>
#include <memory>

// * Client fetcher
// * Servers have VfStorageHash
class VfClientComponentFetcher : public QObject, public VfCmdEventItemComponent
{
    Q_OBJECT
public:
    static std::shared_ptr<VfClientComponentFetcher> create(QString componentName, VfCmdEventItemEntityPtr entityItem);
    VfClientComponentFetcher(QString componentName, VfCmdEventItemEntityPtr entityItem);

    void startGetComponent();
    void processComponentEventData(const VeinComponent::ComponentData *componentData) override;
    void processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData) override;
signals:
    void sigGetFinish(bool ok, QVariant value);
};

typedef std::shared_ptr<VfClientComponentFetcher> VfClientComponentFetcherPtr;

#endif // VFATOMICCLIENTCOMPONENTFETCHER_H
