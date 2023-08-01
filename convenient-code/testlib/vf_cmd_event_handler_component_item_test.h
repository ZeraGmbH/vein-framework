#ifndef VFCOMMANDEVENTHANDLERCOMPONENTITEMTEST_H
#define VFCOMMANDEVENTHANDLERCOMPONENTITEMTEST_H

#include <vf_cmd_event_item_component.h>
#include <QObject>

class VfCmdEventHandlerComponentItemTest : public QObject, public VfCmdEventItemComponent
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
