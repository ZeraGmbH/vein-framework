#include "vf_cmd_event_handler_component_item_test.h"

std::shared_ptr<VfCmdEventHandlerComponentItemTest> VfCmdEventHandlerComponentItemTest::create(QString componentName, VfCmdEventItemEntityPtr entityItem)
{
    return std::make_shared<VfCmdEventHandlerComponentItemTest>(componentName, entityItem);
}

VfCmdEventHandlerComponentItemTest::VfCmdEventHandlerComponentItemTest(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    VfCmdEventItemComponent{componentName, entityItem}
{
}

void VfCmdEventHandlerComponentItemTest::processComponentEventData(const VeinComponent::ComponentData *componentData)
{
    Q_UNUSED(componentData)
    emit sigCommandEventReceived();
}

void VfCmdEventHandlerComponentItemTest::processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData)
{
    Q_UNUSED(originalComponentData)
}
