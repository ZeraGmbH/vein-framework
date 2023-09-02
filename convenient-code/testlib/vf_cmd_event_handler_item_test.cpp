#include "vf_cmd_event_handler_item_test.h"

std::shared_ptr<VfCmdEventHandlerItemTest> VfCmdEventHandlerItemTest::create(int entityId)
{
    return std::make_unique<VfCmdEventHandlerItemTest>(entityId);
}

VfCmdEventHandlerItemTest::VfCmdEventHandlerItemTest(int entityId) :
    VfEntityEventItem(entityId)
{
}

void VfCmdEventHandlerItemTest::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    Q_UNUSED(cmdEvent)
    emit sigCommandEventReceived();
}

void VfCmdEventHandlerItemTest::processErrorCommandEventData(VeinEvent::EventData *originalEventData)
{
    Q_UNUSED(originalEventData)
    emit sigCommandErrorEventReceived();
}
