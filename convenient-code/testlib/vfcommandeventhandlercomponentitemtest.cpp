#include "vfcommandeventhandlercomponentitemtest.h"

std::shared_ptr<VfCommandEventHandlerComponentItemTest> VfCommandEventHandlerComponentItemTest::create(int entityId, QString componentName)
{
    return std::make_shared<VfCommandEventHandlerComponentItemTest>(entityId, componentName);
}

VfCommandEventHandlerComponentItemTest::VfCommandEventHandlerComponentItemTest(int entityId, QString componentName) :
    VfCmdEventItemComponent{entityId, componentName}
{
}

void VfCommandEventHandlerComponentItemTest::processComponentEventData(const VeinComponent::ComponentData *cData)
{
    Q_UNUSED(cData)
    emit sigCommandEvenProcessed();
}
