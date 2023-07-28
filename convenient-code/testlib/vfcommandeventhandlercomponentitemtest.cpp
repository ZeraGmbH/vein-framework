#include "vfcommandeventhandlercomponentitemtest.h"

std::shared_ptr<VfCommandEventHandlerComponentItemTest> VfCommandEventHandlerComponentItemTest::create(int entityId, QString componentName)
{
    return std::make_shared<VfCommandEventHandlerComponentItemTest>(entityId, componentName);
}

VfCommandEventHandlerComponentItemTest::VfCommandEventHandlerComponentItemTest(int entityId, QString componentName) :
    VfCommandEventHandlerComponentItem{entityId, componentName}
{
}

void VfCommandEventHandlerComponentItemTest::processComponentCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    Q_UNUSED(cmdEvent)
    emit sigCommandEvenProcessed();
}
