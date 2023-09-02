#ifndef VFCOMMANDEVENTHANDLERCOMPONENTITEMTEST_H
#define VFCOMMANDEVENTHANDLERCOMPONENTITEMTEST_H

#include <vf_component_event_item.h>
#include <QObject>

class VfCmdEventHandlerComponentItemTest : public QObject, public VfComponentEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfCmdEventHandlerComponentItemTest> create(QString componentName, VfCmdEventItemEntityPtr entityItem);
    VfCmdEventHandlerComponentItemTest(QString componentName, VfCmdEventItemEntityPtr entityItem);

    void processComponentEventData(const VeinComponent::ComponentData *componentData) override;
    void processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData) override;
signals:
    void sigCommandEventReceived();
private:
};

typedef std::shared_ptr<VfCmdEventHandlerComponentItemTest> VfCmdEventHandlerComponentItemTestPtr;

#endif // VFCOMMANDEVENTHANDLERCOMPONENTITEMTEST_H
