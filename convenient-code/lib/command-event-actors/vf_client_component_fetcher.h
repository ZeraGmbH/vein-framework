#ifndef VFATOMICCLIENTCOMPONENTFETCHER_H
#define VFATOMICCLIENTCOMPONENTFETCHER_H

#include <vf_component_event_item.h>
#include <QVariant>
#include <memory>

// * Client fetcher
// * Servers have VfStorage
class VfClientComponentFetcher : public QObject, public VfComponentEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfClientComponentFetcher> create(QString componentName, VfCmdEventItemEntityPtr entityItem);
    VfClientComponentFetcher(QString componentName, VfCmdEventItemEntityPtr entityItem);

    static QEvent* generateEvent(int entityId, QString componentName);
    void startGetComponent();
    void processComponentEventData(const VeinComponent::ComponentData *componentData) override;
    void processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData) override;
signals:
    void sigGetFinish(bool ok, QVariant value);
};

typedef std::shared_ptr<VfClientComponentFetcher> VfClientComponentFetcherPtr;

#endif // VFATOMICCLIENTCOMPONENTFETCHER_H
