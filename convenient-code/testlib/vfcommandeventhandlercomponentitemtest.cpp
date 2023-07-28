#include "vfcommandeventhandlercomponentitemtest.h"

std::shared_ptr<VfCommandEventHandlerComponentItemTest> VfCommandEventHandlerComponentItemTest::create(QString componentName, VfCmdEventItemEntityPtr entityItem)
{
    return std::make_shared<VfCommandEventHandlerComponentItemTest>(componentName, entityItem);
}

VfCommandEventHandlerComponentItemTest::VfCommandEventHandlerComponentItemTest(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    VfCmdEventItemComponent{componentName, entityItem}
{
}

void VfCommandEventHandlerComponentItemTest::processComponentEventData(const VeinComponent::ComponentData *cData)
{
    Q_UNUSED(cData)
    emit sigCommandEvenProcessed();
}
