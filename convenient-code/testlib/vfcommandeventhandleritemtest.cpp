#include "vfcommandeventhandleritemtest.h"

std::shared_ptr<VfCommandEventHandlerItemTest> VfCommandEventHandlerItemTest::create(int entityId)
{
    return std::make_unique<VfCommandEventHandlerItemTest>(entityId);
}

VfCommandEventHandlerItemTest::VfCommandEventHandlerItemTest(int entityId) :
    VfCmdEventItem(entityId)
{
}

void VfCommandEventHandlerItemTest::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    Q_UNUSED(cmdEvent)
    emit sigCommandEvenProcessed();
}

void VfCommandEventHandlerItemTest::processErrorCommandEventData(VeinEvent::EventData *originalEventData)
{

}
